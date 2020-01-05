# Block
@title TEMP
@s TEMP

Block class:
- Variables:
  + string name
  + vector<string> lines
- Functions:
  + parse()
  + virtual parseHeader() = 0

@file source/Block.h
```cpp
#pragma once

class Block
{
public:
  Block();
  virtual ~Block();
};
```

@file source/Block.cpp
```cpp
#include "Block.h"

Block::Block()
{
}

Block::~Block()
{
}
```
