CXX      := g++
CXXFLAGS := -Wall -Wextra -Wpedantic -Werror -std=c++20 -I./src -pthread
LDFLAGS  := -lgtest

BIN_DIR  := test/bin

SHARED_PTR_SRCS  := $(wildcard test/SharedPtr/*.cpp)
WEAK_PTR_SRCS    := $(wildcard test/WeakPtr/*.cpp)
MAKE_SHARED_SRCS := $(filter-out test/makeShared/make_shared_allocation_test.cpp, \
                      $(wildcard test/makeShared/*.cpp))

MAIN_SRC := test/main.cpp

TESTS_BIN := $(BIN_DIR)/tests

.PHONY: all test clean

all: test

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(TESTS_BIN): $(MAIN_SRC) $(SHARED_PTR_SRCS) $(WEAK_PTR_SRCS) $(MAKE_SHARED_SRCS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

test: $(TESTS_BIN)
	@$(TESTS_BIN)

clean:
	rm -rf $(BIN_DIR)
