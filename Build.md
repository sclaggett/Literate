# Build

## Building

The `Build.sh` script creates a sibling directory named `build`, switches to it, and runs CMake. It stops there if building on Windows so Visual Studio can be used.

@file Build.sh +x
```sh
#!/bin/sh
mkdir -p ../build
cd ../build
cmake ../tangled

unameOut="$(uname -s)"
if [[ $unameOut == MINGW* ]]
then
  echo ""
  echo "Open solution in Visual Studio and build from there."
  echo ""
  exit
fi

make
```

## CMake

The configuration file for CMake sets compiler-specific options and enabled grouping for a cleaner Visual Studio project.

@file CMakeLists.txt
```
cmake_minimum_required (VERSION 3.15)

project (lit)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  add_definitions(-Wall -std=c++11)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  add_definitions(-Wall -std=c++11)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  add_compile_options(/W3 /WX)
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
else()
  message(FATAL_ERROR "Unsupported compiler detected: ${CMAKE_CXX_COMPILER_ID}.")
endif()

add_executable(lit
  Block.cpp
  CodeBlock.cpp
  FileBlock.cpp
  Main.cpp
  Parser.cpp
  Tangler.cpp)
```
