#include <iostream>
#include <stdlib.h>
#include <dlfcn.h>
#include <assert.h>

int main(int argc, char** argv){
    using std::cout;
    using std::cerr;
    void* fileHandle = dlopen("./libIncrementByTwoCpp.so",RTLD_LAZY);
    if (!fileHandle) {
        cerr << "Cannot open library: " << dlerror() << '\n';
        return 1;
    }
    int (*fun_ptr)(int) = (int(*)(int))  dlsym(fileHandle,"returnAPlusTwo");
    assert(5==(fun_ptr(3)));
    std::cout<<"Hello world, Five equals "<< (fun_ptr(3)) <<std::endl;
    exit(0);
}
