#ifndef _MY_HEADER_H_
#define _MY_HEADER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h> 
#include <stdbool.h>
#include <arpa/inet.h>
#include <dlfcn.h>
#include <libintl.h>
#include <time.h>
#include <assert.h>


#define CAPACITY_ALL_METHODS 80 // Size of the Hash Table

void *handle;
// GLOBAL VARIABLE FOR ROOT SET
uint32_t * __MethodTable_Pointer_AllMethods_STATIC=NULL;                // This will points to the method Table , it will always points to the starting of the ClassPool
uint32_t * __MethodTable_Pointer_AllMethods_DYNAMIC=NULL;              // This will points to the method Table, it will always points to the current free space of ClassPool
uint32_t __TotalNumber_Of_Items_In_AllMethods=0;             // Counter to keep track of number of items in the table
int IsBigEndian();
int32_t changeEndianness (uint32_t i, uint32_t isBigEndian);
int GetEndianness();
bool isFunc2_Of_FibbonacciArray_Invoked=false;

   time_t __start_Time, __end_Time;
   double __diff_Time;

    #define SEGMENTATION_PROGRAM 1024
    #define SEGMANETAION_CONSTANT_POOL 256
    #define SEGMENTATION_OBJECT_REF_CONST_POOL 768
    #define SEGMENTATION_STACK_FRAME 1048576
    #define MAX_VM_SIZE 4294967295 // Pow(2,32)-1

    char libraryName_Of_CompiledCode[1000]="";
    char functionName_Of_CompiledCode[1000]="";
    //char *compiled_FunctionName=NULL;
        
     int __parameter_From_Main=0;                                  // THIS IS THE PARAM SENT FROM MAIN METHOD
    uint32_t __methodThreshold=0;                                // THS DENOTES THE THRESHOLD VALUE OF THE NUMBER OF INVOKE STACK METHOD

    // Declaration of Pointers
     int *JVM=NULL;                                                // This pointer will point to the starting address of the entire VM 
     int *JVM_End=NULL;                                            // This pointer will point to the end of the VM
     int *heapPointer_VM;                                          // This will initially point to the JVM_End and eventually will grow downwards
     unsigned char *program_Pointer_VM=NULL;                         // This pointer should starts from memery 1024 in the VM     
    
     
     unsigned char *constantPool_Pointer_VM=NULL;                    // This pointer should start from memory 256 in the VM
     unsigned char *objectReference_In_ConstantPool=NULL;            // This pointer will point to the Object Reference in Constant Pool at 768 
     unsigned char *stackFrame_Pointer_VM=NULL;                      // This pointer will point to  Stack Frame in VM     
     unsigned char *interpreter_ProgramCounter=NULL;                 // This poinrer will same as program_Pointer_VM but it is unsigned char pointer
     unsigned char *library_ProgramCounter=NULL;                     // This pointer will points to the compiled file library
     
     int *operandStack_Pointer=NULL;                               // This pointer will point to the Operand Stack
     int *stackFrame_localVariableIndexCounter=NULL;               // This pointer will point to the Local and parameters inside the Stack Frame
     int *stackFramePointer_Dynamic=NULL;                          // This pointer will be dynamic and will increase to the end of the Stack Frame
     int *stackFramePointer_Static=NULL;                           // This pointer will be static and will always point to the start of the stack frame
     int valueOfParameter;         
     int *argumentPointer_InvokeStatic=NULL;                       // This pointer will hold the value passing from the main method as arg---TODO 
     int *stackFrame_Pointer_In_MainMethod=NULL;                   // This pointer will decide whether the function finished executing the all operation and return     
     
    char *header; 
    int execFileLength;                                           // Length of the Program File
    int constantPoolFileLength;                                   // Length of the Constant Pool File
    int libraryFileLength;                                        // Length of the compiled library
  

   const char * dynamicLibrary_Pointer=NULL;
    //char * dynamicLibrary_Pointer=NULL;
    void *handle;
    void (*func_print_name)(const char*);


 #endif //#ifndef VMCLASS_INCREMENT_BY_TWO_H___