#include "Main.h"
#define OPTPARSE_IMPLEMENTATION
#include "thirdparty/Optparse.h"
#include "Parser.h"
#include "Tangler.h"
#include <iostream>

using namespace std;

#define LITERATE_VERSION "0.2"


Main::Main()
{
}

Main::~Main()
{
}


uint32_t Main::run(int argc, char** argv)
{
  struct optparse_long longopts[] =
  {
    {"help", 'h', OPTPARSE_NONE},
    {"version", 'v', OPTPARSE_NONE},
    {"out", 'o', OPTPARSE_REQUIRED},
    {0}
  };

  string outputDirectory;
  int option;
  struct optparse options;
  optparse_init(&options, argv);
  while ((option = optparse_long(&options, longopts, NULL)) != -1)
  {
    switch (option)
    {
    case 'h':
      cout << "Usage:" << endl;
      cout << "  lit [options] <literate file>" << endl << endl;
      cout << "Options:" << endl;
      cout << "  --help/-h      Show the help text." << endl;
      cout << "  --version/-v   Show the version number." << endl;
      cout << "  --out/-o DIR   Put the generated files in DIR." << endl;

      return 0;
  
    case 'v':
      cout << "Literate version " << LITERATE_VERSION << endl;
      return 0;
  
    case 'o':
      outputDirectory = options.optarg;
      break;
  
    case '?':
      cout << "Error: Unknown command line parameter." << endl << endl;
      cout << "Usage:" << endl;
      cout << "  lit [options] <literate file>" << endl << endl;
      cout << "Options:" << endl;
      cout << "  --help/-h      Show the help text." << endl;
      cout << "  --version/-v   Show the version number." << endl;
      cout << "  --out/-o DIR   Put the generated files in DIR." << endl;

      return -1;
    }
  }

  char* arg = optparse_arg(&options);
  if (arg == nullptr)
  {
    cout << "Error: Missing required literate source file in command line parameters." << endl << endl;
    cout << "Usage:" << endl;
    cout << "  lit [options] <literate file>" << endl << endl;
    cout << "Options:" << endl;
    cout << "  --help/-h      Show the help text." << endl;
    cout << "  --version/-v   Show the version number." << endl;
    cout << "  --out/-o DIR   Put the generated files in DIR." << endl;

    return -1;
  }
  string literateFile = arg;


  Parser parser;
  if (!parser.parse(literateFile))
  {
    return -1;
  }

  Tangler tangler;
  if (!tangler.tangle(parser.getFileBlocks(), parser.getCodeBlocks(),
    outputDirectory))
  {
    return -1;
  }

  return 0;
}


int main(int argc, char** argv) 
{
  string err;
  try
  {
    Main main;
    return main.run(argc, argv);
  } catch (const std::exception& ex) {
    err = ex.what();
  } catch (const std::string& ex) {
    err = ex;
  } catch (...) {
    err = "unknown";
  }
  cout << "Fatal error: " << err << endl;
  return -1;
}


