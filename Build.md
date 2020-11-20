# Build
@title TEMP
@s TEMP

## Building

**TODO** Use `Build.sh` to build both applications for the Raspberry Pi and copy them to the device.

@file source/Build.sh +x
```sh
#!/bin/sh
mkdir -p build
cd build
cmake ..
make
```

## CMake

@file source/CMakeLists.txt
```
cmake_minimum_required (VERSION 2.6)

add_definitions(-Wall -std=c++11)

project (lit)

add_executable(lit
  source/Block.cpp
  source/CodeBlock.cpp
  source/FileBlock.cpp
  source/Main.cpp
  source/Parser.cpp
  source/Tangler.cpp)
```
