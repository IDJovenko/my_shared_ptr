#include "SharedPtr.hpp"

namespace ef::smart_pointers {

template <typename T>
inline SharedPtr<T>::SharedPtr(T* ptr) {
  if (ptr) {
    this->ptr = ptr;
    try {
      control_block = new ControlBlockWithPtr<T>(ptr);
    } catch (...) {
      delete ptr;
      throw;
    }
    control_block->add_shared();
    control_block->add_weak();  // Add implicit weak reference for shared
                                // ownership. This link helps us keep control
                                // block, preventing double deletions during
                                // cycle dependency.
  }
}

template <typename T>
inline SharedPtr<T>::SharedPtr(const SharedPtr& other) noexcept
    : control_block(other.control_block), ptr(other.ptr) {
  if (control_block) {
    control_block->add_shared();
  }
}

template <typename T>
inline SharedPtr<T>::SharedPtr(SharedPtr&& other) noexcept
    : control_block(other.control_block), ptr(other.ptr) {
  other.control_block = nullptr;
  other.ptr = nullptr;
}
template <typename T>
inline SharedPtr<T>::SharedPtr(ControlBlockBase<T>* control_block)
    : control_block(control_block) {
  if (control_block) {
    ptr = control_block->get();
  }
}

template <typename T>
inline SharedPtr<T>::~SharedPtr() noexcept {
  if (control_block) {
    control_block->release_shared();
  }
}

template <typename T>
inline SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr& other) {
  if (this != &other) {
    if (control_block) {
      control_block->release_shared();
    }
    control_block = other.control_block;
    ptr = other.ptr;
    if (control_block) {
      control_block->add_shared();
    }
  }
  // auto tmp = SharedPtr<T>(other);
  // std::swap(tmp, *this);
  return *this;
}

template <typename T>
inline SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr&& other) {
  if (this != &other) {
    if (control_block) {
      control_block->release_shared();
    }
    control_block = other.control_block;
    ptr = other.ptr;
    other.control_block = nullptr;
    other.ptr = nullptr;
  }
  // auto tmp = SharedPtr<T>(std::move(other));
  // std::swap(tmp, *this);
  return *this;
}

template <typename T>
inline void SharedPtr<T>::reset(T* ptr) {
  auto tmp = SharedPtr<T>(ptr);
  std::swap(tmp, *this);
}

template <typename T>
inline size_t SharedPtr<T>::use_count() const noexcept {
  return control_block ? control_block->use_count() : 0;
}
}  // namespace ef::smart_pointers
