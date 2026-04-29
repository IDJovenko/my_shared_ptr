#include <gtest/gtest.h>

#include "SharedPtr.hpp"
#include "test_helpers.hpp"

namespace ef::smart_pointers {

TEST(SharedPtrCopyTest, CopyConstructorSharesOwnership) {
  SharedPtr<int> p1(new int(10));
  SharedPtr<int> p2(p1);

  EXPECT_EQ(p1.get(), p2.get());
  EXPECT_EQ(p1.use_count(), 2U);
  EXPECT_EQ(p2.use_count(), 2U);
}

TEST(SharedPtrCopyTest, CopyAssignmentSharesOwnershipAndReleasesOldResource) {
  DestructorCounter::destructor_calls = 0;

  SharedPtr<DestructorCounter> source(new DestructorCounter(1));
  SharedPtr<DestructorCounter> target(new DestructorCounter(2));

  target = source;

  EXPECT_EQ(source.get(), target.get());
  EXPECT_EQ(source.use_count(), 2U);
  EXPECT_EQ(target.use_count(), 2U);
  EXPECT_EQ(DestructorCounter::destructor_calls, 1);
}

TEST(SharedPtrCopyTest, SelfCopyAssignmentKeepsStateStable) {
  DestructorCounter::destructor_calls = 0;

  SharedPtr<DestructorCounter> ptr(new DestructorCounter(5));
  const auto before_count = ptr.use_count();
  auto* before_raw = ptr.get();

  ptr = ptr;

  EXPECT_EQ(ptr.use_count(), before_count);
  EXPECT_EQ(ptr.get(), before_raw);
  EXPECT_EQ(DestructorCounter::destructor_calls, 0);
}

}  // namespace ef::smart_pointers
