#!/bin/bash
gcc -g -c -Wall -Werror -fpic -o iterativeFibonacciCompiler.o iterativeFibonacciCompiler.c
gcc -g -shared -o libIterativeFibonacciCompiler.so iterativeFibonacciCompiler.o
gcc -Wall -g -Werror sample.c -ldl
./a.out