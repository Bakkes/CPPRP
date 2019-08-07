#!/bin/bash
rm *.o 
rm ./main
clang++-6.0 -std=c++17 -Wall -fPIC -c *.cpp -Ofast
clang++-6.0 -std=c++17 *.o -o main -pthread -lm 
echo "w0t"
