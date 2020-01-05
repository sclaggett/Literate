# Parser
@title TEMP
@s TEMP

@file source/Parser.h
```cpp
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
```

@file source/Parser.cpp
```cpp
#include <iostream>
#include "Parser.h"

using namespace std;

Parser::Parser()
{
}

Parser::~Parser()
{
}

bool Parser::parse(std::string literateFile)
{
  cout << "Parse " << literateFile << endl;
  return true;
}

map<string, FileBlock> Parser::getFileBlocks()
{
  return fileBlocks;
}

map<string, CodeBlock> Parser::getCodeBlocks()
{
  return codeBlocks;
}
```
