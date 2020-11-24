# Tangler
@title TEMP
@s TEMP

The *Tangler* class contains the logic for tangling the file and code blocks into the ouput files. It is intended to be used by calling the *tangle()* function with the file and code blocks and output directory and it will generate all output files.

The sections below contain the header file and implementation overview for this class.

@file source/Tangler.h
```cpp
#pragma once

#include <map>
#include <string>
#include "CodeBlock.h"
#include "FileBlock.h"

class Tangler
{
public:
  bool tangle(std::map<std::string, FileBlock*> fileBlocks,
    std::map<std::string, CodeBlock*> codeBlocks, std::string outputDirectory);

private:
  // TODO: Keep the output as an array of lines
  bool tangleBlock(Block* block, std::map<std::string, CodeBlock*> codeBlocks,
    std::vector<std::string>& output);

  std::map<std::string, std::vector<std::string>> tangledBlocks;
};
```

@file source/Tangler.cpp
```cpp
@{[tangler] Includes}
@{[tangler] Namespaces}

@{[tangler] Tangle}

@{[tangler] Tangle block}
```

Including the class header file and use the *std* namespace.

@code [tangler] Includes
```cpp
#include "Tangler.h"
```

@code [tangler] Namespaces
```cpp
using namespace std;
```

## Tangling

The section below give an overview of the tangling process: tangle each code block individually, combine the code blocks into file blocks, and write the file blocks to disk. The class variable *tangledBlocks* holds the results of the first step and the local variable *outputFiles* holds the results of the second.

@code [tangler] Tangle
```cpp
bool Tangler::tangle(map<string, FileBlock*> fileBlocks,
    map<string, CodeBlock*> codeBlocks, string outputDirectory)
{
  @{[tangler] Tangle code blocks}
  map<FileBlock*, vector<string>> outputFiles;
  @{[tangler] Tangle file blocks}
  @{[tangler] Write files}
  return true;
}
```

The logic for tangling code and file blocks is straightforward at this point because both rely on the private function *tangleBlock* which will be defined later. What's important for understanding the following is that *tangleBlock* takes a *Block* pointer and a list of code blocks as inputs and create a list of tangled strings as output.

@code [tangler] Tangle code blocks
```cpp
for (auto it = codeBlocks.begin(); it != codeBlocks.end(); ++it)
{
  vector<string> output;
  if (!tangleBlock(it->second, codeBlocks, output))
  {
    return false;
  }
  tangledBlocks.insert(make_pair(it->first, output));
}
```

@code [tangler] Tangle file blocks
```cpp
for (auto it = fileBlocks.begin(); it != fileBlocks.end(); ++it)
{
  vector<string> output;
  if (!tangleBlock(it->second, codeBlocks, output))
  {
    return false;
  }
  outputFiles.insert(make_pair(it->second, output));
}
```

The final step is to write the file blocks to the disk. This step is a bit more involved and takes place in the stages listed below.

@code [tangler] Write files
```cpp
@{[tangler] Prepare output directory}
for (auto it = outputFiles.begin(); it != outputFiles.end(); ++it)
{
  string outputPath = outputDirectory + it->first->getName();
  @{[tangler] Concatenate block lines}
  @{[tangler] Skip unchanged files}
  @{[tangler] Create missing directories}
  @{[tangler] Write block to file}
  @{[tangler] Set execute bit}
}
```

Prepare the output directory for concatenation with the individual file names before we start iterating over the blocks. On Linux this simply involves checking if the user appended a directory separator and adding one if not.

@code [tangler] Prepare output directory
```cpp
if (outputDirectory.back() != '/')
{
  outputDirectory += "/";
}
```

The first step for each file block is to concatenate all the lines with newlines to create a single string.

@code [tangler] Concatenate block lines
```cpp
stringstream concatStream;
vector<string> outputLines = it->second;
for (auto it = outputLines.begin(); it != outputLines.end(); ++it)
{
  concatStream << *it << endl;
}
string outputString = concatStream.str();
```

The next step for each file block is to skip it if it already exists and hasn't changed. This can be a huge timesaver by prevent unnecessary recompilation by toolchains that rely on the last modified timestamp to detect changes.

@code [tangler] Skip unchanged files
```cpp
ifstream inStream(outputPath);
if (inStream.good())
{
  stringstream existingFile;
  existingFile << inStream.rdbuf();
  inStream.close();
  if (existingFile.str() == outputString)
  {
    continue;
  }
}
```

At this point the file either doesn't exist or has changed and needs to be updated. Create any missing directories so the file can be created. No function exists on Linux to create multiple directories at the same time so we must walk the tree and deal with each directory individually.

@code [tangler] Create missing directories
```cpp
size_t position = outputPath.find("/", 0);
while (position != string::npos)
{
  string directory = outputPath.substr(0, position);
  struct stat st;
  if (stat(directory.c_str(), &st) != 0)
  {
    if (mkdir(directory.c_str(), S_IRWXU | S_IRGRP | S_IXGRP |
      S_IROTH | S_IXOTH) != 0)
    {
      cout << "Error: Failed to create directory '" << directory <<
        "'." << endl;
      return false;
    }
  }
  else if (!(st.st_mode & S_IFDIR))
  {
    cout << "Error: Cannot create directory '" << directory <<
      "' because a file exists with the same name." << endl;
    return false;
  }
  position = outputPath.find("/", position + 1);
}
```

Writing the file block to disk is actually quite simple.

@code [tangler] Write block to file
```cpp
ofstream outStream(outputPath);
outStream << outputString;
outStream.close();
```

Lastly, set the execute bit on the output file if the flag was set on the file block. On Linux this involves reading the current file permissions and writing a modified set.

@code [tangler] Set execute bit
```cpp
if (it->first->getExecutable())
{
  struct stat st;
  if (stat(outputPath.c_str(), &st) != 0)
  {
    cout << "Error: Failed to check file permissions for '" << outputPath <<
      "'." << endl;
    return false;
  }
  if (chmod(outputPath.c_str(), st.st_mode | S_IXUSR | S_IXGRP |
    S_IXOTH) != 0)
  {
    cout << "Error: Failed to set execute bit on '" << outputPath <<
      "'." << endl;
    return false;
  }
}
```

Append the includes necessary for the above code blocks.

@code [tangler] Includes +=
```cpp
#include <fstream>
#include <iostream>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
```

## Tangle block

The final piece that needs to be written is the *tangleBlock* function that we used above. The stanza below gives an overview of the function logic which processes each line separately.

@code [tangler] Tangle block
```cpp
bool Tangler::tangleBlock(Block* block, map<string, CodeBlock*> codeBlocks,
  vector<string>& output)
{
  regex regEx("^(\\s*)@\\{((\\[|\\]|\\w|\\s).*)\\}\\s*$");
  vector<string> lines = block->getLines();
  for (auto it = lines.begin(); it != lines.end(); ++it)
  {
    @{[tangler] Append lines without child code blocks}
    @{[tangler] Parse child code block}
    @{[tangler] Use previously tangled result}
    @{[tangler] Tangle unprocessed block}
    @{[tangler] Append child block to output}
  }
  return true;
}
```

The first step in each loop is to check if the line matches the regex. If not, then it does not contain a child block and can be directly appended to the ouput and the rest of the loop skipped.

@code [tangler] Append lines without child code blocks
```cpp
string line = *it;
smatch results;
if (!regex_match(line, results, regEx))
{
  output.push_back(line);
  continue;
}
```

By the next step we've determined that the line does contain a child code block. Make sure parsing produced the expected number of results and extract the whitespace and name.

@code [tangler] Parse child code block
```cpp
if (results.size() != 4)
{
  cout << "Error: Unexpected results parsing line '" << line <<
    "'." << endl;
  return false;
}
string whitespace = results[1];
string name = results[2];
```

Check our *tangledBlock* map to see if we've already processed this block. If so, remember the result and set the *found* flag.

@code [tangler] Use previously tangled result
```cpp
vector<string> childOutput;
bool found = false;
auto tangledBlock = tangledBlocks.find(name);
if (tangledBlock != tangledBlocks.end())
{
  childOutput = tangledBlock->second;
  found = true;
}
```

The code block has not been processed before if the *found* flag is not set. Find the raw block, tangle it, and add the results to the *tangledBlocks* map for use later if we encounter the same block again.

@code [tangler] Tangle unprocessed block
```cpp
if (!found)
{
  auto rawBlock = codeBlocks.find(name);
  if (rawBlock == codeBlocks.end())
  {
    cout << "Error: Unable to find block '" << name << "'." << endl;
    return false;
  }
  if (!tangleBlock(rawBlock->second, codeBlocks, childOutput))
  {
    return false;
  }
  tangledBlocks.insert(make_pair(rawBlock->first, childOutput));
}
```

The final step is to append the child block to the output. Prepend the whitespace so the indentation is correct in the tangled output which makes the results more readable.

@code [tangler] Append child block to output
```cpp
for (auto it = childOutput.begin(); it != childOutput.end(); ++it)
{
  output.push_back(whitespace + *it);
}
```

Append the includes necessary for the above code blocks.

@code [tangler] Includes +=
```cpp
#include <regex>
```
