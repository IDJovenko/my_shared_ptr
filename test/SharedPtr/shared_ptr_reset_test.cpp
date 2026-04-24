#include <gtest/gtest.h>

#include "../../src/SharedPtr.hpp"

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

}  // namespace ef::smart_pointers
