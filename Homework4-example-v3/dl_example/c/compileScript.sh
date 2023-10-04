#!/bin/bash
gcc -c -Wall -Werror -fpic incrementByTwo.c
gcc -shared -o libIncrementByTwo.so incrementByTwo.o
gcc sample.c -ldl
./a.out