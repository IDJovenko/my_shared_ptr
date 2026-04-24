#include <gtest/gtest.h>

#include "../../src/SharedPtr.hpp"

namespace ef::smart_pointers {

namespace {

struct MultiArg {
  MultiArg(int i, double d, bool b) : i(i), d(d), b(b) {}
  int i;
  double d;
  bool b;
};

struct MoveOnlyArg {
  explicit MoveOnlyArg(std::unique_ptr<int> p) : val(*p) {}
  int val;
};

}  // namespace

TEST(MakeSharedArgsTest, SingleIntArgIsForwarded) {
  auto p = makeShared<int>(99);

  EXPECT_EQ(*p, 99);
}

TEST(MakeSharedArgsTest, MultipleArgsAreForwardedCorrectly) {
  auto p = makeShared<MultiArg>(7, 3.14, true);

  EXPECT_EQ(p->i, 7);
  EXPECT_DOUBLE_EQ(p->d, 3.14);
  EXPECT_TRUE(p->b);
}

TEST(MakeSharedArgsTest, MoveOnlyArgIsForwarded) {
  auto p = makeShared<MoveOnlyArg>(std::make_unique<int>(55));

  EXPECT_EQ(p->val, 55);
}

}  // namespace ef::smart_pointers