#include <gtest/gtest.h>

#include "../../src/SharedPtr.hpp"
#include "../../src/WeakPtr.hpp"
#include "../SharedPtr/test_helpers.hpp"

namespace ef::smart_pointers {

TEST(WeakPtrBasicsTest, DefaultConstructorIsExpired) {
  WeakPtr<int> wp;

  EXPECT_TRUE(wp.expired());
  EXPECT_EQ(wp.use_count(), 0U);
}

TEST(WeakPtrBasicsTest, ConstructFromSharedPtrHasSameUseCount) {
  SharedPtr<int> sp(new int(1));
  WeakPtr<int> wp(sp);

  EXPECT_EQ(wp.use_count(), sp.use_count());
  EXPECT_EQ(wp.use_count(), 1U);
}

TEST(WeakPtrBasicsTest, ConstructFromSharedPtrIsNotExpired) {
  SharedPtr<int> sp(new int(5));
  WeakPtr<int> wp(sp);

  EXPECT_FALSE(wp.expired());
}

TEST(WeakPtrBasicsTest, ConstructFromSharedPtrUseCountMatchesMultipleOwners) {
  SharedPtr<int> sp1(new int(2));
  SharedPtr<int> sp2(sp1);
  WeakPtr<int> wp(sp1);

  EXPECT_EQ(wp.use_count(), 2U);
  EXPECT_EQ(wp.use_count(), sp1.use_count());
}

}  // namespace ef::smart_pointers
