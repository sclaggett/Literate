#include "CodeBlock.h"

using namespace std;

#define CODE_BLOCK_PREFIX "@code "
#define APPEND_POSTFIX " +="


CodeBlock::CodeBlock(string file, uint32_t line) :
  Block(file, line),
  append(false)
{
}

CodeBlock::~CodeBlock()
{
}


bool CodeBlock::checkStart(string line1, string line2)
{
  return (line1.rfind(CODE_BLOCK_PREFIX, 0) == 0) &&
    (line2.rfind(BLOCK_DELIMITER, 0) == 0);
}

bool CodeBlock::parseHeader(string line)
{
  name = line.substr(strlen(CODE_BLOCK_PREFIX));
  if (name.find(APPEND_POSTFIX, name.size() - strlen(APPEND_POSTFIX)) !=
    string::npos)
  {
    name = name.substr(0, name.size() - strlen(APPEND_POSTFIX));
    append = true;
  }
  return true;
}

bool CodeBlock::getAppend()
{
  return append;
}


