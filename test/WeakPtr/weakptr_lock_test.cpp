#include <gtest/gtest.h>

#include "SharedPtr.hpp"
#include "WeakPtr.hpp"

namespace ef::smart_pointers {

TEST(WeakPtrLockTest, LockOnValidObjectReturnsNonEmptySharedPtr) {
  SharedPtr<int> sp(new int(21));
  WeakPtr<int> wp(sp);

  SharedPtr<int> locked = wp.lock();

  EXPECT_NE(locked.get(), nullptr);
  EXPECT_EQ(*locked, 21);
}

TEST(WeakPtrLockTest, LockOnValidObjectIncreasesUseCount) {
  SharedPtr<int> sp(new int(1));
  WeakPtr<int> wp(sp);

  SharedPtr<int> locked = wp.lock();

  EXPECT_EQ(sp.use_count(), 2U);
  EXPECT_EQ(locked.use_count(), 2U);
}

TEST(WeakPtrLockTest, LockReturnsPointerToSameObject) {
  SharedPtr<int> sp(new int(99));
  WeakPtr<int> wp(sp);

  SharedPtr<int> locked = wp.lock();

  EXPECT_EQ(locked.get(), sp.get());
}

TEST(WeakPtrLockTest, LockAfterSharedPtrExpiredReturnsEmpty) {
  WeakPtr<int> wp;
  {
    SharedPtr<int> sp(new int(8));
    wp = sp;
  }

  SharedPtr<int> locked = wp.lock();

  EXPECT_EQ(locked.get(), nullptr);
  EXPECT_FALSE(static_cast<bool>(locked));
}

TEST(WeakPtrLockTest, LockAfterSharedPtrExpiredHasZeroUseCount) {
  WeakPtr<int> wp;
  {
    SharedPtr<int> sp(new int(8));
    wp = sp;
  }

  SharedPtr<int> locked = wp.lock();

  EXPECT_EQ(locked.use_count(), 0U);
}

}  // namespace ef::smart_pointers
