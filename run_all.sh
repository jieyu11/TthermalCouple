#!/bin/sh

make clean;
make
if [ $? != 0 ]; then
  exit
fi

mkdir -p output
./dana -In T_thermal_couples_example.txt
mv *.png result.root output
