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
