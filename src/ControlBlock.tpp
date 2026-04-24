#include "ControlBlock.hpp"

namespace ef::smart_pointers {

template <typename T>
inline void ControlBlockBase<T>::release_shared() {
  if (--shared_count_ == 0) {
    destroy_object();
    release_weak();  // Release the implicit weak reference held by shared
                     // ownership. This link helped us keep control block,
                     // preventing double deletions during cycle dependency.
  }
}

template <typename T>
inline void ControlBlockBase<T>::release_weak() {
  if (--weak_count_ == 0 && shared_count_ == 0) {
    destroy_this();
  }
}

}  // namespace ef::smart_pointers
