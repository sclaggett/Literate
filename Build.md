# Build
@title TEMP
@s TEMP

## Building

**TODO** Use `Build.sh` to build both applications for the Raspberry Pi and copy them to the device.

@file Build.sh +x
```sh
#!/bin/sh
mkdir -p build
cd build
cmake ..
make
```

## CMake

@file CMakeLists.txt
```
cmake_minimum_required (VERSION 2.6)

add_definitions(-Wall -std=c++11)

project (lit)

add_executable(lit
  Block.cpp
  CodeBlock.cpp
  FileBlock.cpp
  Main.cpp
  Parser.cpp
  Tangler.cpp)
```
