#pragma once

#include "Block.h"

class CodeBlock : public Block
{
public:
  CodeBlock(std::string sourceFile, uint32_t sourceLine);
  virtual ~CodeBlock();

public:
  static bool checkStart(std::string line1, std::string line2);
  bool parseHeader(std::string line);
  bool getAppend();

private:
  bool append;
};

