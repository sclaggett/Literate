# Tangler
@title TEMP
@s TEMP

@file source/Tangler.h
```cpp
#pragma once

#include <map>
#include <string>
#include "CodeBlock.h"
#include "FileBlock.h"

class Tangler
{
public:
  Tangler();
  virtual ~Tangler();

public:
  bool tangle(std::map<std::string, FileBlock> fileBlocks,
    std::map<std::string, CodeBlock> codeBlocks, std::string outputDirectory);
};
```

@file source/Tangler.cpp
```cpp
#include "Tangler.h"
#include <iostream>

using namespace std;

Tangler::Tangler()
{
}

Tangler::~Tangler()
{
}

bool Tangler::tangle(map<string, FileBlock> fileBlocks,
    map<string, CodeBlock> codeBlocks, string outputDirectory)
{
  cout << "Tangle " << outputDirectory << endl;
  return true;
}
```
