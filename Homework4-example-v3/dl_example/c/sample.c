#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <assert.h>

int main (int argc, char** argv){
    void* fileHandle = dlopen("./libIncrementByTwo.so",RTLD_LAZY);
    if (fileHandle == NULL)
        {
            printf("Could not find the library\n");
            exit(-1);
        }
    int (*fun_ptr)() =   dlsym(fileHandle,"returnAPlusTwo");
    assert(5==fun_ptr(3));
    printf("Hello World, five equals %i \n",(fun_ptr(3)));
    return 0;
}
