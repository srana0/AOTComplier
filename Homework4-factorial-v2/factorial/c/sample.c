#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <assert.h>

int main (int argc, char** argv){
    void* fileHandle = dlopen("./libFactorialForCompiler.so",RTLD_LAZY);
    if (fileHandle == NULL)
        {
            printf("Could not find the library\n");
            exit(-1);
        }
    int (*fun_ptr)() =  dlsym(fileHandle,"func_1");
    int (*fun_ptr2)() =   dlsym(fileHandle,"func_2");
    assert(1==fun_ptr(0,0,0,0,1));
    assert(2==fun_ptr(0,0,0,0,2));
    assert(6==fun_ptr(0,0,0,0,3));
    assert(24==fun_ptr(0,0,0,0,4));
    assert(120==fun_ptr(0,0,0,0,5));
    assert(720==fun_ptr(0,0,0,0,6));
    assert(5040==fun_ptr(0,0,0,0,7));
    assert(40320==fun_ptr(0,0,0,0,8));
    assert(362880==fun_ptr(0,0,0,0,9));
    assert(3628800==fun_ptr(0,0,0,0,10));
    assert(39916800==fun_ptr(0,0,0,0,11));
    assert(fun_ptr2(0,0,0,0,1)==fun_ptr(0,0,0,0,1));
    assert(fun_ptr2(0,0,0,0,2)==fun_ptr(0,0,0,0,2));
    assert(fun_ptr2(0,0,0,0,3)==fun_ptr(0,0,0,0,3));
    assert(fun_ptr2(0,0,0,0,4)==fun_ptr(0,0,0,0,4));
    assert(fun_ptr2(0,0,0,0,5)==fun_ptr(0,0,0,0,5));
    assert(fun_ptr2(0,0,0,0,6)==fun_ptr(0,0,0,0,6));
    assert(fun_ptr2(0,0,0,0,7)==fun_ptr(0,0,0,0,7));
    assert(fun_ptr2(0,0,0,0,8)==fun_ptr(0,0,0,0,8));
    assert(fun_ptr2(0,0,0,0,9)==fun_ptr(0,0,0,0,9));
    assert(fun_ptr2(0,0,0,0,10)==fun_ptr(0,0,0,0,10));
    assert(fun_ptr2(0,0,0,0,11)==fun_ptr(0,0,0,0,11));

    printf("All tests passed \n");
    return 0;
}
