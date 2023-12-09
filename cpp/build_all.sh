#!/bin/bash

TOP=$PWD

for i in 01 02 03 04 05 07 08 09
do
    cd $TOP/$i
    #g++ main.cpp -O3 -std=c++23
    clang++ main.cpp -O3 -std=c++2b
done
