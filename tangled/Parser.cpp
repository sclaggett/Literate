#include "Parser.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <regex>
using namespace std;

Parser::~Parser()
{
  for (auto it = fileBlocks.begin(); it != fileBlocks.end(); ++it)
  {
    delete it->second;
  }
  fileBlocks.clear();
  for (auto it = codeBlocks.begin(); it != codeBlocks.end(); ++it)
  {
    delete it->second;
  }
  codeBlocks.clear();
}

map<string, FileBlock*> Parser::getFileBlocks()
{
  return fileBlocks;
}

map<string, CodeBlock*> Parser::getCodeBlocks()
{
  return codeBlocks;
}

bool Parser::parse(std::string literateFile)
{
  deque<string> unprocessedSources, processedSources;
  unprocessedSources.push_back(literateFile);
  while (!unprocessedSources.empty())
  {
    string source = unprocessedSources.front();
    ifstream sourceStream(source);
    if (!sourceStream.good())
    {
      cout << "Warning: File \"" << source << "\" not found, skipping." << endl;
      unprocessedSources.pop_front();
      processedSources.push_back(source);
      continue;
    }
    vector<string> lines;
    string line;
    while (getline(sourceStream, line))
    {
      lines.push_back(line);
    }
    sourceStream.close();
    string rootDirectory;
    size_t index = source.rfind("/");
    if (index != string::npos)
    {
      rootDirectory = source.substr(0, index + 1);
    }
    Block* block = nullptr;
    bool isBlockFile = false;
    for (uint32_t lineNumber = 0; lineNumber < lines.size(); ++lineNumber)
    {
      line = lines[lineNumber];
      if (block == nullptr)
      {
        if ((lineNumber + 1) < lines.size())
        {
          string nextLine = lines[lineNumber + 1];
          if (FileBlock::checkStart(line, nextLine))
          {
            block = new FileBlock(rootDirectory, source, lineNumber);
            isBlockFile = true;
          }
          else if (CodeBlock::checkStart(line, nextLine))
          {
            block = new CodeBlock(source, lineNumber);
            isBlockFile = false;
          }
          if (block != nullptr)
          {
            if (!block->parseHeader(line))
            {
              cout << "Error: Failed to parse block header in line " <<
                to_string(lineNumber) << " of file \"" << source << "\"." << endl;
              return false;
            }
            lineNumber += 1;
            continue;
          }
        }
        if (block == nullptr)
        {
          regex linkRegEx("\\[\\w+\\]\\((.*?\\.md)\\)");
          smatch results;
          string::const_iterator matchStart(line.cbegin());
          while (regex_search(matchStart, line.cend(), results, linkRegEx))
          {
            if (results.size() == 2)
            {
              string path = results[1];
              if ((path.rfind("http://", 0) != 0) &&
                (path.rfind("https://", 0) != 0) &&
                (path.rfind("ftp://", 0) != 0))
              {
                string fullPath = rootDirectory + path;
                if ((find(unprocessedSources.begin(), unprocessedSources.end(),
                  fullPath) == unprocessedSources.end()) && 
                  (find(processedSources.begin(), processedSources.end(),
                  fullPath) == processedSources.end()))
                {
                  unprocessedSources.push_back(fullPath);
                }
              }
            }
            matchStart = results.suffix().first;
          }
        }
      }
      else
      {
        if (!block->checkEnd(line))
        {
          block->addLine(line);
          continue;
        }
        if (isBlockFile)
        {
          auto existingBlockIt = fileBlocks.find(block->getName());
          if (existingBlockIt != fileBlocks.end())
          {
            FileBlock* existingBlock = existingBlockIt->second;
            cout << "Error: Duplicate file block \"" << block->getName() <<
              "\" in line " << to_string(lineNumber) << " of file \"" << source <<
              "\", previously encountered in line " << existingBlock->getSourceLine() <<
              " of file \"" << existingBlock->getSourceFile() << "\"." << endl;
            return false;
          }
          fileBlocks.insert(make_pair(block->getName(),
            dynamic_cast<FileBlock*>(block)));
          block = nullptr;
        }
        else
        {
          CodeBlock* codeBlock = dynamic_cast<CodeBlock*>(block);
          if (codeBlock->getAppend())
          {
            map<string, CodeBlock*>::iterator it = codeBlocks.find(codeBlock->getName());
            if (it == codeBlocks.end())
            {
              cout << "Error: Cannot append to non-existent code block \"" <<
                block->getName() << "\" in line " << to_string(lineNumber) <<
                " of file \"" << source << "\"." << endl;
              return false;
            }
            CodeBlock* existingBlock = it->second;
            vector<string> newLines = codeBlock->getLines();
            for (vector<string>::iterator lineIt = newLines.begin();
              lineIt != newLines.end(); ++lineIt)
            {
              existingBlock->addLine(*lineIt);
            }
            delete block;
            block = nullptr;
          }
          else
          {
            if (codeBlocks.find(block->getName()) != codeBlocks.end())
            {
              cout << "Error: Duplicate code block \"" << block->getName() <<
                "\" in line " << to_string(lineNumber) << " of file \"" << source <<
                "\"." << endl;
              return false;
            }
            codeBlocks.insert(make_pair(block->getName(),
              dynamic_cast<CodeBlock*>(block)));
            block = nullptr;
          }
        }
      }
    }
    unprocessedSources.pop_front();
    processedSources.push_back(source);
  }
  return true;
}
