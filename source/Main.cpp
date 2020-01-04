#include "Main.h"
#include <iostream>

using namespace std;


Main::Main()
{
}

Main::~Main()
{
}


uint32_t Main::run(int argc, char** argv)
{
  return 0;
}


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


