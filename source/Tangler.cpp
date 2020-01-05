#include "Tangler.h"
#include <iostream>

using namespace std;

Tangler::Tangler()
{
}

Tangler::~Tangler()
{
}

bool Tangler::tangle(map<string, FileBlock> fileBlocks,
    map<string, CodeBlock> codeBlocks, string outputDirectory)
{
  cout << "Tangle " << outputDirectory << endl;
  return true;
}

