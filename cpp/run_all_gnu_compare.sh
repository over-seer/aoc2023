#!/bin/bash

TOP=$PWD
#TIMEFORMAT=%U
TIMEFORMAT=%E
for i in 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19
do
  cd $TOP/$i
  time ./a.gnu
done