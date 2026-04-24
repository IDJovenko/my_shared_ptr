#include <gtest/gtest.h>

#include "../../src/SharedPtr.hpp"
#include "../../src/WeakPtr.hpp"

namespace ef::smart_pointers {

namespace {

struct Node {
  static int destructor_calls;

  explicit Node(int id_in) : id(id_in) {}
  ~Node() { ++destructor_calls; }

  int id = 0;
  SharedPtr<Node> next;
  WeakPtr<Node> prev;
};

int Node::destructor_calls = 0;

}  // namespace

TEST(WeakPtrCycleTest, NodesWithWeakPtrPrevAreDestroyedCorrectly) {
  Node::destructor_calls = 0;

  {
    SharedPtr<Node> n1(new Node(1));
    SharedPtr<Node> n2(new Node(2));

    n1->next = n2;
    ASSERT_EQ(n1->next.get(), n2.get());
    ASSERT_EQ(n2.use_count(), 2U);
    n2->prev = n1;
    ASSERT_EQ(n1.use_count(), 1U);
  }

  EXPECT_EQ(Node::destructor_calls, 2);
}

TEST(WeakPtrCycleTest, ThreeNodeCycleIsDestroyedCorrectly) {
  Node::destructor_calls = 0;

  {
    SharedPtr<Node> n1(new Node(1));
    SharedPtr<Node> n2(new Node(2));
    SharedPtr<Node> n3(new Node(3));

    n1->next = n2;
    ASSERT_EQ(n1->next.get(), n2.get());
    ASSERT_EQ(n2.use_count(), 2U);
    n2->prev = n1;
    ASSERT_EQ(n1.use_count(), 1U);
    n2->next = n3;
    ASSERT_EQ(n2->next.get(), n3.get());
    ASSERT_EQ(n3.use_count(), 2U);
    n3->prev = n2;
    ASSERT_EQ(n2.use_count(), 2U);
  }

  EXPECT_EQ(Node::destructor_calls, 3);
}

TEST(WeakPtrCycleTest, WeakPrevIsExpiredAfterNodesAreDestroyed) {
  Node::destructor_calls = 0;

  WeakPtr<Node> captured_prev;
  {
    SharedPtr<Node> n1(new Node(1));
    SharedPtr<Node> n2(new Node(2));

    n1->next = n2;
    n2->prev = n1;
    captured_prev = n2->prev;
  }

  EXPECT_EQ(Node::destructor_calls, 2);
  EXPECT_TRUE(captured_prev.expired());
}

}  // namespace ef::smart_pointers
