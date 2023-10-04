#!/bin/bash
g++ -c -o returnThreeCpp.o returnThree.cpp
gcc -shared -o libReturnThreeCpp.so returnThreeCpp.o
g++ sample.cpp  -ldl
./a.out