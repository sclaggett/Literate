cmake_minimum_required (VERSION 3.15)

project (lit)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
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
