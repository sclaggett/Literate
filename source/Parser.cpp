#include "Parser.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>

using namespace std;


Parser::Parser()
{
}

Parser::~Parser()
{
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
  list<string> unprocessedSources, processedSources;
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

    Block* block = nullptr;
    bool isBlockFile = false;
    for (uint32_t i = 0; i < lines.size(); ++i)
    {
      line = lines[i];
      if (block == nullptr)
      {
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

      }
    }

    unprocessedSources.pop_front();
    processedSources.push_back(source);

  }
  return true;
}


