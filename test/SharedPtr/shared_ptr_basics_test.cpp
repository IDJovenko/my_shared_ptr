#include <gtest/gtest.h>

#include "SharedPtr.hpp"
#include "test_helpers.hpp"

namespace ef::smart_pointers {

TEST(SharedPtrBasicsTest, DefaultConstructorCreatesEmptyPointer) {
  SharedPtr<int> ptr;

  EXPECT_EQ(ptr.use_count(), 0U);
  EXPECT_EQ(ptr.get(), nullptr);
  EXPECT_FALSE(static_cast<bool>(ptr));
}

TEST(SharedPtrBasicsTest, ConstructorFromRawPointerOwnsObject) {
  SharedPtr<int> ptr(new int(42));

  EXPECT_EQ(ptr.use_count(), 1U);
  ASSERT_NE(ptr.get(), nullptr);
  EXPECT_EQ(*ptr, 42);
}

TEST(SharedPtrBasicsTest, DereferenceOperatorReturnsReference) {
  SharedPtr<int> ptr(new int(7));
  int& ref = *ptr;

  ref = 11;
  EXPECT_EQ(*ptr, 11);
}

TEST(SharedPtrBasicsTest, ArrowOperatorProvidesMemberAccess) {
  SharedPtr<IntBox> ptr(new IntBox(3));

  ptr->value = 9;
  EXPECT_EQ(ptr->value, 9);
}

TEST(SharedPtrBasicsTest, GetReturnsSamePointerAsStored) {
  auto* raw = new int(55);
  SharedPtr<int> ptr(raw);

  EXPECT_EQ(ptr.get(), raw);
}

TEST(SharedPtrBasicsTest, GetFromNullptrSharedPtrReturnsNullptr) {
  SharedPtr<int> ptr;

  EXPECT_EQ(ptr.get(), nullptr);
}

TEST(SharedPtrBasicsTest, BoolOperatorForNonEmptyPointerIsTrue) {
  SharedPtr<int> ptr(new int(1));

  EXPECT_TRUE(static_cast<bool>(ptr));
}

TEST(SharedPtrBasicsTest, BoolOperatorForEmptyPointerIsFalse) {
  SharedPtr<int> ptr;

  EXPECT_FALSE(static_cast<bool>(ptr));
}

}  // namespace ef::smart_pointers
