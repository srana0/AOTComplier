#include <iostream>
#include <stdlib.h>
#include <dlfcn.h>
#include <assert.h>
#include <stdio.h>
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

int main(int argc, char** argv){
    using std::cout;
    using std::cerr;
    void* fileHandle = dlopen("./libIterativeFibonacciCompiler.so",RTLD_LAZY);
    if (!fileHandle) {
        cerr << "Cannot open library: " << dlerror() << '\n';
        return 1;
    }
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
    void (*fun_ptr)(int32_t,int32_t,int32_t,int32_t,int32_t*,int32_t) = (void(*)(int32_t,int32_t,int32_t,int32_t,int32_t*,int32_t))  dlsym(fileHandle,"func_1");
    void (*fun_ptr2)(int32_t,int32_t,int32_t,int32_t,int32_t,int32_t*) = (void(*)(int32_t,int32_t,int32_t,int32_t,int32_t,int32_t*))  dlsym(fileHandle,"func_2");
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


    std::cout<<"All tests have passed" <<std::endl;
    exit(0);
}
