#include "Parser.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <regex>

using namespace std;


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
                (path.rfind("ftp://", 0) != 0) &&
                (fileBlocks.find(path) == fileBlocks.end()) &&
                (codeBlocks.find(path) == codeBlocks.end()))
              {
                unprocessedSources.push_back(path);
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
          if (fileBlocks.find(block->getName()) != fileBlocks.end())
          {
            cout << "Error: Duplicate file block \"" << block->getName() <<
              "\" in file \"" << source << "\".";
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
                block->getName() << "\" in file \"" << source << "\".";
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
                "\" in file \"" << source << "\".";
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


