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

