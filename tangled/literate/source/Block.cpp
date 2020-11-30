#include "Block.h"
using namespace std;

Block::Block(string file, uint32_t line) : 
  sourceFile(file),
  sourceLine(line)
{
}
Block::~Block()
{
}

bool Block::checkEnd(string line)
{
  return (line == BLOCK_DELIMITER);
}
void Block::addLine(string line)
{
  lines.push_back(line);
}
string Block::getSourceFile()
{
  return sourceFile;
}

uint32_t Block::getSourceLine()
{
  return sourceLine;
}

string Block::getName()
{
  return name;
}

vector<string> Block::getLines()
{
  return lines;
}
