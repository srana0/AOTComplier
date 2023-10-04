#include "iterativeFibonacciCompiler.hpp"

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

void func_1(int32_t p,int32_t p1,int32_t p2,int32_t p3,  int32_t* a,int32_t size){
    int32_t i;
    for (i = 0 ;  i-size  <= 0; i++){
        a[i]=0;
    }
    }

void func_2(int32_t p,int32_t p1,int32_t p2,int32_t p3, int32_t size,int32_t* a){
    /* Inspired and adapted from https://www.geeksforgeeks.org/little-and-big-endian-mystery/
     * and https://developer.ibm.com/articles/au-endianc/
     */
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
    a[0] = 0;
    a[1] = 1;
    int i;
    for (i = 2 ;i - size - 1 < 0; i ++){
        a[i] = a[i-1]+a[i-2];
    }

    for (i = 0 ;i - size - 1 < 0; i ++){
        a[i] = changeEndianness(a[i],isBigEndian);
    }
    }