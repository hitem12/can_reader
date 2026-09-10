# Can_reader

Build:
```
conan install . --build=missing -s build_type=Debug
cmake --build ./cmake-build-debug
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