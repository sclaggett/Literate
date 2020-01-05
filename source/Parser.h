#pragma once

#include <map>
#include <string>
#include "CodeBlock.h"
#include "FileBlock.h"

class Parser
{
public:
  Parser();
  virtual ~Parser();

public:
  bool parse(std::string literateFile);
  std::map<std::string, FileBlock> getFileBlocks();
  std::map<std::string, CodeBlock> getCodeBlocks();

private:
  std::map<std::string, FileBlock> fileBlocks;
  std::map<std::string, CodeBlock> codeBlocks;
};

