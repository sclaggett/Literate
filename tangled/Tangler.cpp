#include "Tangler.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <regex>
using namespace std;

bool Tangler::tangle(map<string, FileBlock*> fileBlocks,
    map<string, CodeBlock*> codeBlocks, string outputDirectory)
{
  for (auto it = codeBlocks.begin(); it != codeBlocks.end(); ++it)
  {
    vector<string> output;
    if (!tangleBlock(it->second, codeBlocks, output))
    {
      return false;
    }
    tangledBlocks.insert(make_pair(it->first, output));
  }
  map<FileBlock*, vector<string>> outputFiles;
  for (auto it = fileBlocks.begin(); it != fileBlocks.end(); ++it)
  {
    vector<string> output;
    if (!tangleBlock(it->second, codeBlocks, output))
    {
      return false;
    }
    outputFiles.insert(make_pair(it->second, output));
  }
  if (outputDirectory.back() != '/')
  {
    outputDirectory += "/";
  }
  for (auto it = outputFiles.begin(); it != outputFiles.end(); ++it)
  {
    string outputPath = outputDirectory + it->first->getName();
    stringstream concatStream;
    vector<string> outputLines = it->second;
    for (auto it = outputLines.begin(); it != outputLines.end(); ++it)
    {
      concatStream << *it << endl;
    }
    string outputString = concatStream.str();
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
    ofstream outStream(outputPath);
    outStream << outputString;
    outStream.close();
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
  }
  return true;
}

bool Tangler::tangleBlock(Block* block, map<string, CodeBlock*> codeBlocks,
  vector<string>& output)
{
  regex regEx("^(\\s*)@\\{((\\[|\\]|\\w|\\s).*)\\}\\s*$");
  vector<string> lines = block->getLines();
  for (auto it = lines.begin(); it != lines.end(); ++it)
  {
    string line = *it;
    smatch results;
    if (!regex_match(line, results, regEx))
    {
      output.push_back(line);
      continue;
    }
    if (results.size() != 4)
    {
      cout << "Error: Unexpected results parsing line '" << line <<
        "'." << endl;
      return false;
    }
    string whitespace = results[1];
    string name = results[2];
    vector<string> childOutput;
    bool found = false;
    auto tangledBlock = tangledBlocks.find(name);
    if (tangledBlock != tangledBlocks.end())
    {
      childOutput = tangledBlock->second;
      found = true;
    }
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
    for (auto it = childOutput.begin(); it != childOutput.end(); ++it)
    {
      output.push_back(whitespace + *it);
    }
  }
  return true;
}
