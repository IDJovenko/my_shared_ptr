#include <gtest/gtest.h>

#include "../../src/SharedPtr.hpp"
#include "../SharedPtr/test_helpers.hpp"

namespace ef::smart_pointers {

TEST(MakeSharedBasicsTest, CreatesValidPointerWithUseCountOne) {
  auto p = makeShared<int>(5);

  EXPECT_NE(p.get(), nullptr);
  EXPECT_EQ(p.use_count(), 1U);
}

TEST(MakeSharedBasicsTest, StoredValueIsCorrect) {
  auto p = makeShared<int>(42);

  EXPECT_EQ(*p, 42);
}

TEST(MakeSharedBasicsTest, BoolOperatorIsTrue) {
  auto p = makeShared<int>(0);

  EXPECT_TRUE(static_cast<bool>(p));
}

TEST(MakeSharedBasicsTest, DefaultConstructedTypeIsCreated) {
  auto p = makeShared<int>();

  EXPECT_NE(p.get(), nullptr);
  EXPECT_EQ(p.use_count(), 1U);
}

}  // namespace ef::smart_pointers