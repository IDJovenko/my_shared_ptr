#include "WeakPtr.hpp"

namespace ef::smart_pointers {
template <typename T>
inline WeakPtr<T>::WeakPtr() : control_block(nullptr) {}

template <typename T>
inline WeakPtr<T>::WeakPtr(const SharedPtr<T>& shared)
    : control_block(shared.control_block) {
  if (control_block) {
    control_block->add_weak();
  }
}

template <typename T>
inline WeakPtr<T>::WeakPtr(const WeakPtr& other)
    : control_block(other.control_block) {
  if (control_block) {
    control_block->add_weak();
  }
}
template <typename T>
inline WeakPtr<T>::WeakPtr(WeakPtr&& other) {
  control_block = other.control_block;
  other.control_block = nullptr;
}

template <typename T>
inline WeakPtr<T>::~WeakPtr() {
  if (control_block) {
    control_block->release_weak();
  }
}

template <typename T>
inline WeakPtr<T>& WeakPtr<T>::operator=(const SharedPtr<T>& shared) {
  if (control_block) {
    control_block->release_weak();
  }
  control_block = shared.control_block;
  if (control_block) {
    control_block->add_weak();
  }
  return *this;
}
template <typename T>
inline WeakPtr<T>& WeakPtr<T>::operator=(const WeakPtr& other) {
  if (this != &other) {
    if (control_block) {
      control_block->release_weak();
    }
    control_block = other.control_block;
    if (control_block) {
      control_block->add_weak();
    }
  }
  return *this;
}
template <typename T>
inline WeakPtr<T>& WeakPtr<T>::operator=(WeakPtr&& other) {
  if (this != &other) {
    if (control_block) {
      control_block->release_weak();
    }
    control_block = other.control_block;
    other.control_block = nullptr;
  }
  return *this;
}

template <typename T>
inline SharedPtr<T> WeakPtr<T>::lock() const {
  if (expired()) {
    return SharedPtr<T>();
  }
  control_block->add_shared();
  return SharedPtr<T>(control_block);  // use private ctor
}

template <typename T>
inline bool WeakPtr<T>::expired() const {
  return !control_block || control_block->use_count() == 0;
}

template <typename T>
inline size_t WeakPtr<T>::use_count() const {
  return control_block ? control_block->use_count() : 0;
};

}  // namespace ef::smart_pointers
