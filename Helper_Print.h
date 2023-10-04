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


#include "Helper_Global.h"

//____________________________________________________________________________________________________________________________
////This will print the Virual Machine
//____________________________________________________________________________________________________________________________
void PrintVirtualMachine(int * JVM,unsigned char * program_Pointer_VM,int execFileLength,unsigned char * constantPool_Pointer_VM,int constantPoolFileLength,int * stackFramePointer_Static)
{

    unsigned char *OS_Karnel_Bottom=(unsigned char *)JVM;
    unsigned char *OS_Kernel_Top=(unsigned char *)JVM+SEGMANETAION_CONSTANT_POOL-1;
    unsigned char *Const_Pool_Top=(unsigned char *)JVM+SEGMENTATION_PROGRAM-1;
    unsigned char *Const_Pool_FileEnd=(unsigned char *)JVM+SEGMANETAION_CONSTANT_POOL+constantPoolFileLength;
    unsigned char *Const_Pool_bottom=(unsigned char *)JVM+SEGMANETAION_CONSTANT_POOL;
    unsigned char *Exec_Top=(unsigned char *)JVM+SEGMENTATION_STACK_FRAME-1;
    unsigned char *Exec_FileEnd=(unsigned char *)JVM+SEGMENTATION_PROGRAM+execFileLength;
    unsigned char *Exec_Bottom=(unsigned char *)JVM+SEGMENTATION_PROGRAM;
   

    printf("\n  VM TOP,Heap Pointer     ****************************************%ld (%p)",(long int)JVM_End,(void *)JVM_End);
    printf("\n                          ***************************************");
    printf("\n                          ***************************************");
    printf("\n                          ***************************************"); 
    printf("\n                          ***************************************");
    printf("\n                          ***************************************");
    printf("\n                          ***************************************");   
    printf("\nStackFrame End,OperandStack****************************************%ld (%p)",(long int)(stackFramePointer_Static+19),(void *)(stackFramePointer_Static+19));
    printf("\n                          ****************************************");
    printf("\n                          ****************************************");
    printf("\n                          ****************************************");
    printf("\nStack Frame Start         ***************************************%ld (%p)",(long int)(stackFramePointer_Static),(void *)stackFramePointer_Static);
    printf("\n                          ****************************************");
    printf("\n                          ****************************************");
    printf("\nProgrm Counter End        ****************************************%ld (%p)",(long int)(Exec_Top),(void *)Exec_Top);
    printf("\n                          ***************************************");
    printf("\n                          ***************************************");
    printf("\n                          ***************************************");
    printf("\nExec File Copy End        ****************************************%ld (%p)",(long int)(Exec_FileEnd), (void *)Exec_FileEnd);
    printf("\n                          ***************************************");
    printf("\n                          ***************************************");
    printf("\nProgram Counter Start     ****************************************%ld (%p)",(long int)(Exec_Bottom), (void *)Exec_Bottom);
    printf("\nConst Pool Pointer End    ****************************************%ld (%p)",(long int)(Const_Pool_Top),(void *)Const_Pool_Top);
    printf("\n                          **************************************");
    printf("\n                          **************************************");
    printf("\n                          **************************************");
    printf("\n                          **************************************");   
    printf("\nConst Pool File End       ****************************************%ld (%p)",(long int)(Const_Pool_FileEnd),(void *) Const_Pool_FileEnd);
    printf("\n                          **************************************");
    printf("\nObject Reference Start    ****************************************%ld(%p)",(long int)(objectReference_In_ConstantPool), (void *) objectReference_In_ConstantPool);
    printf("\n                          **************************************");
    printf("\n                          **************************************");
    printf("\nConst Pool Pointer Start  ****************************************%ld (%p)",(long int)(Const_Pool_bottom),(void *)Const_Pool_bottom);
    printf("\nOS Kernel End             ****************************************%ld (%p)",(long int)(OS_Kernel_Top),(void *)OS_Kernel_Top);
    printf("\n                          **************************************");
    printf("\nOS Kernel Start           ***************************************%ld (%p)\n",(long int)(OS_Karnel_Bottom),(void *)OS_Karnel_Bottom);


}



//____________________________________________________________________________________________________________________________
// This Method should create a Stake Frame and Value Stack on the Top of the Stack
//____________________________________________________________________________________________________________________________
void PrintStackFrame(){
    
    
    // header="Heap , Stack Frame and Operand Stack";
     //Printheader(header);
 
    
    printf("\nVM TOP,Initial Heap Pointer  |\t\t%u\t\t||\t\t%d\t\t| Address: %ld (%p)\n",*(JVM_End),*(JVM_End),(long int)JVM_End,(void *)JVM_End);
    printf("\nVM TOP-1                     |\t\t%u\t\t||\t\t%d\t\t| Address: %ld (%p)\n",*(JVM_End-1),*(JVM_End-1),(long int)(JVM_End-1),(void *)(JVM_End-1));
    printf("\nVM TOP-2                     |\t\t%u\t\t||\t\t%d\t\t| Address: %ld (%p)\n",*(JVM_End-2),*(JVM_End-2),(long int)(JVM_End-2),(void *)(JVM_End-2));
    printf("\nVM TOP-3                     |\t\t%u\t\t||\t\t%d\t\t| Address: %ld (%p)\n",*(JVM_End-3),*(JVM_End-3),(long int)(JVM_End-3),(void *)(JVM_End-3));
    printf("\nVM TOP-4                     |\t\t%u\t\t||\t\t%d\t\t| Address: %ld (%p)\n",*(JVM_End-4),*(JVM_End-4),(long int)(JVM_End-4),(void *)(JVM_End-4));
    printf("\nVM TOP-5                     |\t\t%u\t\t||\t\t%d\t\t| Address: %ld (%p)\n",*(JVM_End-5),*(JVM_End-5),(long int)(JVM_End-5),(void *)(JVM_End-5));
    printf("\nVM TOP-6                     |\t\t%u\t\t||\t\t%d\t\t| Address: %ld (%p)\n",*(JVM_End-6),*(JVM_End-6),(long int)(JVM_End-6),(void *)(JVM_End-6));
    printf("\nVM TOP-7                     |\t\t%u\t\t||\t\t%d\t\t| Address: %ld (%p)\n",*(JVM_End-7),*(JVM_End-7),(long int)(JVM_End-7),(void *)(JVM_End-7));
    printf("\nVM TOP-8                     |\t\t%u\t\t||\t\t%d\t\t| Address: %ld (%p)\n",*(JVM_End-8),*(JVM_End-8),(long int)(JVM_End-8),(void *)(JVM_End-8));
   // printf("\nVM TOP-9                     |\t\t%u\t\t| Address: %ld (%p)\n",*(JVM_End-9),JVM_End-9,JVM_End-9);
    //printf("\nVM TOP-10                    |\t\t%u\t\t| Address: %ld (%p)\n",*(JVM_End-10),JVM_End-10,JVM_End-10);
   // printf("\nVM TOP-11                    |\t\t%u\t\t| Address: %ld (%p)\n",*(JVM_End-11),JVM_End-11,JVM_End-11);
   // printf("\nVM TOP-12                    |\t\t%u\t\t| Address: %ld (%p)\n",*(JVM_End-12),JVM_End-12,JVM_End-12);
   // printf("\nVM TOP-13                    |\t\t%u\t\t| Address: %ld (%p)\n",*(JVM_End-13),JVM_End-13,JVM_End-13);
   // printf("\nVM TOP-14                    |\t\t%u\t\t| Address: %ld (%p)\n",*(JVM_End-14),JVM_End-14,JVM_End-14);
   // printf("\nVM TOP-15                    |\t\t%u\t\t| Address: %ld (%p)\n",*(JVM_End-15),JVM_End-15,JVM_End-15);
    
    printf("\n                             |\t\t\t\t||\t\t\t\t|");
    
    printf("\n** Current HEAP TOP          |\t\t%u\t\t||\t\t%d\t\t| Address: %ld (%p)\n",*(heapPointer_VM),*(heapPointer_VM),(long int)heapPointer_VM,(void *)heapPointer_VM);
    
    printf("\n                             |\t\t\t\t||\t\t\t\t|");
    printf("\n                             |\t\t\t\t||\t\t\t\t|");
    
    printf("\n** Current Operand Stack Top:|\t\t%u\t\t||\t\t%d\t\t|Address: %ld (%p)\n",*(operandStack_Pointer),*(operandStack_Pointer),(long int)operandStack_Pointer,(void *)operandStack_Pointer);
    
    printf("\n                             |\t\t\t\t||\t\t\t\t|");
    printf("\n                             |\t\t\t\t||\t\t\t\t|");   
    
   printf("\nOperand Stack[14]             |\t\t%u\t\t||\t\t%d\t\t| Address: %ld (%p)\n",*(stackFramePointer_Static+34),*(stackFramePointer_Static+34),(long int)(stackFramePointer_Static+34),(void *)(stackFramePointer_Static+34));
   printf("\nOperand Stack[13]             |\t\t%u\t\t||\t\t%d\t\t| Address: %ld (%p)\n",*(stackFramePointer_Static+33),*(stackFramePointer_Static+33),(long int)(stackFramePointer_Static+33),(void *)(stackFramePointer_Static+33));
   printf("\nOperand Stack[12]             |\t\t%u\t\t||\t\t%d\t\t| Address: %ld (%p)\n",*(stackFramePointer_Static+32),*(stackFramePointer_Static+32),(long int)(stackFramePointer_Static+32),(void *)(stackFramePointer_Static+32));
   printf("\nOperand Stack[11]             |\t\t%u\t\t||\t\t%d\t\t| Address: %ld (%p)\n",*(stackFramePointer_Static+31),*(stackFramePointer_Static+31),(long int)(stackFramePointer_Static+31),(void *)(stackFramePointer_Static+31));
   printf("\nOperand Stack[10]             |\t\t%u\t\t||\t\t%d\t\t| Address: %ld (%p)\n",*(stackFramePointer_Static+30),*(stackFramePointer_Static+30),(long int)(stackFramePointer_Static+30),(void *)(stackFramePointer_Static+30));
   printf("\nOperand Stack[9]              |\t\t%u\t\t||\t\t%d\t\t|Address: %ld (%p)\n",*(stackFramePointer_Static+29),*(stackFramePointer_Static+29),(long int)(stackFramePointer_Static+29),(void *)(stackFramePointer_Static+29));
    printf("\nOperand Stack[8]             |\t\t%u\t\t||\t\t%d\t\t| Address: %ld (%p)\n",*(stackFramePointer_Static+28),*(stackFramePointer_Static+28),(long int)(stackFramePointer_Static+28),(void *)(stackFramePointer_Static+28));
    printf("\nOperand Stack[7]             |\t\t%u\t\t||\t\t%d\t\t|Address: %ld (%p)\n",*(stackFramePointer_Static+27),*(stackFramePointer_Static+27),(long int)(stackFramePointer_Static+27),(void *)(stackFramePointer_Static+27));
    printf("\nOperand Stack[6]             |\t\t%u\t\t||\t\t%d\t\t|Address: %ld (%p)\n",*(stackFramePointer_Static+26),*(stackFramePointer_Static+26),(long int)(stackFramePointer_Static+26),(void *)(stackFramePointer_Static+26));
    printf("\nOperand Stack[5]             |\t\t%u\t\t||\t\t%d\t\t| Address: %ld (%p)\n",*(stackFramePointer_Static+25),*(stackFramePointer_Static+25),(long int)(stackFramePointer_Static+25),(void *)(stackFramePointer_Static+25));
    printf("\nOperand Stack[4]             |\t\t%u\t\t||\t\t%d\t\t| Address: %ld (%p)\n",*(stackFramePointer_Static+24),*(stackFramePointer_Static+24),(long int)(stackFramePointer_Static+24),(void *)(stackFramePointer_Static+24));
    printf("\nOperand Stack[3]             |\t\t%u\t\t||\t\t%d\t\t|Address: %ld (%p)\n",*(stackFramePointer_Static+23),*(stackFramePointer_Static+23),(long int)(stackFramePointer_Static+23),(void *)(stackFramePointer_Static+23));
    printf("\nOperand Stack[2]             |\t\t%u\t\t||\t\t%d\t\t| Address: %ld (%p)\n",*(stackFramePointer_Static+22),*(stackFramePointer_Static+22),(long int)(stackFramePointer_Static+22),(void *)(stackFramePointer_Static+22));
    printf("\nOperand Stack[1]             |\t\t%u\t\t||\t\t%d\t\t| Address: %ld (%p)\n",*(stackFramePointer_Static+21),*(stackFramePointer_Static+21),(long int)(stackFramePointer_Static+21),(void *)(stackFramePointer_Static+21)); 
    printf("\nOperand Stack[0]             |\t\t%u\t\t||\t\t%d\t\t| Address: %ld (%p)\n",*(stackFramePointer_Static+20),*(stackFramePointer_Static+20),(long int)(stackFramePointer_Static+20),(void *)(stackFramePointer_Static+20));  
    printf("\nOperand Stack[Top-1]         |\t\t%u\t\t||\t\t%d\t\t|Address: %ld (%p)\n",*(stackFramePointer_Static+19),*(stackFramePointer_Static+19),(long int)(stackFramePointer_Static+19),(void *)(stackFramePointer_Static+19));
    
    printf("\nLocal and Params [15],[Top-1]|\t\t%u\t\t||\t\t%d\t\t| Address: %ld (%p)\n",*(stackFrame_localVariableIndexCounter+15),*(stackFrame_localVariableIndexCounter+15),(long int)(stackFrame_localVariableIndexCounter+15),(void *)(stackFrame_localVariableIndexCounter+15));
    printf("\nLocal and Params [14]        |\t\t%u\t\t||\t\t%d\t\t| Address: %ld (%p)\n",*(stackFrame_localVariableIndexCounter+14),*(stackFrame_localVariableIndexCounter+14),(long int)(stackFrame_localVariableIndexCounter+14),(void *)(stackFrame_localVariableIndexCounter+14)); 
    printf("\nLocal and Params [13]        |\t\t%u\t\t||\t\t%d\t\t| Address: %ld (%p) \n",*(stackFrame_localVariableIndexCounter+13),*(stackFrame_localVariableIndexCounter+13),(long int)(stackFrame_localVariableIndexCounter+13),(void *)(stackFrame_localVariableIndexCounter+13));
    printf("\nLocal and Params [12]        |\t\t%u\t\t||\t\t%d\t\t| Address: %ld (%p) \n",*(stackFrame_localVariableIndexCounter+12),*(stackFrame_localVariableIndexCounter+12),(long int)(stackFrame_localVariableIndexCounter+12),(void *)(stackFrame_localVariableIndexCounter+12));
    printf("\nLocal and Params [11]        |\t\t%u\t\t||\t\t%d\t\t| Address: %ld (%p)\n",*(stackFrame_localVariableIndexCounter+11),*(stackFrame_localVariableIndexCounter+11),(long int)(stackFrame_localVariableIndexCounter+11),(void *)(stackFrame_localVariableIndexCounter+11));
    printf("\nLocal and Params [10]        |\t\t%u\t\t||\t\t%d\t\t| Address: %ld (%p) \n",*(stackFrame_localVariableIndexCounter+10),*(stackFrame_localVariableIndexCounter+10),(long int)(stackFrame_localVariableIndexCounter+10),(void *)(stackFrame_localVariableIndexCounter+10));
    printf("\nLocal and Params [9]         |\t\t%u\t\t||\t\t%d\t\t| Address: %ld (%p) \n",*(stackFrame_localVariableIndexCounter+9),*(stackFrame_localVariableIndexCounter+9),(long int)(stackFrame_localVariableIndexCounter+9),(void *)(stackFrame_localVariableIndexCounter+9));
    printf("\nLocal and Params [8]         |\t\t%u\t\t||\t\t%d\t\t| Address: %ld (%p)\n",*(stackFrame_localVariableIndexCounter+8),*(stackFrame_localVariableIndexCounter+8),(long int)(stackFrame_localVariableIndexCounter+8),(void *)(stackFrame_localVariableIndexCounter+8));
    printf("\nLocal and Params [7]         |\t\t%u\t\t||\t\t%d\t\t| Address: %ld (%p) \n",*(stackFrame_localVariableIndexCounter+7),*(stackFrame_localVariableIndexCounter+7),(long int)(stackFrame_localVariableIndexCounter+7),(void *)(stackFrame_localVariableIndexCounter+7));
    printf("\nLocal and Params [6]         |\t\t%u\t\t||\t\t%d\t\t| Address: %ld (%p) \n",*(stackFrame_localVariableIndexCounter+6),*(stackFrame_localVariableIndexCounter+6),(long int)(stackFrame_localVariableIndexCounter+6),(void *)(stackFrame_localVariableIndexCounter+6));
    printf("\nLocal and Params [5]         |\t\t%u\t\t||\t\t%d\t\t| Address: %ld (%p) \n",*(stackFrame_localVariableIndexCounter+5),*(stackFrame_localVariableIndexCounter+5),(long int)(stackFrame_localVariableIndexCounter+5),(void *)(stackFrame_localVariableIndexCounter+5));
    printf("\nLocal and Params [4]         |\t\t%u\t\t||\t\t%d\t\t| Address: %ld (%p)\n",*(stackFrame_localVariableIndexCounter+4),*(stackFrame_localVariableIndexCounter+4),(long int)(stackFrame_localVariableIndexCounter+4),(void *)(stackFrame_localVariableIndexCounter+4));
    printf("\nLocal and Params [3]         |\t\t%u\t\t||\t\t%d\t\t| Address: %ld (%p)\n",*(stackFrame_localVariableIndexCounter+3),*(stackFrame_localVariableIndexCounter+3),(long int)(stackFrame_localVariableIndexCounter+3),(void *)(stackFrame_localVariableIndexCounter+3));
    printf("\nLocal and Params [2]         |\t\t%u\t\t||\t\t%d\t\t| Address: %ld (%p)\n",*(stackFrame_localVariableIndexCounter+2),*(stackFrame_localVariableIndexCounter+2),(long int)(stackFrame_localVariableIndexCounter+2),(void *)(stackFrame_localVariableIndexCounter+2));
    printf("\nLocal and Params [1]         |\t\t%u\t\t||\t\t%d\t\t| Address: %ld (%p)\n",*(stackFrame_localVariableIndexCounter+1),*(stackFrame_localVariableIndexCounter+1),(long int)(stackFrame_localVariableIndexCounter+1),(void *)(stackFrame_localVariableIndexCounter+1));
    printf("\nLocal and Params [0]         |\t\t%u\t\t||\t\t%d\t\t| Address: %ld (%p)\n",*(stackFrame_localVariableIndexCounter),*(stackFrame_localVariableIndexCounter),(long int)(stackFrame_localVariableIndexCounter),(void *)(stackFrame_localVariableIndexCounter));
    
    printf("\nPrevious Frame Address       |\t\t%u\t\t||\t\t%d\t\t| Address: %ld (%p)\n",*(stackFramePointer_Static+3),*(stackFramePointer_Static+3),(long int)(stackFramePointer_Static+3),(void *)(stackFramePointer_Static+3));
    printf("\nPrevious OperandStackAddress |\t\t%u\t\t||\t\t%d\t\t| Address: %ld (%p)\n",*(stackFramePointer_Static+2),*(stackFramePointer_Static+2),(long int)(stackFramePointer_Static+2),(void *)(stackFramePointer_Static+2));
    printf("\nPrevious Program Counter     |\t\t%u\t\t||\t\t%d\t\t| Address: %ld (%p)\n",*(stackFramePointer_Static+1),*(stackFramePointer_Static+1),(long int)(stackFramePointer_Static+1),(void *)(stackFramePointer_Static+1));
    printf("\nStack Frame ReturnValue      |\t\t%d\t\t||\t\t%d\t\t| Address: %ld (%p)\n",*stackFramePointer_Static,*stackFramePointer_Static,(long int)(stackFramePointer_Static),(void *)(stackFramePointer_Static));
    
   }



//____________________________________________________________________________________________________________________________
// THIS METHOD WILL PRINT THE AOT OUTPUT
//____________________________________________________________________________________________________________________________
void PrintExecutionOutput(double totalExecutionTime)
{
       
   
       printf("\n Total Execution Time ( in seconds) of the application: %lf \n",totalExecutionTime);
        
   

    // Get the Total Number of interpreted and Compiled methods
    uint32_t i=0;
 
    uint32_t method_Identifier=0;
    //int method_indentifer_INT=0;

    uint32_t method_Counter_In_MethodTable_INTERPRETED=0; 
    uint32_t method_Counter_In_MethodTable_COMPILED=0; 
    uint32_t * methodTable_Pointer_AllMethods=(uint32_t *)__MethodTable_Pointer_AllMethods_STATIC;   //ALWAYS START FROM THE STARTRING OF THE MEMORY LOCATION 
    
    if(__TotalNumber_Of_Items_In_AllMethods==0)
    {
        printf("\n No Interpreted or Compiled method found in the program\n");

    }
    else if(__TotalNumber_Of_Items_In_AllMethods>0)
    {
       for(i=0;i<__TotalNumber_Of_Items_In_AllMethods;i++)
        {
            int method_Identifier_INT=(int)(*(methodTable_Pointer_AllMethods+i));   
        
            if(method_Identifier_INT!=-1)
            {
                method_Identifier= *(methodTable_Pointer_AllMethods+i);
                method_Counter_In_MethodTable_INTERPRETED=Search_Number_Of_Methods_In_MethodTable_Interpreter(method_Identifier);
                method_Counter_In_MethodTable_COMPILED=Search_Number_Of_Methods_In_MethodTable_Compiled(method_Identifier);
                printf("\n Method Name: func_%d\tNo of Invokation (Interpreted) : %d\tNo of Invokation (Compiled): %d\n",method_Identifier,method_Counter_In_MethodTable_INTERPRETED,method_Counter_In_MethodTable_COMPILED);
            }   
            
        }    
     
    }
    
}


//____________________________________________________________________________________________________________________________
// THIS METHOD WILL PRINT THE OPCODE
//____________________________________________________________________________________________________________________________
void PrintOpcode(uint32_t opcode,char *opcode_Name)
{
    
    printf("\n OPCODE: %d (0x%x)",opcode,opcode);   
    
    printf(" %s", opcode_Name);
     
}



void Printheader(char *heading)
{   
    printf("\n____________________________________________________________________________________________________________________________\n");    
    printf("%s\n", heading);       
   printf("____________________________________________________________________________________________________________________________\n");
   
}


//____________________________________________________________________________________________________________________________
// HIGHLIGHT PROGRAM BYTE CODE
//____________________________________________________________________________________________________________________________
void Highlight_ProgramByteCode(int indexToHighlight)
{
    int iCount;  
    // Printing the File 
    unsigned char *pointer_to_Program=program_Pointer_VM;
    printf("\n PROGRAM FILE\n");  
    for(iCount=0;iCount<execFileLength;iCount++)
    { 
    unsigned char *PointerToReferencedAddress=(unsigned char *) JVM+indexToHighlight;
    if(pointer_to_Program==PointerToReferencedAddress)
    {         
       
        printf("%hhu\t",*pointer_to_Program);
   
    }
    else
    {
     
        printf("%hhu\t",*pointer_to_Program);
      
    }
        pointer_to_Program++;
    }
    printf("\n\n");
  
 
}




//____________________________________________________________________________________________________________________________
// HIGHLIGHT GOTO
//____________________________________________________________________________________________________________________________
void Highlight_ProgramByteCode_GOTO(unsigned char * PointerToReferencedAddress)
{
    int iCount;  
    // Printing the File 
    unsigned char *pointer_to_Program=program_Pointer_VM;
    printf("\n GO TO\n");  
    for(iCount=0;iCount<execFileLength;iCount++)
    { 
    //unsigned char *PointerToReferencedAddress=(unsigned char *) JVM+indexToHighlight;
    if(pointer_to_Program==PointerToReferencedAddress)
    {         
      
        printf("%hhu\t",*pointer_to_Program);
    
     
    }
    else
    {
     

        printf("%hhu\t",*pointer_to_Program);
    
      
    }
        pointer_to_Program++;
    }
    printf("\n\n");
  
 
}

//____________________________________________________________________________________________________________________________
//  HIGHLIGHT CONSTANT POOL REFERENCE
//____________________________________________________________________________________________________________________________
void Highlight_ConstantPoolReference(unsigned char * referenceToNewMethod)
{
  int iCount;  

 unsigned char *pointer_to_Program=constantPool_Pointer_VM;
 printf("\n CONSTANT POOL FILE\n");  
for(iCount=0;iCount<constantPoolFileLength;iCount++)
{ 
   //unsigned char *PointerToReferencedAddress=(unsigned char *) JVM+indexToHighlight;
   if(pointer_to_Program==referenceToNewMethod)
   {
         
         printf("%hhu\t",*pointer_to_Program);          
      
   }
   else
   {     
        
     printf("%hhu\t",*pointer_to_Program);   
    
   } 
   pointer_to_Program++;
}
 printf("\n\n");

 
}



void ShowMemoryAlloctionInHeap(int * referenceToMemory,int size)
{
        int i=0;
         for(i=0;i<size;i++)
         {
               printf("\n array[%d]   Value : %d address : %p  \n",i,*(referenceToMemory-i),(referenceToMemory-i));
                
         }

}
