# Main
@title TEMP
@s TEMP

The *Main* class parses the command line and uses the *Parser* and *Tangler* classes to process the web of literate files. This file also includes the *main()* application entry point.

The section below is the header file for the *Main* class. I've choosen to write header files as a single block in this project because they are quite simple. You can easily break more complicated header files up into multiple code blocks so explanations could be interspersed.

@file source/Main.h
```cpp
#pragma once

class Main
{
public:
  Main();
  virtual ~Main();

  uint32_t run(int argc, char** argv);

private:
  std::string outputDirectory;
};
```

A feature of literate programs that is hinted at in the header above is a reduction in the number of functions that need to be defined. It's considered bad practice in other paradigms to have one giant function that does many things because it muddles understanding. However, literate code is broken up and explained in a different fashion which allows you to only define functions when they are actually needed.

A related aspect which will be observed below is an almost complete lack of comments in the code. Comments are generally unnecessary because the code is explained in English. In practice I almost never look at the tangled output but always work at the level of the literate files.

Of course, the lack of extraneous functions and comments is what I've found works best. It isn't a requirement of literate programming itself.

The section below gives an overview of the implementation file. Again, my decision to start each literate file in this project with the header file and an overview of the implementation file is a matter of style--you can just as easily start by explaining various aspects of the class and pull everything together at the end.

@file source/Main.cpp
```cpp
@{[main] Includes}
@{[main] Namespaces}

@{[main] Constructor}
@{[main] Destructor}

@{[main] Run}

@{[main] Application entry point}
```

Notice that each code block name starts with the namespace component `[main]`. This isn't required but is useful because *all code blocks exist in a single shared namespace*. That means you will encounter namespace collisions in larger projects unless you make the code block names unique.

Including the class header file and use the *std* namespace. These items are defined in their own code blocks so additional includes and namespaces can be appended later.

@code [main] Includes
```cpp
#include "Main.h"
```

@code [main] Namespaces
```cpp
using namespace std;
```

## Construction and destruction

The constructor and destructor don't do anything.

@code [main] Constructor
```cpp
Main::Main()
{
}
```

@code [main] Destructor
```cpp
Main::~Main()
{
}
```

## Running

- Steps:
  + Parse arguments:
    * `--help/-h`: Show the help text.
    * `--out-dir/-odir DIR`: Put the generated files in `DIR`.
    * `--version`: Show the version number and compiler information.
  + Handle help and version
  + Parse
  + Tangle

@code [main] Run
```cpp
uint32_t Main::run(int argc, char** argv)
{
  return 0;
}
```

## Entry point

Define the *main()* application entry point which creates an instance of the *Main* class and runs it.

@code [main] Application entry point
```cpp
int main(int argc, char** argv) 
{
  string err;
  try
  {
    Main main;
    return main.run();
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
