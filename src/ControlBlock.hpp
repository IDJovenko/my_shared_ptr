#pragma once
#include <cstddef>  // for size_t

namespace ef::smart_pointers {

template <typename T>
class ControlBlockBase {
 public:
  ControlBlockBase() = default;
  ControlBlockBase(const ControlBlockBase&) = delete;
  ControlBlockBase& operator=(const ControlBlockBase&) = delete;
  ControlBlockBase(ControlBlockBase&&) = delete;
  ControlBlockBase& operator=(ControlBlockBase&&) = delete;

  virtual ~ControlBlockBase() noexcept = default;

  void add_shared() noexcept { shared_count_++; }
  void release_shared();
  void add_weak() noexcept { weak_count_++; }
  void release_weak();

  size_t use_count() const noexcept { return shared_count_; }
  virtual T* get() noexcept = 0;

 private:
  virtual void destroy_object() = 0;
  virtual void destroy_this() noexcept = 0;

  size_t shared_count_ = 0;
  size_t weak_count_ = 0;
};

template <typename T>
class ControlBlockWithPtr : public ControlBlockBase<T> {
 public:
  explicit ControlBlockWithPtr(T* ptr) : ptr(ptr) {};
  ~ControlBlockWithPtr() override = default;

  T* get() noexcept override { return ptr; }

 private:
  void destroy_object() override { delete ptr; }
  void destroy_this() noexcept override { delete this; };

  T* ptr;
};

template <typename T>
class ControlBlockWithTInplace : public ControlBlockBase<T> {
 public:
  template <typename... Args>
  explicit ControlBlockWithTInplace(Args&&... args) {
    new (storage) T(std::forward<Args>(args)...);
  }
  ~ControlBlockWithTInplace() override = default;

  T* get() noexcept override { return reinterpret_cast<T*>(storage); }

 private:
  void destroy_object() override { reinterpret_cast<T*>(storage)->~T(); }
  void destroy_this() noexcept override { delete this; };

  alignas(T) char storage[sizeof(T)];
};

}  // namespace ef::smart_pointers

#include "ControlBlock.tpp"
