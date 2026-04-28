#include <gtest/gtest.h>

#include "SharedPtr.hpp"
#include "WeakPtr.hpp"

namespace ef::smart_pointers {

TEST(WeakPtrExpiredTest, NotExpiredWhileSharedPtrIsAlive) {
  SharedPtr<int> sp(new int(5));
  WeakPtr<int> wp(sp);

  EXPECT_FALSE(wp.expired());
}

TEST(WeakPtrExpiredTest, NotExpiredWhileAtLeastOneSharedPtrIsAlive) {
  SharedPtr<int> sp1(new int(5));
  SharedPtr<int> sp2(sp1);
  WeakPtr<int> wp(sp1);

  sp1.reset();

  EXPECT_FALSE(wp.expired());
}

TEST(WeakPtrExpiredTest, BecomesExpiredAfterLastSharedPtrIsDestroyed) {
  WeakPtr<int> wp;
  {
    SharedPtr<int> sp(new int(5));
    wp = sp;
    EXPECT_FALSE(wp.expired());
  }

  EXPECT_TRUE(wp.expired());
}

TEST(WeakPtrExpiredTest, BecomesExpiredAfterAllSharedPtrsReset) {
  SharedPtr<int> sp1(new int(3));
  SharedPtr<int> sp2(sp1);
  WeakPtr<int> wp(sp1);

  sp1.reset();
  EXPECT_FALSE(wp.expired());

  sp2.reset();
  EXPECT_TRUE(wp.expired());
}

}  // namespace ef::smart_pointers
