# Block
@title TEMP
@s TEMP

The *Block* class is an abstract base class that encapsulates variables and functions common to the *FileBlock* and *CodeBlock* classes. Three functions are defined for use when parsing blocks out of a literate file:

1. *parseHeader()*: Parses the name and modifiers from the header.
2. *checkEnd()*: Checks for the end of a block.
3. *addLine()*: Adds a line to the internal array.

The function *parseHeader()* is abstract and must be implemented by derived classes.

The sections below contain the header file and implementation overview for this class.

@file source/Block.h
```cpp
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
```

@file source/Block.cpp
```cpp
@{[block] Includes}
@{[block] Namespaces}

@{[block] Constructor}
@{[block] Destructor}

@{[block] Check for end}
@{[block] Add line}
@{[block] Getters}
```

Including the class header file and use the *std* namespace.

@code [block] Includes
```cpp
#include "Block.h"
```

@code [block] Namespaces
```cpp
using namespace std;
```

## Construction and destruction

The constructor simply remembers the source file and line while the destructor doesn't do anything.

@code [block] Constructor
```cpp
Block::Block(string file, uint32_t line) : 
  sourceFile(file),
  sourceLine(line)
{
}
```

@code [block] Destructor
```cpp
Block::~Block()
{
}
```

## Parsing

Most of the parsing logic exists in the *parseHeader()* function of the derived classes, leaving this class quite simple. Define the *checkEnd()* function which checks for the block delimiter and a function to add a line.

@code [block] Check for end
```cpp
bool Block::checkEnd(string line)
{
  return (line == BLOCK_DELIMITER);
}
```

@code [block] Add line
```cpp
void Block::addLine(string line)
{
  lines.push_back(line);
}
```

## Getters

Define getters for use by external classes.

@code [block] Getters
```cpp
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
```
