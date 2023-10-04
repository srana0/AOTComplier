#!/bin/bash
g++ -c -o incrementByTwoCpp.o incrementByTwo.cpp
gcc -shared -o libIncrementByTwoCpp.so incrementByTwoCpp.o
g++ sample.cpp  -ldl
./a.out