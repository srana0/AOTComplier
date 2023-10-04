#include "factorialForCompiler.hpp"

int func_1(int p,int p1,int p2,int p3,int a){
    if (a<2) return 1;
    return func_1(p,p1,p2,p3,a-1)*a;
    }

int func_2(int p,int p1,int p2,int p3,int a){
    int result = 1;
    if (a<2)
        result = 1;
    int i; 
    for (i = 2 ; i <= a; i++) 
        result = result * i; 
    return result;
    }