#include <gtest/gtest.h>

#include "SharedPtr.hpp"
#include "test_helpers.hpp"

namespace ef::smart_pointers {

TEST(SharedPtrResetTest, ResetClearsPointerAndCount) {
  SharedPtr<int> ptr(new int(12));

  ptr.reset();

  EXPECT_EQ(ptr.get(), nullptr);
  EXPECT_EQ(ptr.use_count(), 0U);
  EXPECT_FALSE(static_cast<bool>(ptr));
}

TEST(SharedPtrResetTest, ResetOnSharedObjectLeavesOtherOwnerValid) {
  SharedPtr<int> p1(new int(99));
  SharedPtr<int> p2 = p1;

  p1.reset();

  EXPECT_EQ(p1.get(), nullptr);
  EXPECT_EQ(p1.use_count(), 0U);
  ASSERT_NE(p2.get(), nullptr);
  EXPECT_EQ(*p2, 99);
  EXPECT_EQ(p2.use_count(), 1U);
}

TEST(SharedPtrResetTest, ResetWithPointerReplacesManagedObject) {
  DestructorCounter::destructor_calls = 0;

  SharedPtr<DestructorCounter> ptr(new DestructorCounter(1));
  auto* old_raw = ptr.get();

  ptr.reset(new DestructorCounter(2));

  EXPECT_NE(ptr.get(), old_raw);
  ASSERT_NE(ptr.get(), nullptr);
  EXPECT_EQ(ptr->value, 2);
  EXPECT_EQ(ptr.use_count(), 1U);
  EXPECT_EQ(DestructorCounter::destructor_calls, 1);
}

TEST(SharedPtrResetTest,
     ResetWithPointerOnSharedObjectKeepsOtherOwnerOnOldObject) {
  DestructorCounter::destructor_calls = 0;

  SharedPtr<DestructorCounter> p1(new DestructorCounter(10));
  SharedPtr<DestructorCounter> p2 = p1;
  auto* old_raw = p1.get();

  p1.reset(new DestructorCounter(20));

  EXPECT_NE(p1.get(), old_raw);
  EXPECT_EQ(p1.use_count(), 1U);
  ASSERT_NE(p1.get(), nullptr);
  EXPECT_EQ(p1->value, 20);

  EXPECT_EQ(p2.get(), old_raw);
  EXPECT_EQ(p2.use_count(), 1U);
  ASSERT_NE(p2.get(), nullptr);
  EXPECT_EQ(p2->value, 10);
  EXPECT_EQ(DestructorCounter::destructor_calls, 0);

  p2.reset();
  EXPECT_EQ(DestructorCounter::destructor_calls, 1);

  p1.reset();
  EXPECT_EQ(DestructorCounter::destructor_calls, 2);
}

}  // namespace ef::smart_pointers
