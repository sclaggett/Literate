# CodeBlock
@title TEMP
@s TEMP

CodeBlock class (public Block):
- Variables:
  + bool append
- Functions:
  + virtual parseHeader()

@file source/CodeBlock.h
```cpp
#pragma once

#include "Block.h"

class CodeBlock : public Block
{
public:
  CodeBlock();
  virtual ~CodeBlock();
};
```

@file source/CodeBlock.cpp
```cpp
#include "CodeBlock.h"

CodeBlock::CodeBlock()
{
}

CodeBlock::~CodeBlock()
{
}
```
