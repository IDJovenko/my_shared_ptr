#include <gtest/gtest.h>

#include <utility>

#include "../../src/SharedPtr.hpp"
#include "../../src/WeakPtr.hpp"

namespace ef::smart_pointers {

TEST(WeakPtrCopyMoveTest, CopyConstructorDoesNotChangeSharedCount) {
  SharedPtr<int> sp(new int(10));
  WeakPtr<int> wp1(sp);

  WeakPtr<int> wp2(wp1);

  EXPECT_EQ(sp.use_count(), 1U);
  EXPECT_EQ(wp1.use_count(), 1U);
  EXPECT_EQ(wp2.use_count(), 1U);
  EXPECT_FALSE(wp2.expired());
}

TEST(WeakPtrCopyMoveTest, CopyConstructorSharesReferenceToSameObject) {
  SharedPtr<int> sp(new int(7));
  WeakPtr<int> wp1(sp);
  WeakPtr<int> wp2(wp1);

  auto locked1 = wp1.lock();
  auto locked2 = wp2.lock();

  EXPECT_EQ(locked1.get(), locked2.get());
}

TEST(WeakPtrCopyMoveTest, CopyAssignmentDoesNotChangeSharedCount) {
  SharedPtr<int> sp(new int(3));
  WeakPtr<int> wp1(sp);
  WeakPtr<int> wp2;

  wp2 = wp1;

  EXPECT_EQ(sp.use_count(), 1U);
  EXPECT_FALSE(wp2.expired());
}

TEST(WeakPtrCopyMoveTest, SelfCopyAssignmentKeepsStateStable) {
  SharedPtr<int> sp(new int(42));
  WeakPtr<int> wp(sp);

  wp = wp;

  EXPECT_EQ(sp.use_count(), 1U);
  EXPECT_FALSE(wp.expired());
}

TEST(WeakPtrCopyMoveTest, MoveConstructorTransfersObservation) {
  SharedPtr<int> sp(new int(4));
  WeakPtr<int> wp1(sp);

  WeakPtr<int> wp2(std::move(wp1));

  EXPECT_TRUE(wp1.expired());
  EXPECT_FALSE(wp2.expired());
  EXPECT_EQ(sp.use_count(), 1U);
}

TEST(WeakPtrCopyMoveTest, MoveAssignmentTransfersObservation) {
  SharedPtr<int> sp(new int(9));
  WeakPtr<int> wp1(sp);
  WeakPtr<int> wp2;

  wp2 = std::move(wp1);

  EXPECT_TRUE(wp1.expired());
  EXPECT_FALSE(wp2.expired());
  EXPECT_EQ(sp.use_count(), 1U);
}

TEST(WeakPtrCopyMoveTest, SelfMoveAssignmentKeepsStateStable) {
  SharedPtr<int> sp(new int(6));
  WeakPtr<int> wp(sp);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wself-move"
  wp = std::move(wp);
#pragma GCC diagnostic pop

  EXPECT_EQ(sp.use_count(), 1U);
}

}  // namespace ef::smart_pointers
