#!/bin/bash

TOP=$PWD

for i in 01 02 03 04 05 07
do
  cd $TOP/$i
  ./a.out
done
