#!/bin/sh

cd ..
mkdir -p build
cd build

g++ -std=c++11 ../code/meditation_game.cpp -lSDL2 -lSDL2_image -o meditation_game