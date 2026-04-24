#include <gtest/gtest.h>

#include "../../src/SharedPtr.hpp"
#include "../SharedPtr/test_helpers.hpp"

namespace ef::smart_pointers {

TEST(MakeSharedResetTest, ReassignWithMakeSharedDestroysOldObject) {
  DestructorCounter::destructor_calls = 0;

  auto p = makeShared<DestructorCounter>(1);
  p = makeShared<DestructorCounter>(2);

  EXPECT_EQ(DestructorCounter::destructor_calls, 1);
  EXPECT_EQ(p->value, 2);
  EXPECT_EQ(p.use_count(), 1U);
}

TEST(MakeSharedResetTest, ResetBeforeMakeSharedDestroysOldObject) {
  DestructorCounter::destructor_calls = 0;

  auto p = makeShared<DestructorCounter>(10);
  p.reset();

  EXPECT_EQ(DestructorCounter::destructor_calls, 1);
  EXPECT_EQ(p.get(), nullptr);

  p = makeShared<DestructorCounter>(20);

  EXPECT_NE(p.get(), nullptr);
  EXPECT_EQ(p->value, 20);
  EXPECT_EQ(p.use_count(), 1U);
}

TEST(MakeSharedResetTest, SharedOwnerUnchangedAfterOtherReassigns) {
  DestructorCounter::destructor_calls = 0;

  auto p1 = makeShared<DestructorCounter>(5);
  auto p2 = p1;

  p1 = makeShared<DestructorCounter>(6);

  EXPECT_EQ(DestructorCounter::destructor_calls, 0);
  EXPECT_EQ(p2.use_count(), 1U);
  EXPECT_EQ(p2->value, 5);
}

}  // namespace ef::smart_pointers