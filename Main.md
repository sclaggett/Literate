# Main
@title TEMP
@s TEMP

The *Main* class parses the command line and uses the *Parser* and *Tangler* classes to process the web of literate files.

This file also includes the *main()* application entry point.

The section below is the header file for the *Main* class. I've chosen to write header files as a single block in this project because they are quite simple. You can easily break more complicated header files up into multiple code blocks and intersperse explanations.

@file Main.h
```cpp
#pragma once

#include <string>

class Main
{
public:
  uint32_t run(int argc, char** argv);
};
```

A feature of literate programs that is hinted at in the header above is a reduction in the number of functions that need to be defined. It's considered bad practice in other paradigms to have one giant function that does many things because it muddles understanding. However, literate code is broken up and explained in a different fashion which allows you to only define functions when they are actually needed.

A related aspect which will be observed below is an almost complete lack of comments in the code. Comments are generally unnecessary because the code is explained in English.

Of course, the lack of extraneous functions and comments is what I've found works best. It isn't a requirement of literate programming itself.

The section below gives an overview of the implementation file. Again, my decision to start each literate file in this project with the header file and an overview of the implementation file is a matter of style--you can just as easily start by explaining various aspects of the class and pull everything together at the end.

@file Main.cpp
```cpp
@{[main] Includes}
@{[main] Namespaces}
@{[main] Definitions}

@{[main] Run}

@{[main] Application entry point}
```

The somewhat odd-looking syntax above means "replace this line with the code block named `[main] Includes`" which we will define elsewhere. Essentially all the *Literate* preprocessor does is gather up the file and code blocks, perform these substitutions, and write the results to disk.

Notice that the name of each code block starts with the namespace component `[main]`. This isn't required but is useful because *all code blocks exist in a single shared namespace*. That means you will encounter namespace collisions in larger projects unless you do something to make the names unique.

Including the class header file and use the *std* namespace. These items are defined in their own code blocks so additional includes and namespaces can be appended to them later.

@code [main] Includes
```cpp
#include "Main.h"
```

@code [main] Namespaces
```cpp
using namespace std;
```

Define the version number. The previous version of *Literate* was 0.1 so increment to 0.2:

@code [main] Definitions
```cpp
#define LITERATE_VERSION "0.2"
```

## Running

The *run()* function is where the action happens. The entire *Literate* program can be summed up in the three steps listed below.

@code [main] Run
```cpp
uint32_t Main::run(int argc, char** argv)
{
  @{[main] Parse command line arguments}
  @{[main] Parse web}
  @{[main] Tangle output}
  return 0;
}
```

**Parse command line.** The first step is to parse the command line. It's sometimes surprising to me how complicated it is to do something as fundamental as argument parsing in C++ (yeah, I know, I switched away from D which comes with that functionality build in). Use [Optparse](Optparse.md) for a portable solution with an interface that is essentially identical to *getopt()*.

The link above to *Optparse* has a dual purpose: it provides the human reader a path to the code and it lets the *Literate* preprocessor know that the file exists. *All literate source files must be accessible via links* from the root source file in order to be included in the tangled output.

Parsing occurs in the following three steps:

@code [main] Parse command line arguments
```cpp
@{[main] Define command line arguments}
@{[main] Process arguments}
@{[main] Extract input file}
```

Start by defining the command line arguments that we recognize:

- `--help/-h`: Show the help text.
- `--version/-v`: Show the version number.
- `--out/-o DIR`: Put the generated files in `DIR`.

One aspect of the following code block that wasn't immediately clear to me is the meaning of the *OPTPARSE_NONE*/*OPTPARSE_REQUIRED* flags. Experimentation has shown that the latter indicates that the output directory parameter must be followed by an additional parameter.

@code [main] Define command line arguments
```cpp
struct optparse_long longopts[] =
{
  {"help", 'h', OPTPARSE_NONE},
  {"version", 'v', OPTPARSE_NONE},
  {"out", 'o', OPTPARSE_REQUIRED},
  {0}
};
```

Iterate over each command line argument and process it. The output directory defaults to the current directory if not specified.

@code [main] Process arguments
```cpp
string outputDirectory(".");
int option;
struct optparse options;
optparse_init(&options, argv);
while ((option = optparse_long(&options, longopts, NULL)) != -1)
{
  switch (option)
  {
  case 'h':
    @{[main] Print help}
    return 0;

  case 'v':
    cout << "Literate version " << LITERATE_VERSION << endl;
    return 0;

  case 'o':
    outputDirectory = options.optarg;
    break;

  default:
    cout << "Error: Unknown command line parameter." << endl << endl;
    @{[main] Print help}
    return -1;
  }
}
```

Note that what might be a function named *printHelp()* under a different paradigm can be written a code block that is used several times. Make sure you understand that this approach will result in code duplication in the tangled output. This is similar to an inline function in C++ and a similar thought process should be used to decide if a chunk of logic should be a code block or a function.

The input literate file will come through the parser as a non-flag argument. Make sure an input file exists and ignore anything after the first value.

@code [main] Extract input file
```cpp
char* arg = optparse_arg(&options);
if (arg == nullptr)
{
  cout << "Error: Missing required literate source file in command line parameters." << endl << endl;
  @{[main] Print help}
  return -1;
}
string literateFile = arg;
```

Define the code block that prints the help message.

@code [main] Print help
```cpp
cout << "Usage:" << endl;
cout << "  lit [options] <literate file>" << endl << endl;
cout << "Options:" << endl;
cout << "  --help/-h      Show the help text." << endl;
cout << "  --version/-v   Show the version number." << endl;
cout << "  --out/-o DIR   Put the generated files in DIR." << endl;
```

**Parse web.** The second step is to parse the web of Markdown files starting with the input file. A simple project may consist of just a single input file while a more complicated one could have hundreds of literate files that are tied together by a web of Markdown links. The responsibility for parsing the input files and walking the web has been delegated to the *Parser* class which makes the following code block trivial.

The parser will print a description of any error it encounters to *stdout*. Simply exit with an error code if parsing fails.

@code [main] Parse web
```cpp
Parser parser;
if (!parser.parse(literateFile))
{
  return -1;
}
```

**Tangle output.** The final step is to tangle the file and code blocks and save the output to disk. The logic for doing so will be explained in the *Tangler* class.

@code [main] Tangle output
```cpp
Tangler tangler;
if (!tangler.tangle(parser.getFileBlocks(), parser.getCodeBlocks(),
  outputDirectory))
{
  return -1;
}
```

Include the *Optparse*, *Parser*, and *Tanger* header files.

@code [main] Includes +=
```cpp
#define OPTPARSE_IMPLEMENTATION
#include "Optparse.h"
#include "Parser.h"
#include "Tangler.h"
```

## Entry point

Define the *main()* application entry point which creates an instance of the *Main* class and calls the *run()* function.

@code [main] Application entry point
```cpp
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
```

@code [main] Includes +=
```cpp
#include <iostream>
```
