#!/bin/bash
g++ -g -c -fPIC -o iterativeFibonacciCompiler.o iterativeFibonacciCompiler.cpp
gcc -g -shared -o libIterativeFibonacciCompiler.so iterativeFibonacciCompiler.o
g++ -g sample.cpp  -ldl
./a.out