#pragma once

#include "Block.h"

class FileBlock : public Block
{
public:
  FileBlock(std::string sourceFile, uint32_t sourceLine);
  virtual ~FileBlock();

public:
  static bool checkStart(std::string line1, std::string line2);
  bool parseHeader(std::string line);
  bool getExecutable();

private:
  bool executable;
};
