#include <gtest/gtest.h>

#include <utility>

#include "SharedPtr.hpp"
#include "test_helpers.hpp"

namespace ef::smart_pointers {

TEST(SharedPtrMoveTest, MoveConstructorTransfersOwnership) {
  SharedPtr<int> p1(new int(8));

  SharedPtr<int> p2(std::move(p1));

  EXPECT_EQ(p1.get(), nullptr);
  EXPECT_FALSE(static_cast<bool>(p1));
  ASSERT_NE(p2.get(), nullptr);
  EXPECT_EQ(*p2, 8);
  EXPECT_EQ(p2.use_count(), 1U);
}

TEST(SharedPtrMoveTest, MoveAssignmentReleasesOldResourceAndTakesNew) {
  DestructorCounter::destructor_calls = 0;

  SharedPtr<DestructorCounter> p1(new DestructorCounter(1));
  SharedPtr<DestructorCounter> p2(new DestructorCounter(2));
  auto* moved_raw = p1.get();

  p2 = std::move(p1);

  EXPECT_EQ(DestructorCounter::destructor_calls, 1);
  EXPECT_EQ(p1.get(), nullptr);
  EXPECT_FALSE(static_cast<bool>(p1));
  EXPECT_EQ(p2.get(), moved_raw);
  EXPECT_EQ(p2.use_count(), 1U);
}

TEST(SharedPtrMoveTest, SelfMoveAssignmentKeepsObjectValid) {
  DestructorCounter::destructor_calls = 0;

  SharedPtr<DestructorCounter> ptr(new DestructorCounter(10));
  auto* before_raw = ptr.get();
  const auto before_count = ptr.use_count();

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wself-move"
  ptr = std::move(ptr);
#pragma GCC diagnostic pop

  EXPECT_EQ(ptr.get(), before_raw);
  EXPECT_EQ(ptr.use_count(), before_count);
  EXPECT_EQ(DestructorCounter::destructor_calls, 0);
}

}  // namespace ef::smart_pointers
