# Parser
@title TEMP
@s TEMP

The *Parser* class contains the logic for parsing the web of literate source files. It is intended to be used by calling the *parse()* function with the root literate file as the parameter and it will walk the web of all linked files. The parsed blocks can then be obtained using the *getFileBlocks()* and *getCodeBlocks()* functions.

This class is designed to load all source files into memory at once. I've chosen this approach because it's easier than handling things as streams. My rationalization is that text is quite small compared to the amount of memory that modern computers have, hence I don't anticipate this causing any issues. Granted, assumptions like that are the root of all pain and suffering in software development and this may bite someone eventually. But since this is a literate program you'll at least know that it was an intentional decision rather than an oversight and will know how to curse me appropriately.

The sections below contain the header file and implementation overview for this class.

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
  std::map<std::string, FileBlock*> getFileBlocks();
  std::map<std::string, CodeBlock*> getCodeBlocks();

private:
  std::map<std::string, FileBlock*> fileBlocks;
  std::map<std::string, CodeBlock*> codeBlocks;
};
```

@file source/Parser.cpp
```cpp
@{[parser] Includes}
@{[parser] Namespaces}

@{[parser] Constructor}
@{[parser] Destructor}

@{[parser] Getters}

@{[parser] Parse web}
```

Including the class header file and use the *std* namespace.

@code [parser] Includes
```cpp
#include "Parser.h"
```

@code [parser] Namespaces
```cpp
using namespace std;
```

## Construction and destruction

The constructor and destructor don't do anything.

@code [parser] Constructor
```cpp
Parser::Parser()
{
}
```

**TODO** Delete blocks.

@code [parser] Destructor
```cpp
Parser::~Parser()
{
}
```

## Getters

Define getters that allow external classes to access the file and code block maps.

@code [parser] Getters
```cpp
map<string, FileBlock*> Parser::getFileBlocks()
{
  return fileBlocks;
}

map<string, CodeBlock*> Parser::getCodeBlocks()
{
  return codeBlocks;
}
```

## Parsing

The code block below give an overview of the parsing process. Start by defining two arrays, one that will contain a list of literate files that need to be processed and another to keep track of files that have already been processed. We'll add new literate files to the first array as we encounter links to them and move them to the second array after they've been processed to avoid duplicating work.

Each literate source is processed in four steps as outlined below.

@code [parser] Parse web
```cpp
bool Parser::parse(std::string literateFile)
{
  list<string> unprocessedSources, processedSources;
  unprocessedSources.push_back(literateFile);
  while (!unprocessedSources.empty())
  {
    @{[parser] Check if source exists}
    @{[parser] Read next source into memory}
    @{[parser] Parse source}
    @{[parser] Move source to processed list}
  }
  return true;
}
```

**Check if source exists.** The first step is to make sure the source file exists. We don't want to fail if a source isn't found so simply issue a warning, move the source to the process list, and continue around the loop.

@code [parser] Check if source exists
```cpp
string source = unprocessedSources.front();
ifstream sourceStream(source);
if (!sourceStream.good())
{
  cout << "Warning: File \"" << source << "\" not found, skipping." << endl;
  @{[parser] Move source to processed list}
  continue;
}
```

**Read source.** Read the entire source file into memory as a array of lines and close the stream.

@code [parser] Read next source into memory
```cpp
vector<string> lines;
string line;
while (getline(sourceStream, line))
{
  lines.push_back(line);
}
sourceStream.close();
```

**Parse source.** The third step is the most involved: process all lines in the file, extract the file and code blocks, and remember any other literate sources that we encounter links to.

Start by defining a *block* variable to keep track of the current file or code block that we are parsing and an *isBlockFile* flag to remember if it's a file or code block. Iterate over each line in the file and handle it differently depending on whether we're currently parsing a block or not.

@code [parser] Parse source
```cpp
Block* block = nullptr;
bool isBlockFile = false;
for (uint32_t i = 0; i < lines.size(); ++i)
{
  line = lines[i];
  if (block == nullptr)
  {
    @{[parser] Handle start of block}
    @{[parser] Parse source links}
  }
  else
  {
    @{[parser] Add block line}
    @{[parser] Handle end of block}
  }
}
```

Consider the first case where we are not currently parsing a block. Here we need to handle two conditions: the start of a new block and any source links.

The code below is where we detect and handle the start of a new block. Check that we have at least one additional line beyond the current one and invoke the static *checkStart()* functions of the derived classes for the actual check.

@code [parser] Handle start of block
```cpp
if ((i + 1) < lines.size())
{
  string nextLine = lines[i + 1];
  if (FileBlock::checkStart(line, nextLine))
  {
    block = new FileBlock(source, i);
    isBlockFile = true;
  }
  else if (CodeBlock::checkStart(line, nextLine))
  {
    block = new CodeBlock(source, i);
    isBlockFile = false;
  }
  if (block != nullptr)
  {
    if (!block->parseHeader(line))
    {
      cout << "Error: Failed to parse block header in file \"" << source <<
        "\" line " << to_string(i) << ".";
      return false;
    }
    i += 1;
    continue;
  }
}
```

@code [parser] Parse source links
```cpp
```

Next comes the second case in which we are currently parsing a block. Again two conditions need to be handled: lines that belong to the current block and the end of the block.

The code below adds lines to the current block which turns out to be fairly simple: make sure we aren't at the end of the block, add the line and continue around the loop.

@code [parser] Add block line
```cpp
if (!block->checkEnd(line))
{
  block->addLine(line);
  continue;
}
```

@code [parser] Handle end of block
```cpp
if (isBlockFile)
{
  if (fileBlocks.find(block->getName()) != fileBlocks.end())
  {
    cout << "Error: Duplicate file block \"" << block->getName() <<
      "\" in file \"" << source << "\".";
    return false;
  }
  fileBlocks.insert(make_pair(block->getName(),
    dynamic_cast<FileBlock*>(block)));
}
else
{
  // Check for append modifier. If not set, make sure block isn't a duplicate and insert it. If set, make sure block has already been defined and append to original.
  codeBlocks.insert(make_pair(block->getName(),
    dynamic_cast<CodeBlock*>(block)));
}
block = nullptr;
```




**Move source to processed list.** The final step is to move the source from the unprocessed list to the processed one. Any new sources that were discovered via links will have been appended to the back of the unprocessed list so we can simply pop off the front item.

@code [parser] Move source to processed list
```cpp
unprocessedSources.pop_front();
processedSources.push_back(source);
```

Include the necessary headers.

@code [parser] Includes +=
```cpp
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
```
