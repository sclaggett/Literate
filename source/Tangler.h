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
  bool tangle(std::map<std::string, FileBlock*> fileBlocks,
    std::map<std::string, CodeBlock*> codeBlocks, std::string outputDirectory);
};

