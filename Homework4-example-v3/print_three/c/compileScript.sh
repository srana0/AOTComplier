#!/bin/bash
gcc -c -Wall -Werror -fpic returnThree.c
gcc -shared -o libReturnThree.so returnThree.o
gcc sample.c -ldl
./a.out