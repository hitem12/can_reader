# Can_reader

CMake custom parameters:
- can_reader_CAN_IFACE - set the interface for system daemon 
Build with:
```
conan install . --build=missing -s build_type=Release
cmake -S . -B cmake-build-release -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=cmake-build-release/conan_toolchain.cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -Dcan_reader_CAN_IFACE=vcan0
cmake --build ./cmake-build-release/ 
```

Install: 
```
cmake install 
```

### Requirement:
#### libraries 
 - cli11
 - spdlog
For tests
 - gtest