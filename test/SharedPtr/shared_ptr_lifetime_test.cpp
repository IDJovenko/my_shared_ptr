#include <gtest/gtest.h>

#include "SharedPtr.hpp"
#include "test_helpers.hpp"

namespace ef::smart_pointers {

TEST(SharedPtrLifetimeTest, NoLeakAfterScopeExitDestructorIsCalled) {
  DestructorCounter::destructor_calls = 0;

  {
    SharedPtr<DestructorCounter> ptr(new DestructorCounter(42));
    EXPECT_EQ(ptr.use_count(), 1U);
  }

  EXPECT_EQ(DestructorCounter::destructor_calls, 1);
}

TEST(SharedPtrLifetimeTest, ManagedObjectIsDeletedExactlyOnce) {
  DestructorCounter::destructor_calls = 0;

  {
    SharedPtr<DestructorCounter> p1(new DestructorCounter(1));
    SharedPtr<DestructorCounter> p2 = p1;
    SharedPtr<DestructorCounter> p3 = p2;

    EXPECT_EQ(p1.use_count(), 3U);
    EXPECT_EQ(p2.use_count(), 3U);
    EXPECT_EQ(p3.use_count(), 3U);
  }

  EXPECT_EQ(DestructorCounter::destructor_calls, 1);
}

}  // namespace ef::smart_pointers
