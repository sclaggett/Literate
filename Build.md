# Build

## Building

The `Build.sh` script makes running CMake slightly easier.

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
