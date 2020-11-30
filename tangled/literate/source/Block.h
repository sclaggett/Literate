#pragma once

#include <string>
#include <vector>

#define BLOCK_DELIMITER "```"

class Block
{
public:
  Block(std::string sourceFile, uint32_t sourceLine);
  virtual ~Block();

public:
  virtual bool parseHeader(std::string line) = 0;
  bool checkEnd(std::string line);
  void addLine(std::string line);

  std::string getSourceFile();
  uint32_t getSourceLine();
  std::string getName();
  std::vector<std::string> getLines();

protected:
  std::string sourceFile;
  uint32_t sourceLine;
  std::string name;
  std::vector<std::string> lines;
};
