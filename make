#!/bin/sh
cd ./bin
rm a.out
cc ../src/main.c ../src/airline.c ../src/plane.c ../src/map.c ../src/common.c
./a.out
