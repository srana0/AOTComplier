#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
int32_t changeEndianness (uint32_t i, uint32_t isBigEndian) {
    /* Copied and adapted from https://developer.ibm.com/articles/au-endianc/
     * isBigEndian equal to zero (0) denotes    big endian
     * isBigEndian equal to  one (1) denotes little endian
     */ 
    unsigned char c1, c2, c3, c4;

    if (isBigEndian == 0) {
        return i;
    } else {
        c1 = i & 255;
        c2 = (i >> 8) & 255;
        c3 = (i >> 16) & 255;
        c4 = (i >> 24) & 255;

        return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
    }
}
void fibonacci(int32_t size,int32_t* a){
    a[0] = 0;
    a[1] = 1;
    int i;
    for (i = 2 ;i - size - 1 < 0; i ++){
        a[i] = a[i-1]+a[i-2];
    }
}

int main (int argc, char** argv){
    void* fileHandle = dlopen("./libIterativeFibonacciCompiler.so",RTLD_LAZY);
    uint32_t aHexint = 1;
    char* test = (char*) &aHexint;
    int32_t isBigEndian = -1; // If machine IS big endian, will contain 0; otherwise 1;
    if (test[0]==1)
        {
            isBigEndian = 1;
        }
    else 
        {
            isBigEndian = 0;
        }
    if (fileHandle == NULL)
        {
            printf("Could not find the library\n");
            exit(-1);
        }
    int (*fun_ptr)() =  dlsym(fileHandle,"func_1");
    int (*fun_ptr2)() =   dlsym(fileHandle,"func_2");
    int oft = 66 ;
    int pft = oft * 125;
    int32_t* array = (int*) malloc(sizeof(int32_t)* pft);
    int32_t* array2 = (int*) malloc(sizeof(int32_t)* pft);
    for (int i = 0 ; i - pft <= 0 ; i++){
        fun_ptr(0,0,0,0,array,45);
        fun_ptr(0,0,0,0,array2,45);
        for (int j = 0 ; j - 45 < 0; j++){
            fun_ptr2(0,0,0,0,j,array);
            fibonacci(j,array2);
        }
    }
    for (int i = 0 ; i - 46 < 0 ; i++)
        assert(array2[i]==changeEndianness(array[i],isBigEndian));
    for (int i = 0; i - 46 < 0; i++){
        printf("%i\n",changeEndianness(array[i],isBigEndian));
    }

    printf("All tests passed \n");
    return 0;
}
