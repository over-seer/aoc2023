#!/bin/bash

TOP=$PWD

for i in 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15
do
    cd $TOP/$i
    g++ main.cpp -O3 -std=c++23 -o a.gnu
    clang++ main.cpp -O3 -std=c++2b -o a.clang
done
