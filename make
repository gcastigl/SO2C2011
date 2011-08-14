#!/bin/sh
cd ./bin
rm a.out
gcc -Wall ../src/main.c ../src/airline.c ../src/plane.c ../src/map.c ../src/common.c
./a.out
