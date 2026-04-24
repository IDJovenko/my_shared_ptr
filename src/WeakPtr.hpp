#pragma once
#include <cstddef>

#include "ControlBlock.hpp"
#include "SharedPtr.hpp"

namespace ef::smart_pointers {

template <typename T>
class WeakPtr {
 public:
  WeakPtr();
  WeakPtr(const SharedPtr<T>& shared);
  WeakPtr(const WeakPtr& other);
  WeakPtr(WeakPtr&& other);
  ~WeakPtr();

  WeakPtr& operator=(const SharedPtr<T>& shared);
  WeakPtr& operator=(const WeakPtr& other);
  WeakPtr& operator=(WeakPtr&& other);

  SharedPtr<T> lock() const;
  bool expired() const;
  size_t use_count() const;

 private:
  ControlBlockBase<T>* control_block;
};
}  // namespace ef::smart_pointers

#include "WeakPtr.tpp"
