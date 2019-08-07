#!/bin/bash
rm *.o 
rm ./main
g++-9 -std=c++17 -Wall -fPIC -c *.cpp -Ofast
g++-9 -std=c++17 *.o -o main -pthread -lm 
echo "w0t"
