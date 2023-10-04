#!/bin/bash
g++ -c -fPIC -o factorialForCompiler.o factorialForCompiler.cpp
gcc -shared -o libFactorialForCompilerCpp.so factorialForCompiler.o
g++ sample.cpp  -ldl
./a.out