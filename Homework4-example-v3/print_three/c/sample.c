#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <assert.h>

int main (int argc, char** argv){
    void* fileHandle = dlopen("./libReturnThree.so",RTLD_LAZY);
    if (fileHandle == NULL)
        {
            printf("Could not find the library\n");
            exit(-1);
        }
    int (*fun_ptr)() =   dlsym(fileHandle,"func_1");
    assert(3==fun_ptr(0));
    printf("Hello World, three equals %i \n",(fun_ptr(0)));
    return 0;
}
