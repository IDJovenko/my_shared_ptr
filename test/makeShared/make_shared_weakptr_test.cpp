#include <gtest/gtest.h>

#include "../../src/SharedPtr.hpp"
#include "../../src/WeakPtr.hpp"
#include "../SharedPtr/test_helpers.hpp"

namespace ef::smart_pointers {

TEST(MakeSharedWeakPtrTest, WeakPtrCanObserveMakeSharedObject) {
  auto sp = makeShared<int>(10);
  WeakPtr<int> wp(sp);

  EXPECT_FALSE(wp.expired());
  EXPECT_EQ(wp.use_count(), 1U);
}

TEST(MakeSharedWeakPtrTest, LockOnMakeSharedObjectReturnsValidSharedPtr) {
  auto sp = makeShared<int>(77);
  WeakPtr<int> wp(sp);

  auto locked = wp.lock();

  ASSERT_NE(locked.get(), nullptr);
  EXPECT_EQ(*locked, 77);
  EXPECT_EQ(sp.use_count(), 2U);
}

TEST(MakeSharedWeakPtrTest, WeakPtrExpiresAfterMakeSharedSharedPtrDestroyed) {
  WeakPtr<int> wp;
  {
    auto sp = makeShared<int>(3);
    wp = sp;
    EXPECT_FALSE(wp.expired());
  }

  EXPECT_TRUE(wp.expired());
  auto locked = wp.lock();
  EXPECT_EQ(locked.get(), nullptr);
}

TEST(MakeSharedWeakPtrTest, MakeSharedDoesNotInfluenceWeakUseCount) {
  auto sp = makeShared<int>(5);
  WeakPtr<int> wp1(sp);
  WeakPtr<int> wp2(sp);

  EXPECT_EQ(sp.use_count(), 1U);
}

}  // namespace ef::smart_pointers