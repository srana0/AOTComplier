#include <iostream>
#include <stdlib.h>
#include <dlfcn.h>
#include <assert.h>

int main(int argc, char** argv){
    using std::cout;
    using std::cerr;
    void* fileHandle = dlopen("./libReturnThreeCpp.so",RTLD_LAZY);
    if (!fileHandle) {
        cerr << "Cannot open library: " << dlerror() << '\n';
        return 1;
    }
    int (*fun_ptr)(int) = (int(*)(int))  dlsym(fileHandle,"func_1");
    assert(3==(fun_ptr(0)));
    std::cout<<"Hello world, three equals "<< (fun_ptr(0)) <<std::endl;
    exit(0);
}
