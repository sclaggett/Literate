# CodeBlock
@title TEMP
@s TEMP

The *CodeBlock* class derives from the base *Block* class, encapsulates a single literate code block, and contains the associated parsing logic.

A code block has the following signature:

>  @code \<name> [+=]
>  ```
>  [line 1]
>  [line 2]
>  ...
>  [line n]
>  ```

The presence of the optional `+=` modifier indicates that the code block should be appended to an already existing one with the same name.

The sections below contain the header file and implementation overview for this class.

@file source/CodeBlock.h
```cpp
#pragma once

#include "Block.h"

class CodeBlock : public Block
{
public:
  CodeBlock(std::string sourceFile, uint32_t sourceLine);
  virtual ~CodeBlock();

public:
  static bool checkStart(std::string line1, std::string line2);
  bool parseHeader(std::string line);
  bool getAppend();

private:
  bool append;
};
```

@file source/CodeBlock.cpp
```cpp
@{[codeblock] Includes}
@{[codeblock] Namespaces}
@{[codeblock] Definitions}

@{[codeblock] Constructor}
@{[codeblock] Destructor}

@{[codeblock] Check for start}
@{[codeblock] Parse header}
@{[codeblock] Get append flag}
```

Including the class header file and use the *std* namespace.

@code [codeblock] Includes
```cpp
#include "CodeBlock.h"
```

@code [codeblock] Namespaces
```cpp
using namespace std;
```

Define the code block prefix and append modifier postfix.

@code [codeblock] Definitions
```cpp
#define CODE_BLOCK_PREFIX "@code "
#define APPEND_POSTFIX " +="
```

## Construction and destruction

The constructor simply passes the source file and line to the base class while the destructor doesn't do anything.

@code [codeblock] Constructor
```cpp
CodeBlock::CodeBlock(string file, uint32_t line) :
  Block(file, line),
  append(false)
{
}
```

@code [codeblock] Destructor
```cpp
CodeBlock::~CodeBlock()
{
}
```

## Check for start

The static *checkStart()* function takes two consecutive lines and tests to see if they contain the start of a code block. This is done by checking if the first starts with the code block prefix and the second with the block delimiter. The code below uses the *rfind()* function to test if a string starts with a particular value.

@code [codeblock] Check for start
```cpp
bool CodeBlock::checkStart(string line1, string line2)
{
  return (line1.rfind(CODE_BLOCK_PREFIX, 0) == 0) &&
    (line2.rfind(BLOCK_DELIMITER, 0) == 0);
}
```

## Parse header

Define the *parseHeader()* function which extracts the name and optional append modifier from the code block header. This function is intended to be called after *checkStart()* has confirmed the presence of a new code block.

Strip off the code block prefix from the header to get the name. The name may also contain the append modifier so test for that as well. Strip if off if found and set the flag.

@code [codeblock] Parse header
```cpp
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
```

## Get append flag

Define a getter that external classes will use to check the append flag.

@code [codeblock] Get append flag
```cpp
bool CodeBlock::getAppend()
{
  return append;
}
```
