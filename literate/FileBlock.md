# FileBlock
@title TEMP
@s TEMP

The *FileBlock* class derives from the base *Block* class, encapsulates a single literate file block, and contains the associated parsing logic.

A file block has the following signature:

>  @file <name> [+x]
>  ```
>  [line 1]
>  [line 2]
>  ...
>  [line n]
>  ```

The presence of the optional `+x` modifier indicates that the file should have the execute bit set.

The sections below contain the header file and implementation overview for this class.

@file source/FileBlock.h
```cpp
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
```

@file source/FileBlock.cpp
```cpp
@{[fileblock] Includes}
@{[fileblock] Namespaces}
@{[fileblock] Definitions}

@{[fileblock] Constructor}
@{[fileblock] Destructor}

@{[fileblock] Check for start}
@{[fileblock] Parse header}
@{[fileblock] Get executable flag}
```

Including the class header file and use the *std* namespace.

@code [fileblock] Includes
```cpp
#include "FileBlock.h"
```

@code [fileblock] Namespaces
```cpp
using namespace std;
```

Define the file block prefix and execute modifier postfix.

@code [fileblock] Definitions
```cpp
#define FILE_BLOCK_PREFIX "@file "
#define EXECUTE_POSTFIX " +x"
```

## Construction and destruction

The constructor simply passes the source file and line to the base class while the destructor doesn't do anything.

@code [fileblock] Constructor
```cpp
FileBlock::FileBlock(string file, uint32_t line) :
  Block(file, line),
  executable(false)
{
}
```

@code [fileblock] Destructor
```cpp
FileBlock::~FileBlock()
{
}
```

## Check for start

The static *checkStart()* function takes two consecutive lines and tests to see if they contain the start of a file block. This is done by checking if the first starts with the file block prefix and the second with the block delimiter. The code below uses the *rfind()* function to test if a string starts with a particular value.

@code [fileblock] Check for start
```cpp
bool FileBlock::checkStart(string line1, string line2)
{
  return (line1.rfind(FILE_BLOCK_PREFIX, 0) == 0) &&
    (line2.rfind(BLOCK_DELIMITER, 0) == 0);
}
```

## Parse header

Define the *parseHeader()* function which extracts the name and optional executable modifier from the file block header. This function is intended to be called after *checkStart()* has confirmed the presence of a new file block.

Strip off the file block prefix from the header to get the name. The name may also contain the execute modifier so test for that as well. Strip if off if found and set the flag.

@code [fileblock] Parse header
```cpp
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
```

## Get executable flag

Define a getter that external classes will use to check the executable flag.

@code [fileblock] Get executable flag
```cpp
bool FileBlock::getExecutable()
{
  return executable;
}
```
