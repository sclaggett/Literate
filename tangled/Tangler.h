#pragma once

#include <map>
#include <string>
#include "CodeBlock.h"
#include "FileBlock.h"

class Tangler
{
public:
  bool tangle(std::map<std::string, FileBlock*> fileBlocks,
    std::map<std::string, CodeBlock*> codeBlocks, std::string outputDirectory);

private:
  // TODO: Keep the output as an array of lines
  bool tangleBlock(Block* block, std::map<std::string, CodeBlock*> codeBlocks,
    std::vector<std::string>& output);

  std::map<std::string, std::vector<std::string>> tangledBlocks;
};
