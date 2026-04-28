# my_shared_ptr
Implementation of analogs of std::shared_ptr and std::weak_ptr with automatic memory management via reference counting.

## Build and run tests

```bash
# Configure project and generate build system files
cmake -B build -S .

# Build test executable
cmake --build build

# Run all registered tests
ctest --test-dir build
```