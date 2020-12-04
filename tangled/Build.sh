#!/bin/sh
mkdir -p ../build
cd ../build
cmake ../tangled

unameOut="$(uname -s)"
if [[ $unameOut == MINGW* ]]
then
  echo ""
  echo "Open solution in Visual Studio and build from there."
  echo ""
  exit
fi

make
