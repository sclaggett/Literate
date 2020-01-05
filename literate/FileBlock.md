# FileBlock
@title TEMP
@s TEMP

FileBlock class (public Block):
- Variables:
  + bool executable
- Functions:
  + virtual parseHeader()

@file source/FileBlock.h
```cpp
#pragma once

#include "Block.h"

class FileBlock : public Block
{
public:
  FileBlock();
  virtual ~FileBlock();
};
```

@file source/FileBlock.cpp
```cpp
#include "FileBlock.h"

FileBlock::FileBlock()
{
}

FileBlock::~FileBlock()
{
}
```
