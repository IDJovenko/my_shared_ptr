#pragma once

struct IntBox {
  explicit IntBox(int value_in) : value(value_in) {}

  int value;
};

struct DestructorCounter {
  static int destructor_calls;

  explicit DestructorCounter(int value_in = 0) : value(value_in) {}
  ~DestructorCounter() { ++destructor_calls; }

  int value;
};

inline int DestructorCounter::destructor_calls = 0;
