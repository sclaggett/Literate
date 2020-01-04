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

