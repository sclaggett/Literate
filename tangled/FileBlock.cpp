#include "FileBlock.h"
using namespace std;
#define FILE_BLOCK_PREFIX "@file "
#define EXECUTE_POSTFIX " +x"

FileBlock::FileBlock(string file, uint32_t line) :
  Block(file, line),
  executable(false)
{
}
FileBlock::~FileBlock()
{
}

bool FileBlock::checkStart(string line1, string line2)
{
  return (line1.rfind(FILE_BLOCK_PREFIX, 0) == 0) &&
    (line2.rfind(BLOCK_DELIMITER, 0) == 0);
}
bool FileBlock::parseHeader(string line)
{
  name = line.substr(strlen(FILE_BLOCK_PREFIX));
  if (name.find(EXECUTE_POSTFIX, name.size() - strlen(EXECUTE_POSTFIX)) !=
    string::npos)
  {
    name = name.substr(0, name.size() - strlen(EXECUTE_POSTFIX));
    executable = true;
  }
  return true;
}
bool FileBlock::getExecutable()
{
  return executable;
}
