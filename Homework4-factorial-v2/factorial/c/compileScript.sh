#!/bin/bash
gcc -c -Wall -Werror -fpic -o factorialForCompiler.o factorialForCompiler.c
gcc -shared -o libFactorialForCompiler.so factorialForCompiler.o
gcc -Wall -Werror sample.c -ldl
./a.out