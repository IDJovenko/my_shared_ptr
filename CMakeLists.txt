cmake_minimum_required(VERSION 3.25)

project(my_shared_ptr LANGUAGES CXX)

# Enable CTest integration for test discovery and execution.
enable_testing()

find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

# Keep source files explicit to avoid silent reconfiguration issues
# when adding/removing tests (instead of file(GLOB ...)).
add_executable(tests
  test/main.cpp
  test/SharedPtr/shared_ptr_basics_test.cpp
  test/SharedPtr/shared_ptr_copy_test.cpp
  test/SharedPtr/shared_ptr_lifetime_test.cpp
  test/SharedPtr/shared_ptr_move_test.cpp
  test/SharedPtr/shared_ptr_reset_test.cpp
  test/WeakPtr/weakptr_basics_test.cpp
  test/WeakPtr/weakptr_copy_move_test.cpp
  test/WeakPtr/weakptr_cycle_test.cpp
  test/WeakPtr/weakptr_expired_test.cpp
  test/WeakPtr/weakptr_interaction_test.cpp
  test/WeakPtr/weakptr_lock_test.cpp
  test/makeShared/make_shared_args_test.cpp
  test/makeShared/make_shared_basics_test.cpp
  test/makeShared/make_shared_exception_test.cpp
  test/makeShared/make_shared_reset_test.cpp
  test/makeShared/make_shared_weakptr_test.cpp
)

# Request C++20 in a target-scoped way.
target_compile_features(tests PRIVATE cxx_std_20)

# Keep warnings and strict mode scoped to this target only.
target_compile_options(tests PRIVATE
  -Wall
  -Wextra
  -Wpedantic
  -Werror
)

# Test-only include paths (do not propagate to unrelated targets).
target_include_directories(tests PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/test/SharedPtr
  ${CMAKE_CURRENT_SOURCE_DIR}/test/WeakPtr
  ${CMAKE_CURRENT_SOURCE_DIR}/test/makeShared
)

# Link GTest and portable threading support via imported targets.
target_link_libraries(tests PRIVATE
  GTest::gtest
  Threads::Threads
)

# Match Makefile-style output layout for the test executable.
set_target_properties(tests PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test/bin
)

add_test(NAME tests COMMAND $<TARGET_FILE:tests>)
