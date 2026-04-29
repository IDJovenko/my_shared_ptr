#pragma once
#include <cstddef>

#include "ControlBlock.hpp"

namespace ef::smart_pointers {

template <typename T>
class WeakPtr;

template <typename T>
class SharedPtr {
 public:
  explicit SharedPtr(T* ptr = nullptr);
  SharedPtr(const SharedPtr& other) noexcept;
  SharedPtr(SharedPtr&& other) noexcept;
  ~SharedPtr() noexcept;

  // The operators aren't noexcept because they can throw if the
  // destructor of T throws.
  SharedPtr& operator=(const SharedPtr& other);
  SharedPtr& operator=(SharedPtr&& other);

  T& operator*() const noexcept { return *ptr; };
  T* operator->() const noexcept { return ptr; };
  explicit operator bool() const noexcept { return ptr != nullptr; }

  // reset must be noexcept, but it can throw if the destructor of T throws.
  void reset(T* ptr = nullptr);
  T* get() const noexcept { return ptr; }
  size_t use_count() const noexcept;

  friend class WeakPtr<T>;
  template <typename U, typename... Args>
  friend SharedPtr<U> makeShared(Args&&... args);

 private:
  // It isn't incrementing the shared count,
  // use ControlBlockBase::add_shared() to increment the shared count when
  // creating a SharedPtr from a control block.
  explicit SharedPtr(ControlBlockBase<T>* control_block);

  ControlBlockBase<T>* control_block = nullptr;
  T* ptr = nullptr;
};

template <typename T, typename... Args>
SharedPtr<T> makeShared(Args&&... args) {
  auto control_block =
      new ControlBlockWithTInplace<T>(std::forward<Args>(args)...);
  control_block->add_shared();
  control_block->add_weak();  // Add the implicit weak reference held by
                              // shared ownership. This link helps us keep
                              // control block, preventing double deletions
                              // during cycle dependency.
  SharedPtr<T> ptr(control_block);
  return ptr;
}
}  // namespace ef::smart_pointers

#include "SharedPtr.tpp"
