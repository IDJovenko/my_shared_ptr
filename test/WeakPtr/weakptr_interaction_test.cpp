#include <gtest/gtest.h>

#include "../../src/SharedPtr.hpp"
#include "../../src/WeakPtr.hpp"
#include "../SharedPtr/test_helpers.hpp"

namespace ef::smart_pointers {

TEST(WeakPtrInteractionTest, ObjectIsDestroyedEvenWithLiveWeakPtr) {
  DestructorCounter::destructor_calls = 0;

  WeakPtr<DestructorCounter> wp;
  {
    SharedPtr<DestructorCounter> sp(new DestructorCounter(1));
    wp = sp;
    EXPECT_EQ(DestructorCounter::destructor_calls, 0);
  }

  EXPECT_EQ(DestructorCounter::destructor_calls, 1);
  EXPECT_TRUE(wp.expired());
}

TEST(WeakPtrInteractionTest, MultipleWeakPtrsDoNotExtendLifetime) {
  DestructorCounter::destructor_calls = 0;

  WeakPtr<DestructorCounter> wp1;
  WeakPtr<DestructorCounter> wp2;
  WeakPtr<DestructorCounter> wp3;
  {
    SharedPtr<DestructorCounter> sp(new DestructorCounter(2));
    wp1 = sp;
    wp2 = sp;
    wp3 = sp;
  }

  EXPECT_EQ(DestructorCounter::destructor_calls, 1);
  EXPECT_TRUE(wp1.expired());
  EXPECT_TRUE(wp2.expired());
  EXPECT_TRUE(wp3.expired());
}

TEST(WeakPtrInteractionTest, CreatingWeakPtrDoesNotIncreaseUseCount) {
  SharedPtr<int> sp(new int(5));
  const auto count_before = sp.use_count();

  WeakPtr<int> wp(sp);

  EXPECT_EQ(sp.use_count(), count_before);
  EXPECT_EQ(sp.use_count(), 1U);
}

TEST(WeakPtrInteractionTest, MultipleWeakPtrsDoNotIncreaseUseCount) {
  SharedPtr<int> sp(new int(5));

  WeakPtr<int> wp1(sp);
  WeakPtr<int> wp2(sp);
  WeakPtr<int> wp3(wp1);

  EXPECT_EQ(sp.use_count(), 1U);
}

}  // namespace ef::smart_pointers
