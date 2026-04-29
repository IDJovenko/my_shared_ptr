#include <gtest/gtest.h>

#include <atomic>
#include <cstdlib>
#include <new>
#include <stdexcept>

#include "SharedPtr.hpp"

// Счётчик вызовов деструктора для проверки, что объекты не удаляются при
// выбрасывании исключения в конструкторе.
namespace {
std::atomic<int> g_exc_alloc_count{0};
}  // namespace

namespace ef::smart_pointers {

namespace {

int g_throw_destructor_calls = 0;

struct ThrowOnConstruct {
  ThrowOnConstruct() { throw std::runtime_error("constructor failed"); }
  ~ThrowOnConstruct() { ++g_throw_destructor_calls; }
};

struct ThrowOnConstructWithArg {
  explicit ThrowOnConstructWithArg(bool do_throw) {
    if (do_throw) throw std::runtime_error("constructor failed");
  }
};

}  // namespace

TEST(MakeSharedExceptionTest, ExceptionInConstructorPropagates) {
  EXPECT_THROW(makeShared<ThrowOnConstruct>(), std::runtime_error);
}

TEST(MakeSharedExceptionTest, ExceptionInConstructorDoesNotLeakDestructor) {
  // Объект не был сконструирован → деструктор не должен вызываться
  g_throw_destructor_calls = 0;
  EXPECT_THROW(makeShared<ThrowOnConstruct>(), std::runtime_error);
  EXPECT_EQ(g_throw_destructor_calls, 0);
}

TEST(MakeSharedExceptionTest, SuccessfulConstructionAfterFailedOneWorks) {
  EXPECT_THROW(makeShared<ThrowOnConstructWithArg>(true), std::runtime_error);

  auto p = makeShared<ThrowOnConstructWithArg>(false);
  EXPECT_NE(p.get(), nullptr);
  EXPECT_EQ(p.use_count(), 1U);
}

}  // namespace ef::smart_pointers