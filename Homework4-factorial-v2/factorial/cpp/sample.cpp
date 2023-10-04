#include <iostream>
#include <stdlib.h>
#include <dlfcn.h>
#include <assert.h>

int main(int argc, char** argv){
    using std::cout;
    using std::cerr;
    void* fileHandle = dlopen("./libFactorialForCompilerCpp.so",RTLD_LAZY);
    if (!fileHandle) {
        cerr << "Cannot open library: " << dlerror() << '\n';
        return 1;
    }
    int (*fun_ptr)(int,int,int,int,int) = (int(*)(int,int,int,int,int))  dlsym(fileHandle,"func_1");
    int (*fun_ptr2)(int,int,int,int,int) = (int(*)(int,int,int,int,int))  dlsym(fileHandle,"func_2");
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

    std::cout<<"All tests have passed" <<std::endl;
    exit(0);
}
