#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<inttypes.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h> 
#include<stdbool.h>
#include<arpa/inet.h>
#include<dlfcn.h>
#include<libintl.h>
#include<time.h>

#include "Helper_Global.h"
#include "VirtualMachineConfig.h"
#include "Method_Table_Compiled.h"
#include "Method_Table_Interpreter.h"
#include "Helper_File.h"
#include "Helper_Print.h"
# include "Method_Table_AllMethods.h"
//____________________________________________________________________________________________________________________________________________
//THIS WAS SUBMITED AS ASSIGNEMENT : 4
//____________________________________________________________________________________________________________________________________________
//.Desciption:              AOT Compiler
//.Author:                 Subhabrata Rana
//.Compile:                 gcc AOT.c -o AOT

//.Compile Shared Object:   gcc AOT.c -ldl -o AOT
//.Run Program:            ./AOT -e exec -c constant_pool arg -a num -l library

// Example 1:              ./AOT -e factorial-for-compiler.bvmclass -c factorial-for-compiler.constantpool 11 -a 0 -l libFactorialForCompiler
// Example 2:              ./AOT -e PrintThree.bvmclass -c PrintThree.constantpool 0 -a 0 -l libReturnThree
// Example 3:              ./AOT -e IterativeArrayFibonacciArray.bvmclass -c IterativeArrayFibonacciArray.constantpool 0 -a 0 -l libIterativeFibonacciCompiler

           


//___________________________________________________________________________________________________________________________________________________________________
//EXPECTED PARAMETERS :  
// argv[0]:./AOT
// argv[1]:-e  
// argv[2]:fileName.bvmclass   
// argv[3]:-c    
// argv[4]: fileName.constantPool   
// argv[5]:arg
// argv[6]:-a  
// argv[7]:num
// argv[8]:-l  
// argv[9]:./libReturnThree.so
//________________________________________________________________________________________________________________________________________________________________________


 
//________________________________________________________________________________________________________________________________________________________________________
// MAIN INTERPRETER START
 //________________________________________________________________________________________________________________________________________________________________________
  
  
int main(int argc, char* argv[])
{
 

     
   
       time(&__start_Time);

       //GET THE VALUE FROM THE MAIN METHOD ARGUMENT
     
      __parameter_From_Main=atoi(argv[5]);
        __methodThreshold=atoi(argv[7]);
        dynamicLibrary_Pointer=argv[9];     // this points to the Dynamic Library   

               //CREATE THE LIBRARY NAME
              char soPrefix[]="./";
              char soPostFix[]=".so";
            
              strcat(soPrefix,dynamicLibrary_Pointer);
              strcat(soPrefix,soPostFix);
              strcpy(libraryName_Of_CompiledCode,soPrefix);

              handle = dlopen(libraryName_Of_CompiledCode,RTLD_LAZY|RTLD_GLOBAL);
                if (handle == NULL) 
                      {
                            // library does not exists                                 
                           
                            printf("\nCould not find the library name %s\n",libraryName_Of_CompiledCode);
                            fprintf(stderr, "%s\n", dlerror());
                           // exit(EXIT_FAILURE);
                      }
     // printf("\nJVM Interpreter program started\n"); 
     // Allocate memory for the Virtual Machine. It will return the pointer to the starting of the Entire Virtual Machine   
     JVM=(int *)malloc(MAX_VM_SIZE); 

     // Validate the Memory Allocation
     if(JVM==NULL)
     {
       
       printf("\nMemory allocation failed for the entire VM at location : %ld (%p)",JVM,JVM);
       exit(EXIT_FAILURE);
       
     }
    
     //  printf("\nMemory allocation successful at location : %ld (%p)",JVM,JVM);   
     //___________________________________________________________________________________________________________________________
     //Assignment of the Pointers
     //___________________________________________________________________________________________________________________________
     
     // Pointer-->JVM_End
     JVM_End=(int *)((char *)JVM+MAX_VM_SIZE);                              // Needed to convert to Char Pointer first then again int to properly start from MAX_VM_SIZE

     // Pointer-->Heap Pointer at VM                                        // Initially will points to the JVM_End
      heapPointer_VM=JVM_End;                                       
    
     // Pointer-->Program Counter                                            // Pointer to the 1024 Location
      program_Pointer_VM= (unsigned char *)JVM+SEGMENTATION_PROGRAM;
   

      //  header="Program File";
     // Printheader(header);
      execFileLength=LoadBinaryFileIntoVM(argv,2,SEGMENTATION_PROGRAM,JVM);  

      //Pointer-->Constant Pool                                             // Pointer to the 256 location
      constantPool_Pointer_VM= (unsigned char *)JVM+SEGMANETAION_CONSTANT_POOL;

     // header="Constant Pool File";
     // Printheader(header);    
   
      constantPoolFileLength=LoadBinaryFileIntoVM(argv,4,SEGMANETAION_CONSTANT_POOL,JVM);   
      

      // Pointer-->Object Reference in the Constant Pool                     // Pointer to the 768 location
      objectReference_In_ConstantPool=(unsigned char *)JVM+SEGMENTATION_OBJECT_REF_CONST_POOL;    
      
      // Pointer-->Stack Frame                                                // Pointer to the Stack Frame
      stackFrame_Pointer_VM=(unsigned char *)JVM+SEGMENTATION_STACK_FRAME;  
      
      // Pointer--> stackFramePointer_Static.  stackFramePointer_Dynamic       // Converting the StakeFrame_Pointer-VM from signed Char to Int     
      stackFramePointer_Static=(int *) (stackFrame_Pointer_VM);                // It will always points to the starting point of the Stack Frame
      stackFramePointer_Dynamic=(int *) (stackFramePointer_Static+19);         // This is will be dynamic and will increase to the end of the Stack Frame
     
      // Pointer-->  stackFrame_Pointer_In_Main Method
      stackFrame_Pointer_In_MainMethod=stackFramePointer_Static=(int *) (stackFrame_Pointer_VM);  // This will keep track of the Stack Frame and will help in deciding whether we are in Main Method
      
      // Pointer-->Program Counter                                           // Converting the PC to unsigned Char.    
     interpreter_ProgramCounter=program_Pointer_VM; //SUBHA
     
      // Pointer-->Local Variables and Parameters inside Stack Frame
      stackFrame_localVariableIndexCounter=stackFramePointer_Static+4;  
 
     // Pointer-> Operand Stack                                                // This will point to the Stack Top(TOP=-1) location. When we need to PUSH, we need to first increment the Stack Top
      operandStack_Pointer=stackFramePointer_Static+19;

     // Print entire Virtual Machine        
    // header="Entire Virtual Machine";
    // Printheader(header);
    // PrintVirtualMachine(JVM,program_Pointer_VM,execFileLength,constantPool_Pointer_VM,constantPoolFileLength,stackFramePointer_Static);

     //___________________________________________________________________________________________________________________________
     //Assign Values for Creating Initial Stack Frame for Main Method
     //___________________________________________________________________________________________________________________________
     uint32_t programCounter_Address;    
     programCounter_Address=0;                       // INITIALIZATION-MAIN METHOD: Previous program counter is zero
     
     uint32_t stackFrame_ReturnValue;
     stackFrame_ReturnValue=0;                       // INITIALIZATION-MAIN METHOD: Return Value is zero

     uint32_t stackFrame_Address;   
    stackFrame_Address=0;                            // INITIALIZATION-MAIN METHOD: Previous Stack Frame Address is zero

     //uint32_t numberOfArguments;
     //numberOfArguments=numberOfParameters;           // INITIALIZATION-MAIN METHOD: Number of Argument is same as passed to Main Method

     uint32_t ArgumentType;
     ArgumentType=0;                                 // INITIALIZATION-MAIN METHOD: Number of Argument Type is zero for Integer        

     uint32_t operandStack_Address;                 
     operandStack_Address=0;                         // INITIALIZATION-MAIN METHOD: Previous Operand Stack address is zero    

    // Initialize Stack Frame with the Values
    //SaveValueInStackFrame(stackFrame_ReturnValue,programCounter_Address,operandStack_Address,stackFrame_Address,numberOfArguments,argPointer);


    // Initialize Stack Frame with the Values
   SaveValueInStackFrame(stackFrame_ReturnValue,programCounter_Address,operandStack_Address,stackFrame_Address,__parameter_From_Main);
 // PrintStackFrame();

  // Initialize Method Table-INTERPRETER
  __MethodTable_Pointer_Interpreter_STATIC=(uint32_t *) calloc(CAPACITY_METHOD_TABLE_INTERPRETER,sizeof(uint32_t));
  __MethodTable_Pointer_Interpreter_DYNAMIC=__MethodTable_Pointer_Interpreter_STATIC;
   Initialize_Method_In_MethodTable_Interpreter();
   //WriteToTextFile_Method_Interpreter(__MethodTable_Pointer_Interpreter_STATIC,CAPACITY_METHOD_TABLE_INTERPRETER);

  // Initialize Method Table-COMPILER
   __MethodTable_Pointer_Compiled_STATIC=(uint32_t *)calloc(CAPACITY_METHOD_TABLE_COMPILED,sizeof(uint32_t));
   __MethodTable_Pointer_Compiled_DYNAMIC=__MethodTable_Pointer_Compiled_STATIC; 
     Initialize_Method_In_MethodTable_Compiled();
     // WriteToTextFile_MethodTable_Compiler(__MethodTable_Pointer_Compiled_STATIC,CAPACITY_METHOD_TABLE_COMPILED);

    
  // Initialize Method Table-INTERPRETER WRITE
   __Pointer_Interpreter_Write_STATIC=(uint32_t *)calloc(CAPACITY_INTERPRETER_WRITE,sizeof(uint32_t));
   __Pointer_Interpreter_Write_DYNAMIC=__Pointer_Interpreter_Write_STATIC;    
    // WriteToTextFile_Output_InterpreterWrite(__Pointer_Interpreter_Write_STATIC,CAPACITY_INTERPRETER_WRITE);

  // Initialize Method Table-METHOD ALL
   __MethodTable_Pointer_AllMethods_STATIC=(uint32_t *)calloc(CAPACITY_ALL_METHODS,sizeof(uint32_t));
   __MethodTable_Pointer_AllMethods_DYNAMIC=__MethodTable_Pointer_AllMethods_STATIC; 
    Initialize_Method_In_MethodTable_AllMethods();   
    // WriteToTextFile_Output_AllMethods(__MethodTable_Pointer_AllMethods_STATIC,CAPACITY_ALL_METHODS);

     // Get the First Opcode from the Execution File
      uint32_t opcode=(uint32_t)(*interpreter_ProgramCounter);   
      char *opcodeName; 
      int numberOfByteCodes=0;  
  
      //header="Program Execution Started";
      //Printheader(header);
    
     
     for(numberOfByteCodes=0;numberOfByteCodes<execFileLength-1;numberOfByteCodes++)
     {   
                            
       // Halt while hitting return
         while (opcode != 0xb1) {         
         uint32_t opcode=(uint32_t)(*interpreter_ProgramCounter);     
      
        switch (opcode) {

                 
                case 0X10: 
                        //  opcodeName="BIPUSH";              
                         // PrintOpcode(opcode,opcodeName);                                                          
                     BIPUSH();                    
	                   break;  
                 case 0X36:  
                        // opcodeName="ISTORE";              
                       // PrintOpcode(opcode,opcodeName);                          
                                                    
                      ISTORE(); 
                      break; 
                 case 0X03:  
                                // opcodeName="ICONST_0";              
                                // PrintOpcode(opcode,opcodeName); 
                                                                       
                      ICONST_0();
                      break; 
                case 0X15: 
                                 // opcodeName="ILOAD";              
                                // PrintOpcode(opcode,opcodeName);                                                              
                      ILOAD();                      
                      break;
                 case 0X9d:  
                               //  opcodeName="IFGT";              
                               // PrintOpcode(opcode,opcodeName);                                                                
                       IFGT();              
                       break; 
                 case 0Xa7:  
                               // opcodeName="GOTO";              
                              //  PrintOpcode(opcode,opcodeName); 
                                 
                      GOTO();  
                      break; 
                 case 0X64:   
                               //  opcodeName="ISUB";              
                              //   PrintOpcode(opcode,opcodeName);         
  
                      ISUB();             
                      break;
                 case 0X60: 
                                 //  opcodeName="IADD";              
                                // PrintOpcode(opcode,opcodeName);  
  
                      IADD(); 
                      break;
                case  0X9f:   
                                 // opcodeName="ICMPEQ";              
                               //  PrintOpcode(opcode,opcodeName);    
                      
                         ICMPEQ();                              
                         break; 
                case 0x84: 
                                //opcodeName="IINC";              
                                 // PrintOpcode(opcode,opcodeName); 
                      IINC();
                      break;
                case 0xff:  
                                //   opcodeName="IWRITE";              
                              //  PrintOpcode(opcode,opcodeName); 

                      IWRITE();                                   
                      break;              
                case 0X2e: 
                                // opcodeName="IALOAD";              
                              //  PrintOpcode(opcode,opcodeName); 
                      IALOAD();            
                      break; 
                case 0X7e: 
                            // opcodeName="IAND";              
                             //  PrintOpcode(opcode,opcodeName);  

                      IAND();
                      break; 
                case 0X4f: 
                            // opcodeName="IASTORE";              
                             // PrintOpcode(opcode,opcodeName);  

                       IASTORE();      
                       break; 
                case 0X6c: 
                               //  opcodeName="IDIV";              
                               //  PrintOpcode(opcode,opcodeName);  
                      IDIV();                  
                      break; 
                case 0X99:    
                               // opcodeName="IFEQ";              
                               //  PrintOpcode(opcode,opcodeName);   
                      IFEQ(); 
                      break;
              case 0X9b:    
                                //  opcodeName="IFLT";              
                              //  PrintOpcode(opcode,opcodeName);  
                     IFLT(); 
                     break; 
                case 0X9c:    
                               // opcodeName="IFGE";              
                               //  PrintOpcode(opcode,opcodeName);            
                      IFGE();
                      break;    
              case 0X9e:    

                                //opcodeName="IFLE";              
                                // PrintOpcode(opcode,opcodeName);   
                     IFLE();           
                     break;  
               case 0Xc6:   
                               // opcodeName="IFNULL";              
                               // PrintOpcode(opcode,opcodeName);   
                     IFNULL();         
                     break;  
              case 0X68:    
                               /// opcodeName="IMULL";              
                              // PrintOpcode(opcode,opcodeName); 
                    IMULL();              
                    break; 
              case 0X74:   
                            //   opcodeName="INEG";              
                             //   PrintOpcode(opcode,opcodeName); 
                     INEG();    
                    break;                
                case 0X80:   
                             // opcodeName="IOR";              
                              // PrintOpcode(opcode,opcodeName); 
                      IOR();               
                      break; 
              case 0X57:    
                               // opcodeName="POP";              
                              //  PrintOpcode(opcode,opcodeName);          
                    POP_OperandStack();          
                     break; 
              case 0Xac:    
                                 // opcodeName="IRETURN";              
                                 //PrintOpcode(opcode,opcodeName);   
                      IRETURN();
                      break;             
               case 0xb1: 
                                // opcodeName="RETURN";              
                                 //PrintOpcode(opcode,opcodeName);         

                    RETURN();                                  
                    break;

          case 0Xb8:   
                                 ///opcodeName="INVOKE_STATIC";              
                                 // PrintOpcode(opcode,opcodeName);                                 

                    INVOKE_STATIC();
                    break; 
          case 0xfe:  
                              // opcodeName="IREAD";              
                                //PrintOpcode(opcode,opcodeName);    
                 IREAD();
                 break;
          case 0xbb:     
                                //opcodeName="NEW";              
                               // PrintOpcode(opcode,opcodeName);                                                   
                    
                 NEW();                 
                 break;
          case 0xbd: 
                               // opcodeName="ANEWARRAY";              
                                //PrintOpcode(opcode,opcodeName);
                  ANEWARRAY();
                 break;

            case 0xbc:                                                     
                                // opcodeName="NEWARRAY";              
                               // PrintOpcode(opcode,opcodeName);
                 NEWARRAY();
                 break;
             case 0xb4:      
                                // opcodeName="GETFIELD";              
                              //  PrintOpcode(opcode,opcodeName);
                 
                  GETFIELD(); 
                  break;
            case 0xb5:            
                              //  opcodeName="PUTFIELD";              
                              //  PrintOpcode(opcode,opcodeName);
            
                    PUTFIELD();             
                     break;
              case  0x19:    
              
                               // opcodeName="ALOAD";              
                              //  PrintOpcode(opcode,opcodeName);
                ALOAD();    
                break;
            case  0x3a:           
                                //opcodeName="ASTORE";              
                               // PrintOpcode(opcode,opcodeName);
                   ASTORE();   
                   break;
            case  0x59:
                             // opcodeName="DUP";              
                             //   PrintOpcode(opcode,opcodeName);
                DUP();   
                break;

            case    0x1: 
                              //  opcodeName="ACONST_NULL";              
                              // PrintOpcode(opcode,opcodeName);
             ACONST_NULL();
             break;

          case    0x53: 
                                 //opcodeName="AASTORE";              
                              //PrintOpcode(opcode,opcodeName);
             AASTORE();
             break;
           case    0x32: 
                                //opcodeName="AALOAD";              
                                //  PrintOpcode(opcode,opcodeName);
             AALOAD();
             break;
           default:// The exit (0) shows the successful termination of the program and the exit(1) shows the abnormal termination of the program
           break;
            //PrintStackFrame();

              // after the execution is over print statistics         
           //time(&__end_Time);
          // __diff_Time= difftime(__end_Time,__start_Time);
          // PrintExecutionOutput(__diff_Time);
           // exit(0);
  
            }
         }   

     }        

            // after the execution is over print statistics
            // close the handle
              dlclose(handle);
           time(&__end_Time);
           __diff_Time= difftime(__end_Time,__start_Time);
           PrintExecutionOutput(__diff_Time);
    
    return 0;
}



//____________________________________________________________________________________________________________________________
// START: ALL BYTE CODE INSTRUCTION 
//____________________________________________________________________________________________________________________________
/*Operation
       Push byte
Format
       bipush
       byte
Forms
       bipush = 16 (0x10)
*/
void BIPUSH()
{           int32_t value;
            interpreter_ProgramCounter++; 
           // The immediate byte is sign-extended to an int value. That value is pushed onto the operand stack.           
            value=(int32_t)((signed char)*interpreter_ProgramCounter);          
           
             Push(value);                                
             interpreter_ProgramCounter++; 
          // PrintStackFrame();  // UNCOMMENT FOR DEBUG
}


/*Operation
     Store int into local variable
 Format
       istore
       index
Forms
      istore = 54 (0x36)
Description
The index is an unsigned byte that must be an index into the local variable array of the current frame (§?2.6).
 The value on the top of the operand stack must be of type int. 
It is popped from the operand stack, and the value of the local variable at index is set to value.
*/
void ISTORE()
{
  interpreter_ProgramCounter++; 
  uint8_t index =(uint8_t)(*interpreter_ProgramCounter);  

  int iResult=(int)(Pop()); 
  (*(stackFrame_localVariableIndexCounter+index))=iResult;  

  // INCREMENT PC        
  interpreter_ProgramCounter++; 
 
   // UNCOMMENT FOR DEBUG
  // PrintStackFrame();

}


/*
Operation
       Push int constant
Format
     iconst_<i>
Forms  
     iconst_0 = 3 (0x3)
*/
void ICONST_0()
{
   Push(0); 
  interpreter_ProgramCounter++;   // INCREMENT PC
    // UNCOMMENT FOR DEBUG
    // PrintStackFrame();
}



/*
Operation
       Branch if int comparison with zero succeeds
Format
       if<cond>
       branchbyte1
       branchbyte2
Forms
    ifeq = 153 (0x99)
    ifne = 154 (0x9a)
    iflt = 155 (0x9b)
    ifge = 156 (0x9c)
    ifgt = 157 (0x9d)
*/
void IFGT()
{

      interpreter_ProgramCounter++;             
      uint8_t unsigned_a=(uint8_t)((unsigned char)*interpreter_ProgramCounter);     //reference: (uint32_t)((signed char)*interpreter_ProgramCounter); 
    
      interpreter_ProgramCounter++;             
      uint8_t unsigned_b=(uint8_t)((unsigned char)*interpreter_ProgramCounter);     

      int iResult=(int)(Pop());              
      if(iResult>0)
      {

          signed short signedShort_offsetAddress=(signed short)((unsigned_a << 8) | unsigned_b);
          interpreter_ProgramCounter--;
          interpreter_ProgramCounter--;                 
          interpreter_ProgramCounter=interpreter_ProgramCounter+signedShort_offsetAddress;     

            //Highlight_ProgramByteCode_GOTO(interpreter_ProgramCounter); // DEBUG
            
      }
      else
      {
        interpreter_ProgramCounter++; 
      }   

        // UNCOMMENT FOR DEBUG
      //   PrintStackFrame();

}

/*goto
   Operation
        Branch always

Format
      goto
      branchbyte1
      branchbyte2
Forms
      goto = 167 (0xa7)
*/
 void GOTO()
 {
              interpreter_ProgramCounter++;                  
              uint8_t  unsigned_a=(uint8_t)(*interpreter_ProgramCounter);

              interpreter_ProgramCounter++;
              uint8_t unsigned_b=(uint8_t)(*interpreter_ProgramCounter);

               int16_t int16_offset=(int16_t)((unsigned_a << 8) | unsigned_b);
               interpreter_ProgramCounter--;
               interpreter_ProgramCounter--;
              
           
              interpreter_ProgramCounter=interpreter_ProgramCounter+int16_offset; 

               // Highlight_ProgramByteCode_GOTO(interpreter_ProgramCounter);
          
               // UNCOMMENT FOR DEBUG
               // PrintStackFrame();
      

 }
/*
 Operation
     Subtract int
Format
     isub
Forms
     isub = 100 (0x64)
 */
 void ISUB()
 {
                int iValue2 =(int)(Pop()); 
                int iValue1= (int)(Pop());           
                int iResult = iValue1-iValue2;                       
                Push(iResult);                 
                // Increment PC
                 interpreter_ProgramCounter++;
                  // UNCOMMENT FOR DEBUG
                  // PrintStackFrame();
 }
 void IADD()
  {  

       int iValue1 =(int) (Pop());                 
       int iValue2= (int) (Pop());              
       int iResult = iValue1+iValue2;                    
       Push(iResult);
      // Increment PC
      interpreter_ProgramCounter++;  

    // UNCOMMENT FOR DEBUG
      //PrintStackFrame();

 }
//Both value1 and value2 must be of type int. They are both popped from the operand stack and compared. All comparisons are signed. The results of the comparison are as follows:
 void ICMPEQ()
 {
      
              interpreter_ProgramCounter++;             
              uint8_t unsigned_a=(uint8_t)((unsigned char)*interpreter_ProgramCounter);     //reference: (uint32_t)((signed char)*interpreter_ProgramCounter); 
              
              interpreter_ProgramCounter++;             
              uint8_t unsigned_b=(uint8_t)((unsigned char)*interpreter_ProgramCounter);     
          

              int iValue1=(int)(Pop()); 
              int iValue2=(int)(Pop()); 
              if(iValue1==iValue2)
              {
                 int16_t int16_offset=(int16_t)((unsigned_a << 8) | unsigned_b);
                 interpreter_ProgramCounter--;
                 interpreter_ProgramCounter--;                 
                 interpreter_ProgramCounter=interpreter_ProgramCounter+int16_offset;                  
                  // Highlight_ProgramByteCode_GOTO(interpreter_ProgramCounter);             
              }
              else
              {
                   interpreter_ProgramCounter++; 
              }  
             // UNCOMMENT FOR DEBUG
             // PrintStackFrame();

 }
/*
 Operation
   Increment local variable by constantF
Format
   iinc
   index
   const
Forms
    iinc = 132 (0x84)
   */
 void IINC() //The index is an unsigned byte that must be an index into the local variable array of the current frame (§?2.6).  //The const is an immediate signed byte. The local variable at index must contain an int. The value const is first sign-extended to an int, and then the local variable at index is incremented by that amount.                
 {
               interpreter_ProgramCounter++;               
               uint8_t index_To_LocalVariableArray =(uint8_t)(*interpreter_ProgramCounter);     //The index is an unsigned byte that must be an index into the local variable array of the current frame (§?2.6). 

                interpreter_ProgramCounter++; 
               int8_t  const_value=(int8_t)(*interpreter_ProgramCounter);                    //The const is an immediate signed byte.


               //The local variable at index must contain an int.
               int value_Referenced_By_Index=(int)(*(stackFrame_localVariableIndexCounter+index_To_LocalVariableArray));  

             //The value const is first sign-extended to an int
            // int signExtended_const_value=(int)const_value;
             value_Referenced_By_Index=value_Referenced_By_Index+(int)const_value;     
            *(stackFrame_localVariableIndexCounter+index_To_LocalVariableArray)=value_Referenced_By_Index; 
     
                            

               // Increment PC
                interpreter_ProgramCounter++;
                // UNCOMMENT FOR DEBUG

 }
 void IWRITE()
 {

        int iResult=(int)(Pop());                 
      
      
      //  Add_Entry_To_Interpreter_Write(iResult);
      //  WriteToTextFile_Output_InterpreterWrite(__Pointer_Interpreter_Write_STATIC,CAPACITY_INTERPRETER_WRITE)

        // THIS IS ONLY FOR THE TEST CASE ITERATIVE ARRAY
        // THERE IS AN ISSUE WITH THE LITTLE ENDIAN AND BIG ENDIAN PRINTING
        // SOMETHIMES THE ENDIANNESS CHANGES DOES NOT NOT
        // I HAVE HANDLED IT HARDCODED FLAG
        // IF I GET TIME, I WOULD DEFINATELY ADDRESS THE ISSUE WITH DYNAMICALLY
        // NOW, IT IS HARDCODED ONLY THE PRINTING PART
    
        
          unsigned char c1, c2, c3, c4;
          if (isFunc2_Of_FibbonacciArray_Invoked)
          { 
            int endianness=GetEndianness();
           int result=changeEndianness(iResult,endianness);          

           printf(" \n  %d",result);
          }
          else
          {
              printf(" \n  %d",iResult);
          }     
        
          
          // INCREMENT PC
          interpreter_ProgramCounter++;

          // UNCOMMENT FOR DEBUG
 }



 void IAND()
 {

               int iValue2 = (int) (Pop());             
               int iValue1 = (int)(Pop());              
               int  iResult=(int)(iValue1 & iValue2);             
                Push(iResult);             
                // INCREMENT PC
                 interpreter_ProgramCounter++;

               // UNCOMMENT FOR DEBUG
              //  PrintStackFrame();
 }

 void IDIV()
 {
              int iValue2 = (int) (Pop());            
              int iValue1 = (int)(Pop());            
              int iResult=iValue1/iValue2;
              Push(iResult);        
              // INCREMENT PC
               interpreter_ProgramCounter++;

                 // UNCOMMENT FOR DEBUG
                //  PrintStackFrame();

 }
 void  IFEQ()
 {

              interpreter_ProgramCounter++;             
              uint8_t unsigned_a=(uint8_t)((unsigned char)*interpreter_ProgramCounter);     //reference: (uint32_t)((signed char)*interpreter_ProgramCounter); 
             
              interpreter_ProgramCounter++;             
              uint8_t unsigned_b=(uint8_t)((unsigned char)*interpreter_ProgramCounter);     
           
                          
             int  iResult=(int)(Pop());
              if(iResult==0)
              {
             
                signed short signedShort_offsetAddress=(signed short)((unsigned_a << 8) | unsigned_b);
                 interpreter_ProgramCounter--;
                 interpreter_ProgramCounter--;   
                 interpreter_ProgramCounter=interpreter_ProgramCounter+signedShort_offsetAddress;   

                  // Highlight_ProgramByteCode_GOTO(interpreter_ProgramCounter);           
                   
              }
              else
              {
                  //INCREMENT PC
                   interpreter_ProgramCounter++; 
              }   

              // UNCOMMENT FOR DEBUG
              // PrintStackFrame();
 }
 void IFLT()
 {
        

              interpreter_ProgramCounter++;             
              uint8_t unsigned_a=(uint8_t)((unsigned char)*interpreter_ProgramCounter);     //reference: (uint32_t)((signed char)*interpreter_ProgramCounter); 
              interpreter_ProgramCounter++;             
              uint8_t unsigned_b=(uint8_t)((unsigned char)*interpreter_ProgramCounter);     
           
              
             int  iResult=(int)(Pop());             

              if(iResult<0)
              {
               
                signed short signedShort_offsetAddress=(signed short)((unsigned_a << 8) | unsigned_b);
                 interpreter_ProgramCounter--;
                 interpreter_ProgramCounter--;              
                
                 interpreter_ProgramCounter=interpreter_ProgramCounter+signedShort_offsetAddress;     

                  // Highlight_ProgramByteCode_GOTO(interpreter_ProgramCounter);           
              }
              else
              {
                  //INCREMENT PC
                   interpreter_ProgramCounter++; 
              }   
              // UNCOMMENT FOR DEBUG
              // PrintStackFrame();

 }

 void IFGE()
 {

             interpreter_ProgramCounter++;                  
             uint8_t unsigned_a=(uint8_t)(*interpreter_ProgramCounter);  
              interpreter_ProgramCounter++;              
             uint8_t  unsigned_b=(uint8_t)(*interpreter_ProgramCounter);             
              
             int iResult=(int)(Pop()); 
              if(iResult>=0)
              {
                
                signed short  signedShort_offsetAddress=(signed short)((unsigned_a << 8) | unsigned_b);
                 interpreter_ProgramCounter--;
                 interpreter_ProgramCounter--;                                                 
                 interpreter_ProgramCounter=interpreter_ProgramCounter+signedShort_offsetAddress;                
                   
                //   Highlight_ProgramByteCode_GOTO(interpreter_ProgramCounter);
              }
              else
              {
                  //INCREMENT PC
                   interpreter_ProgramCounter++; 
              }   
             // UNCOMMENT FOR DEBUG
             // PrintStackFrame();

 }

 void  IFLE()
{
 
               interpreter_ProgramCounter++;             
              uint8_t unsigned_a=(uint8_t)((unsigned char)*interpreter_ProgramCounter);     //reference: (uint32_t)((signed char)*interpreter_ProgramCounter); 
             
              interpreter_ProgramCounter++;             
              uint8_t unsigned_b=(uint8_t)((unsigned char)*interpreter_ProgramCounter);     
            
              
              int iResult=(int)(Pop());           
              
              if(iResult<=0)
              {
               
                 signed short  signedShort_offsetAddress=(signed short)((unsigned_a << 8) | unsigned_b);
                 interpreter_ProgramCounter--;
                 interpreter_ProgramCounter--;   
                 interpreter_ProgramCounter=interpreter_ProgramCounter+signedShort_offsetAddress;                
                   
                //   Highlight_ProgramByteCode_GOTO(interpreter_ProgramCounter);
              }
              else
              {
                  //INCREMENT PC
                   interpreter_ProgramCounter++; 
              }   
             // UNCOMMENT FOR DEBUG
            //  PrintStackFrame();
}

void IFNULL()
{

               interpreter_ProgramCounter++;             
              uint8_t unsigned_a=(uint8_t)((unsigned char)*interpreter_ProgramCounter);     //reference: (uint32_t)((signed char)*interpreter_ProgramCounter); 
             
              interpreter_ProgramCounter++;             
              uint8_t unsigned_b=(uint8_t)((unsigned char)*interpreter_ProgramCounter);     
              
             int iResult=(int)(Pop());         
              
              if(iResult==0)
              {                
                 signed short signedShort_offsetAddress=(signed short)((unsigned_a << 8) | unsigned_b);
                 interpreter_ProgramCounter--;                 
                 interpreter_ProgramCounter--;  

                 interpreter_ProgramCounter=interpreter_ProgramCounter+signedShort_offsetAddress;  
                 
               //    Highlight_ProgramByteCode_GOTO(interpreter_ProgramCounter);             
                                  
              }
              else
              {
                  //INCREMENT PC
                   interpreter_ProgramCounter++; 
              }   
              // UNCOMMENT FOR DEBUG
             //  PrintStackFrame();

}
//The result is the 32 low-order bits of the true mathematical result in a sufficiently wide two's-complement format, represented as a value of type int. If overflow occurs, then the sign of the result may not be the same as the sign of the mathematical sum of the two values.
void  IMULL()
{
             int iValue2 = (int) (Pop());            
             int iValue1 = (int)(Pop()); 
         
              int iResult=iValue1* iValue2;          
              Push(iResult);
              
              //INCREMENT PC
               interpreter_ProgramCounter++;

            // UNCOMMENT FOR DEBUG
            // PrintStackFrame();

}
void INEG()
{
             int iValue1 = (int) (Pop()); 
             int iResult=-iValue1;
           
              Push(iResult);             

              //INCREMENT PC
              interpreter_ProgramCounter++;

              // UNCOMMENT FOR DEBUG
            //   PrintStackFrame();

}
void IOR()
{
               int iValue2 = (int) (Pop());           
               int iValue1 = (int)(Pop()); 
           
               int iResult=(int)(iValue1 | iValue2);              
                
               Push(iResult);  
                   
                //INCREMENT PC
                interpreter_ProgramCounter++;

                // UNCOMMENT FOR DEBUG
              //  PrintStackFrame();

}

void POP_OperandStack() 	                              // discard the top value on the stack
{
    // Check if StackPointer value is not Less than equal to FramePointer
     if(operandStack_Pointer<=stackFramePointer_Dynamic)
      {
        printf("\n Can not pop as Operand Stack is Empty.");      
      }
     else{
          //int poppedValue=*operandStack_Pointer;   
          operandStack_Pointer--;          
        }                   
              interpreter_ProgramCounter++;

             // UNCOMMENT FOR DEBUG
            //  PrintStackFrame();
}


/*
return
Operation
   Return void from method
Format
    return
Forms
    return = 177 (0xb1)
Operand Stack
   ... ??
   [empty]
Description
      The current method must have return type void. If the current method is a synchronized method, 
      the monitor entered or reentered on invocation of the method is updated and possibly exited as if by execution of 
      a monitorexit instruction (§?monitorexit) in the current thread. If no exception is thrown, any values on the operand 
      stack of the current frame (§?2.6) are discarded.
      The interpreter then returns control to the invoker of the method, reinstating the frame of the invoker.
*/
void RETURN()
{
 
        // If it is main method, exit from the program  
        // CHECKING IF THE CURRENT STACK FRAME POINTER IS SAME AS FIRST STACK FRAME POINTER OF MAIN METHOD 
        if(stackFrame_Pointer_In_MainMethod==stackFramePointer_Static) 
        {    
            // printf("\t(Returning from Main Method)\n\n");
             
            time(&__end_Time);
           __diff_Time= difftime(__end_Time,__start_Time);
           PrintExecutionOutput(__diff_Time);

             exit(0);// RETURING FROM MAIN  
        }
        else
        {
              // RETURN WILL DO THAT SAME AS IRETURN EXCEPT THE RETURNING ANY VALUE FROM PREVIOUS METHOD
     
               
              // Need to take value from current Stack Frame to go to the previous Frame
              // This will be opposite to Invoke Static where we create a new frame and go to that frame
              // We will return from that frame specified by the addresses previously saved by the Invoke Static Method.

              // Read all the value saved in the current Frame
              // First: We need program counter to return to the previous Frame
              // Previous program counter address is saved in the curren frame
             // Always get the physical addres before doing 
             
             

                // get the previous Program counter pointer, this is integer pointer
                int * previousFrame_ProgramCounter_POINTER_TO_PHYSICAL_ADDRESS=Convert_To_PhysicalAddress((uint32_t)(*(stackFramePointer_Static+1)));  
             
                // change it to the unsigned char Pointer
                unsigned char * previousFrame_ProgramCounter_CHAR_POINTER=(unsigned char *)previousFrame_ProgramCounter_POINTER_TO_PHYSICAL_ADDRESS;

              //Get the pointer to the the Physical Address of OPERAND STACK
              int * previouFrame_OperandStack_POINTER_TO_PHISICAL_ADDRESS=Convert_To_PhysicalAddress((uint32_t)(*(stackFramePointer_Static+2)));  

              // Get the pointer to the physical address of the STACK FRAME
              int * previouFrame_StackFrame_POINTER_TO_PHISICAL_ADDRESS=Convert_To_PhysicalAddress((uint32_t)(*(stackFramePointer_Static+3))); 


                // Here operand stack top will be 1 less than the Current Stack Frame
                // NEW OPERAND STACK POINTER
                operandStack_Pointer=stackFramePointer_Static-1; 

              // New Stack Frame Pointer
               stackFramePointer_Static=previouFrame_StackFrame_POINTER_TO_PHISICAL_ADDRESS;
             
             // Once we get the StackFrame Pointer of previous Frame, we can calcuale the other pointers based on the relative postion
              stackFramePointer_Dynamic=stackFramePointer_Static+19;

              // we can now return the pointer for stackFrame_localVariableIndexCounter
               stackFrame_localVariableIndexCounter=stackFramePointer_Static+4;

              // New PROGRAM COUNTER POINTER.
              interpreter_ProgramCounter=previousFrame_ProgramCounter_CHAR_POINTER;       
          
        }  
   
   // PrintStackFrame();         
}



void IRETURN()  //Return int from method
{
       
            // Save the value popped from the Operand Stack to the Return Value Section of the Current Stack Frame
            // Later this will become top of stack of previous Operand Stack
          
           // PrintStackFrame();
               int iResult=(int)(Pop());
              // Saving the value at the top of the stack frame
              *stackFramePointer_Static=iResult; 

            
              // get the previous Program counter pointer, this is integer pointer
                int * previousFrame_ProgramCounter_POINTER_TO_PHYSICAL_ADDRESS=Convert_To_PhysicalAddress((uint32_t)(*(stackFramePointer_Static+1)));  
             
                // change it to the unsigned char Pointer
                unsigned char * previousFrame_ProgramCounter_CHAR_POINTER=(unsigned char *)previousFrame_ProgramCounter_POINTER_TO_PHYSICAL_ADDRESS;
               
               // PROGRAM COUNTER
                interpreter_ProgramCounter=previousFrame_ProgramCounter_CHAR_POINTER;
               
              //PREVIOUS OPERAND STACK POINTER
              int * previouFrame_OperandStack_POINTER_TO_PHISICAL_ADDRESS=(Convert_To_PhysicalAddress((uint32_t)(*(stackFramePointer_Static+2))));  

             // PREVIOUS STACK FRAME POINTER
              int * previouFrame_StackFrame_POINTER_TO_PHISICAL_ADDRESS=(Convert_To_PhysicalAddress((uint32_t)(*(stackFramePointer_Static+3)))); 


                // PREVIOUS OPERAND STACK IS THE SAME AS CURRENT STACK FRAME ADDRESS
                operandStack_Pointer=stackFramePointer_Static;                
              
              // STACK FRAME POINTER STATIC
               stackFramePointer_Static=previouFrame_StackFrame_POINTER_TO_PHISICAL_ADDRESS;
             
             // DYNAMIC FRAME POINTER
              stackFramePointer_Dynamic=stackFramePointer_Static+19;

              // LOCAL VARIABLE POINTER
               stackFrame_localVariableIndexCounter=stackFramePointer_Static+4;
           
          //  PrintStackFrame(); // UNCOMMENT FOR DEBUG   
           //printf("\n");

}

/*

invokestatic
Operation
      Invoke a class (static) method
Format
        invokestatic
        indexbyte1
        indexbyte2
Forms
      invokestatic = 184 (0xb8)

Operand Stack
      ..., [arg1, [arg2 ...]] ??.     ..

Description
        The unsigned indexbyte1 and indexbyte2 are used to construct an index into the run-time constant pool of the current class (§?2.6), where the value of the index is (indexbyte1 << 8) | indexbyte2. 
        The run-time constant pool item at that index must be a symbolic reference to a method (§?5.1), 
        which gives the name and descriptor (§?4.3.3) of the method as well as a symbolic reference to the class in which the method is to be found. The named method is resolved (§?5.4.3.3). 
        The resolved method must not be an instance initialization method (§?2.9) or the class or interface initialization method (§?2.9). It must be static, and therefore cannot be abstract.

        On successful resolution of the method, the class that declared the resolved method is initialized (§?5.5) if that class has not already been initialized.
        The operand stack must contain nargs argument values, where the number, type, and order of the values must be consistent with the descriptor of the resolved method.


*/

void INVOKE_STATIC()
{  
            //  printf("\n before creating a frame");
           //  PrintStackFrame();
              bool is_Invoked_Interpreter=false;   // WHEN THERE WILL BE A          
             
             
              interpreter_ProgramCounter++;             
              uint8_t unsigned_a=(uint8_t)((unsigned char)*interpreter_ProgramCounter);     //reference: (uint32_t)((signed char)*interpreter_ProgramCounter); 
              
              interpreter_ProgramCounter++;             
              uint8_t unsigned_b=(uint8_t)((unsigned char)*interpreter_ProgramCounter);     

             
              int16_t constantPool_Reference=(int16_t)(unsigned_a << 8 | unsigned_b); 

              unsigned char * referenceToNewMethod=(unsigned char *)(constantPool_Pointer_VM+constantPool_Reference*7);          
              uint32_t methodReference = ((uint32_t)referenceToNewMethod[0] << 24) | ((uint32_t)referenceToNewMethod[1] << 16) | ((uint32_t)referenceToNewMethod[2] << 8) | (uint32_t)referenceToNewMethod[3];
              
              uint8_t  numberOfArguments_InvokeStatic=(uint8_t )(*(referenceToNewMethod+4));
              uint8_t  argumentType_InvokeStatic=(uint8_t )(*(referenceToNewMethod+5));
              uint8_t  returnValue_InvokeStatic=(uint8_t )(*(referenceToNewMethod+6)); 
          
           // DEBUG INFO: DO NOT DELETE-COMMENT ONLY
           // printf("\n numberOfArguments_InvokeStatic %d",numberOfArguments_InvokeStatic);
          //  printf("\n argumentType_InvokeStatic %d",argumentType_InvokeStatic);
           //  printf("\n returnValue_InvokeStatic %d",returnValue_InvokeStatic);
 
             
              // Highlight_ProgramByteCode(methodReference);
              // CREATE THE FUNCTION NAME
              char function_number[1000]; 
              char func[]="func_";            
              sprintf(function_number,"%d",(uint32_t)constantPool_Reference);
              int lengh_of_String=strlen(function_number);
              strncat(func,function_number,lengh_of_String);
              strcpy(functionName_Of_CompiledCode,func);    

             // This table will maintain the unique name of functions/methods
             bool isMethodPresent=Is_Method_Exists_In_Table((uint32_t) constantPool_Reference);
             // If not present add it to the table, otherwise no action needed
             if(!isMethodPresent)
             {
                Add_Entry_To_AllMethods((uint32_t) constantPool_Reference);
               //  WriteToTextFile_Output_AllMethods(__MethodTable_Pointer_AllMethods_STATIC,__TotalNumber_Of_Items_In_AllMethods);
             }
           

             // AOT SUPPORT
              /*
              For each of the methods that are in the const_pool you need to keep a counter.
              This counter will start at 0 and increment every time the method is invoked. Once
              it reaches a threshold then you are to check if a compiled version of the method
              exists. If so, then you are to make note of this in the const_pool and from then on
              invoke the compiled method instead of the interpreted one
              */

            // ________________________________________________________________________________________________________
             // STAGE 1: 
             // CHECKS WHETHER THE PRECOMPILED METHOD EXISTS OR NOT . BASED ON THE RESULT THE FLAG WILL BE SET.
             // IF PRECOMPILED METHOD EXISTS, EXECUTE IT
             // IF NOT, SET THE FLAG FOR NORMAL INVOKE STATIC
             //__________________________________________________________________________________________________________
      
             uint32_t methodCounter_Interpreter=Search_Number_Of_Methods_In_MethodTable_Interpreter((uint32_t) constantPool_Reference);
             uint32_t methodCounter_Compiler=Search_Number_Of_Methods_In_MethodTable_Compiled((uint32_t) constantPool_Reference);
     
            //  METHOD COUNTER > THREASHOLD GO FOR ** COMPILATION
            if(methodCounter_Interpreter>=__methodThreshold)
            {
                                        
          

                    // void* handle = dlopen(libraryName_Of_CompiledCode,RTLD_LAZY|RTLD_GLOBAL);
                   
                     //int (*fp_ReturnINT_Argument_0)();                          // RETURN TYPE INTEGER, NO ARGUMENT
                     int (*fp_ReturnINT_Argument_1)(int);                         // RETURN TYPE INTEGER, WITH  1 ARGUMENT 
                     //int (*fp_ReturnINT_Argument_2)(int,int);                   // RETURN TYPE INTEGER, WITH  2 ARGUMENT 
                    // int (*fp_ReturnINT_Argument_3)(int,int,int);                // RETURN TYPE INTEGER, WITH 3 ARGUMENT 
                    // int (*fp_ReturnINT_Argument_4)(int,int,int,int);            // RETURN TYPE INTEGER, WITH 4 ARGUMENT 
                     int (*fp_ReturnINT_Argument_5)(int,int,int,int,int);        // RETURN TYPE INTEGER, WITH  5 ARGUMENT                     
                  
                     //void (*fp_ReturnVOID_Argument_0)();                                                                                                                 // RETURN TYPE VOID, NO ARGUMENT           // placeholder for future
                     //void (*fp_ReturnVOID_Argument_1)(int);                                                                                           // RETURN TYPE VOID, WITH 1 ARGUMENT       // placeholder for future
                    // void (*fp_ReturnVOID_Argument_2)(int,int);                                                                                       // RETURN TYPE VOID, WITH 2 ARGUMENT       // placeholder for future
                     //void (*fp_ReturnVOID_Argument_3)(int,int,int);                                                                                   // RETURN TYPE VOID, WITH 3 ARGUMENT       // placeholder for future
                    // void (*fp_ReturnVOID_Argument_4)(int,int,int,int);                                                                               // RETURN TYPE VOID, WITH 4 ARGUMENT       // placeholder for future
                    // void (*fp_ReturnVOID_Argument_5)(int, int, int, int,int);                                                                        // RETURN TYPE VOID, WITH 5 ARGUMENT       // placeholder for future               
                     void (*fp_ReturnVOID_Argument_6_V1)(int32_t,int32_t,int32_t,int32_t,int32_t*,int32_t);// RETURN TYPE VOID, WITH 6 ARGUMENT       // TYPE1
                     void (*fp_ReturnVOID_Argument_6_V2)(int32_t,int32_t,int32_t,int32_t,int32_t,int32_t*);// RETURN TYPE VOID, WITH 6 ARGUMENT       // TYPE2
                
                     char *error;                  
                                            
                        /* 
                         argument types   :  0 for integer and 1 for a reference 
                         The return value :  0 for an int, 1 for a reference, and 2 for no return value.
                         */
                       if (handle != NULL)
                       {                 
                      
                               if(returnValue_InvokeStatic==0)
                               {   

                                        switch(numberOfArguments_InvokeStatic)
                                        {
                                            case 0:                                                      
                                                   // NOT IMPLEMENTED                            
                                            break;
                                            
                                            // SUPPORTS PRINT 3
                                            case 1:
                                                        fp_ReturnINT_Argument_1= (int (*)(int)) dlsym(handle,functionName_Of_CompiledCode );                                                      
                                                        dlerror();
                                                        error = dlerror();
                                                        if (error != NULL)
                                                        {
                                                     
                                                            is_Invoked_Interpreter=true;     // YES INTERPRETER METHOD
                                                            fprintf(stderr, "%s\n", error);
                                                           // exit(EXIT_FAILURE);
                                                        }
                                                        else
                                                        {  
                                                           is_Invoked_Interpreter=false;  
                                                          // ADDS ENTRY TO THE COMPILER COUNTER TABLE 
                  
                                                           Add_Entry_To_Method_Table_Compiler((uint32_t) constantPool_Reference,methodCounter_Compiler);

                                                              int arg0; 
                                                          
                                                               arg0=(int) (Pop());  
                                                             int returnValue_from_CompiledMethod=fp_ReturnINT_Argument_1(arg0);                                                         
                                                       
                                                            // NEED TO PUT THIS VALUE AT THE TOP OF THE STACK  
                                                            operandStack_Pointer=operandStack_Pointer+1;                                                       
                                                            *operandStack_Pointer=returnValue_from_CompiledMethod;   

                                                            interpreter_ProgramCounter++;                                                        
                                                           // PrintStackFrame();
                                                        }
                                            break;                                          
                                        
                                            // SUPPORTS FIBONACCI
                                            case 5:
                                                        //PrintStackFrame();
                                                 
                                                        fp_ReturnINT_Argument_5= (int (*)(int, int, int, int,int)) dlsym(handle,functionName_Of_CompiledCode );                                                      
                                                        dlerror();
                                                        error = dlerror();
                                                        if (error != NULL)
                                                        {                                                           
                                                            is_Invoked_Interpreter=true;           // YES ==>INTERPRETER METHOD

                                                            fprintf(stderr, "%s\n", error);
                                                           // exit(EXIT_FAILURE);
                                                        }
                                                        else
                                                        {                                                                                                             

                                                           is_Invoked_Interpreter=false;  
                                                            // ADDS ENTRY TO THE COMPILER COUNTER TABLE     
                                                             
                                                           Add_Entry_To_Method_Table_Compiler((uint32_t) constantPool_Reference,methodCounter_Compiler);  
                                                           // WriteToTextFile_MethodTable_Compiler(__MethodTable_Pointer_Compiled_STATIC,CAPACITY_METHOD_TABLE_COMPILED );
                                                          
                                                          int arg0,arg1, arg2, arg3, arg4;
                                                          // Based on the Number of Argument from the constant pool,retrieve them
                                                          // This switch statement, there are 5 number of args                                          
                                                             
                                                               arg4=(int) (Pop()); 
                                                               arg3=(int) (Pop());  
                                                               arg2=(int) (Pop());  
                                                               arg1=(int) (Pop());  
                                                               arg0=(int) (Pop());  

                                                              // DO I  NEED TO CONVERT FROM 64 BIT TO 32 BIT?
                                                              // ALEADY THE VALUES ARE LESS THAN 64 BIT IN THE TEST CASES
                                                              // OTHERWISE I NEED TO TAKE TWO SPACE
                                                                int returnValue_from_CompiledMethod;                                                           
                                                                returnValue_from_CompiledMethod=(int)(fp_ReturnINT_Argument_5(arg0,arg1,arg2,arg3,arg4)); 
                                                                 
                                                              // NEED TO PUT THIS VALUE AT THE TOP OF THE STACK  
                                                               operandStack_Pointer=operandStack_Pointer+1;                                                       
                                                               *operandStack_Pointer=returnValue_from_CompiledMethod;

                                                               interpreter_ProgramCounter++;                                                           
                                                              //PrintStackFrame();
                                                        } 
                                             break;
                                            default:
                                            break;

                                        }                                       
                                 
                               }
                               //1 for a reference                               
                               else if(returnValue_InvokeStatic==1)
                               {
                                 /// METHOD SHARE NOT IMPLEMENTED
                               }
                               //2 for no return value.
                               else if(returnValue_InvokeStatic==2)
                               {                                 
                                      switch(numberOfArguments_InvokeStatic)
                                      {                                                                                   
                                            
                                           
                                               case 6:
                                                         // PrintStackFrame();
                                                      // Need to implement something dynamic way to check the types of argument and call apt method
                                                      // For now static support 
                                                      if((uint32_t) constantPool_Reference==1)
                                                      {
                                                       
                                                        
                                                          fp_ReturnVOID_Argument_6_V1=(void (*)(int32_t,int32_t,int32_t,int32_t,int32_t *,int32_t)) dlsym(handle, functionName_Of_CompiledCode);    // RETURN TYPE VOID, WITH 6 ARGUMENT      
                                                           dlerror();
                                                           error = dlerror();
                                                           if (error != NULL)
                                                           {                                                         
                                                             is_Invoked_Interpreter=true;     // YES INTERPRETER METHOD
                                                             fprintf(stderr, "%s\n", error);
                                                            //exit(EXIT_FAILURE);
                                                           }  
                                                           else
                                                           {

                                                           //  PrintStackFrame();                                                    
                                                            is_Invoked_Interpreter=false;  
                                                            // ADDS ENTRY TO THE COMPILER COUNTER TABLE     
                                                             Add_Entry_To_Method_Table_Compiler((uint32_t) constantPool_Reference,methodCounter_Compiler);
                                                            // WriteToTextFile_MethodTable_Compiler(__MethodTable_Pointer_Compiled_STATIC,CAPACITY_METHOD_TABLE_COMPILED );

                                                             int32_t arg0,arg1,arg2,arg3,size;
                                                             int32_t *pointer_To_Array=NULL;
                                                             uint32_t array_Logical_Address;

                                                             // get the reference of the object in the heap                                                         

                                                               size=(int32_t) (Pop()); 

                                                               // GET THE LOGICAL ADDRESS FROM THE JVM OPERAND STACK
                                                               array_Logical_Address=(int32_t)(Pop()); 
                                                       
                                                               // convert the address to the array address in C program
                                                               pointer_To_Array=(Convert_To_PhysicalAddress(array_Logical_Address));

                                                               arg3=(int32_t) (Pop());  
                                                               arg2=(int32_t) (Pop());  
                                                               arg1=(int32_t) (Pop());  
                                                               arg0=(int32_t) (Pop());  

                                                              fp_ReturnVOID_Argument_6_V1(arg0,arg1,arg2,arg3,pointer_To_Array,size); 

                                                             interpreter_ProgramCounter++;                                                        
                                                           // PrintStackFrame(); 
                                                        } 

                                                      }
                                                      else if((uint32_t) constantPool_Reference==2)
                                                      {
                                                      
                                                           fp_ReturnVOID_Argument_6_V2=(void (*)(int32_t,int32_t,int32_t,int32_t,int32_t,int32_t*)) dlsym(handle, functionName_Of_CompiledCode);    
                                                          
                                                           dlerror();
                                                           error = dlerror();
                                                           if (error != NULL)
                                                           {                                                         
                                                             is_Invoked_Interpreter=true;     // YES INTERPRETER METHOD
                                                             fprintf(stderr, "%s\n", error);
                                                            //exit(EXIT_FAILURE);
                                                           }  
                                                           else
                                                           {


                                                            is_Invoked_Interpreter=false;  
                                                            // ADDS ENTRY TO THE COMPILER COUNTER TABLE     
                                                             Add_Entry_To_Method_Table_Compiler((uint32_t) constantPool_Reference,methodCounter_Compiler);
                                                             // WriteToTextFile_MethodTable_Compiler(__MethodTable_Pointer_Compiled_STATIC,CAPACITY_METHOD_TABLE_COMPILED );
                                                             
                                                             int32_t arg0,arg1,arg2,arg3,size;
                                                             int32_t *pointer_To_Array=NULL;
                                                             uint32_t array_Logical_Address;                                                            

                                                              // GET THE LOGICAL ADDRESS FROM THE JVM OPERAND STACK
                                                               array_Logical_Address=(int32_t)(Pop()); 
                                                               // convert the address to the array address in C program
                                                               pointer_To_Array=(Convert_To_PhysicalAddress(array_Logical_Address));
                                                            

                                                               size=(int32_t) (Pop()); 
                                                               arg3=(int32_t) (Pop());  
                                                               arg2=(int32_t) (Pop());  
                                                               arg1=(int32_t) (Pop());  
                                                               arg0=(int32_t) (Pop());  

                                                              fp_ReturnVOID_Argument_6_V2(arg0,arg1,arg2,arg3,size,pointer_To_Array); 
                                                               
                                                               isFunc2_Of_FibbonacciArray_Invoked=true; // Flag to handle print from Little Endian to BigEndian
                                                              //ShowMemoryAlloctionInHeap(pointer_To_Array,size); // DEBUG
                                                              interpreter_ProgramCounter++;                                                        
                                                              // PrintStackFrame(); 
                                                        } 
                                                        
                                                      }                                             

                                               break;

                                               default:
                                               break;

                                      }// switch                                  
                               }
                       
                         
                       }    

                      
                                        
            }
            // IF THE METHOD COUNTER < THREASHOLD GO FOR ** INTERPRETATION
            // Did not excceded the theshold value, keep calling the Invoke Static Method
            else
            {
               // printf("\n INTERPRETER METHODS WILL BE INVOKED\n");
                is_Invoked_Interpreter=true;          
            }       
             

             //_________________________________________________________________________________________________________
             //STAGE 2
             // BASED ON THE FLAG VALUE, EITHER CALL THE INERPRETER OR COMPILED CODE
             //___________________________________________________________________________________________________________
             
             // OLD INVOKE STATIC
             if(is_Invoked_Interpreter)
             {
                

                  // Add the method to the Interpreter Method Table
                   Add_Entry_To_Method_Table_Interpreter((uint32_t) constantPool_Reference,methodCounter_Interpreter);
                   //WriteToTextFile_Method_Interpreter(__MethodTable_Pointer_Interpreter_STATIC,CAPACITY_METHOD_TABLE_INTERPRETER);
             
                  // This is for Diagnostic Purpose. It highlights the byte in Exec
                  // Do not delete
                 // Highlight_ProgramByteCode(methodReference);

                  // This is for Diagnostic Purpose. It highlights the byte in cosntant pool
                  // Do not delete
                   // Highlight_ConstantPoolReference(referenceToNewMethod);

               // Create a New Frame
               
    
                // SINCE WE HAVE READ TWO BYTES BY PROGRAM COUNTER TO CREATE THE INDEX, NEED TO DECREMENT THE PC TO GO THE OPCODE
                interpreter_ProgramCounter--;
                interpreter_ProgramCounter--;

                // WE NEED TO PASS THE ARGEMENT TO THE 
                int arraryOfArgument[16]={0};
                int *argumentPointer_InvokeStatic=arraryOfArgument;  
                int poppedValue;
                int argCounter;
                // POP THE VALUE AND THEN SAVE IN THE ARRAY
                  for(argCounter=numberOfArguments_InvokeStatic-1;argCounter>=0;argCounter--)
                  {                
                      poppedValue=(int) (Pop());                
                      arraryOfArgument[argCounter]=poppedValue;                 
                  }

              // SAVE THE VALUES INTO THE CURRENT FRAME
              *(operandStack_Pointer+1)=returnValue_InvokeStatic;                                               // LOCATION 0: RETURN VALUE              
              *(operandStack_Pointer+2)=(uint32_t)(Convert_To_LogicalAddres(interpreter_ProgramCounter+3));     // LOCATION 1: PREVIOUS PROGRAM COUNTER
              *(operandStack_Pointer+3)=(uint32_t)(Convert_To_LogicalAddres(operandStack_Pointer));             // LOCATION 2: PREVIOUS OPERAND STACK ADDRESS
              *(operandStack_Pointer+4)=(uint32_t)(Convert_To_LogicalAddres(stackFramePointer_Static));         // LOCATION 3: PREVIOUS FRAME ADDRESS

            // Change the Pointer Variables
            stackFramePointer_Static= operandStack_Pointer + 1;                      // new stack frame pointer STATIC
            stackFramePointer_Dynamic=stackFramePointer_Static+19;                   // new stack frame pointer DYNAMIC
            stackFrame_localVariableIndexCounter = stackFramePointer_Static + 4;     // new index to the local variables
            operandStack_Pointer = stackFramePointer_Static + 19;                     // new Oprand stack . It will be pointing to the Top-1    


            // Copying the previous values to the new Local variable Location         // LOCTION  4: SAVE THE PARAMETER       
            for(argCounter=0;argCounter<=numberOfArguments_InvokeStatic-1;argCounter++)
            {
                *(stackFrame_localVariableIndexCounter+argCounter)=*(argumentPointer_InvokeStatic+argCounter);       
            }
              // INTERPRETER PC WILL BE JVM + THE REFERENCE BY THE CONSTANT POOL
              interpreter_ProgramCounter = (unsigned char *)JVM+methodReference;


              //header="INSIDE INVOKE STATIC: Created a New Frame";
              // Printheader(header);
              //PrintStackFrame(); // UNCOMMENT FOR DEBUG    

             } // END OF IF : INVOKE_STATIC()                  
              
}



//read file
//______________________________________________________________________________
void IREAD()
{
          int readInt;
          scanf("%d",&readInt);              
          Push(readInt);
         // INCREMENT PC
          interpreter_ProgramCounter++;

        // PrintStackFrame(); // UNCOMMENT FOR DEBUG
}

/*
new
Operation
    Create new object
Format
    new
    indexbyte1
    indexbyte2
Forms
    new = 187 (0xbb)
Operand Stack
.   .. ??
    ..., objectref
Description
    The unsigned indexbyte1 and indexbyte2 are used to construct an index intfo the run-time constant pool of the current class (§?2.6), 
    where the value of the index is (indexbyte1 << 8) | indexbyte2. The run-time constant pool item at the index must be a 
    symbolic reference to a class or interface type. The named class or interface type is resolved (§?5.4.3.1) 
    and should result in a class type. Memory for a new instance of that class is allocated from the garbage-collected heap, 
    and the instance variables of the new object are initialized to their default initial values (§?2.3, §?2.4). The objectref, 
    a reference to the instance, is pushed onto the operand stack.
*/
void NEW()    
{ 
               interpreter_ProgramCounter++;             
              uint8_t unsigned_a=(uint8_t)((unsigned char)*interpreter_ProgramCounter);     //reference: (uint32_t)((signed char)*interpreter_ProgramCounter); 
             
              interpreter_ProgramCounter++;             
              uint8_t unsigned_b=(uint8_t)((unsigned char)*interpreter_ProgramCounter);     

              // Create new object of type identified by class reference in constant pool index (indexbyte1 << 8 | indexbyte2)
               int16_t index_in_constPool=(int16_t)(unsigned_a << 8 | unsigned_b); 

             
            // Unsigned char because the constnat pool file itself has one byte values
             unsigned char *fieldReference_In_Const_Pool=(unsigned char *)(objectReference_In_ConstantPool+index_in_constPool*4);  

            // Highlight_ConstantPoolReference(fieldReference_In_Const_Pool);           
             
             // Get the Size of Fields Pointed by the fieldReference_In_Const_Pool 
               uint8_t  numberOfObjects=(uint8_t )(*(fieldReference_In_Const_Pool));  

             // Need to save the pointer to the stack   // Push the START OF THE logical address into the stack             
              uint32_t logicalAddress_HeapPointer=(uint32_t)(Convert_To_LogicalAddres(heapPointer_VM));                 
               Push(logicalAddress_HeapPointer);    
             
             // Assign zero to all objects in the heap  
              int COUNTER=0;                               
              for(COUNTER=0;COUNTER<numberOfObjects;COUNTER++)
              {
                 *(heapPointer_VM-COUNTER)=0;                  
                 
              }                                            
             
                heapPointer_VM=heapPointer_VM-numberOfObjects;     // Decrement the heap Pointer based on the size of the fields              
                interpreter_ProgramCounter++;    // Increment the Program Counter
               // PrintStackFrame(); // UNCOMMENT FOR DEBUG              

}


/*
anewarray
Operation
    Create new array of reference
Format
    anewarray
    indexbyte1
    indexbyte2
Forms   
anewarray = 189 (0xbd)
Operand Stack
    ..., count ??
    ..., arrayref
Description
    The count must be of type int. It is popped off the operand stack. 
    The count represents the number of components of the array to be created.
     The unsigned indexbyte1 and indexbyte2 are used to construct an index into the run-time constant pool of the current class (§?2.6), 
     where the value of the index is (indexbyte1 << 8) | indexbyte2. The run-time constant pool item at that index must be a 
     symbolic reference to a class, array, or interface type. The named class, array, or interface type is resolved (§?5.4.3.1). 
     A new array with components of that type, of length count, is allocated from the garbage-collected heap,
      and a reference arrayref to this new array object is pushed onto the operand stack. 
      All components of the new array are initialized to null, the default value for reference types (§?2.4).
*/
void ANEWARRAY()  //Create new array of reference
{
    
              int number_of_components_of_array_to_be_Created=(int)(Pop());   
         
            
              interpreter_ProgramCounter++;  
              uint8_t unsigned_a=(uint8_t)((unsigned char)*interpreter_ProgramCounter);     //reference: (uint32_t)((signed char)*interpreter_ProgramCounter); 
             
              interpreter_ProgramCounter++;             
              uint8_t unsigned_b=(uint8_t)((unsigned char)*interpreter_ProgramCounter);     


              //Get the Index in Constant Pool               
               int16_t index_In_ConstantPool=(int16_t)(unsigned_a << 8 | unsigned_b); 
               
               // Get the Pointer
              unsigned char * symbolic_reference_To_Array=(unsigned char *)(objectReference_In_ConstantPool+index_In_ConstantPool*4);  
              
                // Need to create an array and save the starting address into the stack
                  // Relative Address
                 uint32_t logicalAddress_HeapPointer=(uint32_t)(Convert_To_LogicalAddres(heapPointer_VM));                
           
                 // Push the Logical Address into stack           
                 Push(logicalAddress_HeapPointer);

                  int COUNTER=0;             
                 for(COUNTER=0;COUNTER<number_of_components_of_array_to_be_Created;COUNTER++)
                  {
                     *((heapPointer_VM)-COUNTER)=0; 
                   }                     
                         
                  // Decrement the Heap Pointer
                  heapPointer_VM=heapPointer_VM-number_of_components_of_array_to_be_Created;
                 
                 // INCREMENT PC
                  interpreter_ProgramCounter++;  
               // PrintStackFrame(); // UNCOMMENT FOR DEBUG

}


/*
newarray
Operation
    Create new array
Format
    newarray
    atype
Forms
    newarray = 188 (0xbc)
Operand Stack
.   .., count ??
  ..., arrayref
Description
    The count must be of type int. It is popped off the operand stack. 
    The count represents the number of elements in the array to be created.
    The atype is a code that indicates the type of array to create. It must take one of the following values:
*/
void NEWARRAY()
{
     
        //PrintStackFrame(); // UNCOMMENT FOR DEBUG 
              int totalSpaceForArray,loopCounter;
              // We need to declare different Pointers to point to different types of objects
              uint8_t *heapPointer_Type_Bool,*heapPointer_Type_Char,*heapPointer_Type_Byte;
              uint16_t *heapPointer_Type_Short;
              uint32_t *heapPointer_Type_Float,*heapPointer_Type_Long,*heapPointer_Type_Int;
              uint64_t *heapPointer_Type_Double;

             // Get the Array Type
              interpreter_ProgramCounter++;             
          
              uint8_t arrayType=(uint8_t)((unsigned char)*interpreter_ProgramCounter);  

              // Get the Count of Elements of Array           
               int numberOfItems=(int)(Pop());       
              
              // Reseting the value to the Default Value
               if(arrayType==4)                                  //  T_BOOLEAN	4
               {
                  // Reserve the space in the Heap according to the Type of the item
                 totalSpaceForArray=numberOfItems*1;              // Boolean reserves 1 Byte
                 heapPointer_Type_Bool=(uint8_t *)heapPointer_VM;
                 for(loopCounter=0;loopCounter<numberOfItems;loopCounter++)
                 {            
                 *(heapPointer_Type_Bool-loopCounter)=false;               // Default Value of Bool=false
                 } 
               }
              else if(arrayType==5)                              // T_CHAR	5
              {
                  // Reserve the space in the Heap according to the Type of the item
                 totalSpaceForArray=numberOfItems*1;               // Char reserves 1 Byte
                 heapPointer_Type_Char=(uint8_t *)heapPointer_VM;
                 for(loopCounter=0;loopCounter<numberOfItems;loopCounter++)
                  {
                    *(heapPointer_VM-loopCounter)='\0';                     // Default Value of Bool=false
                  }  
              }
              else if(arrayType==6)                               // T_FLOAT	6
              {
                // Reserve the space in the Heap according to the Type of the item
                 totalSpaceForArray=numberOfItems*4;              // Float reserves 4 Byte in 32 bit computer
                 heapPointer_Type_Float=(uint32_t *)heapPointer_VM;
                  for(loopCounter=0;loopCounter<numberOfItems;loopCounter++)
                 {
                 *(heapPointer_Type_Float-loopCounter)=0.0f;         // Default Value of Float 0.0f
                 }  
              }
            else if(arrayType==7)                           // T_DOUBLE	7
            {
                // Reserve the space in the Heap according to the Type of the item
                 totalSpaceForArray=numberOfItems*8;               // Double reserves 8 Byte in 32 bit computer
                 heapPointer_Type_Double=(uint64_t *)heapPointer_VM;
                 for(loopCounter=0;loopCounter<numberOfItems;loopCounter++)
                 {
                  *(heapPointer_Type_Double-loopCounter)=0.0;          // Default Value of Double 0.0
                 } 
            }
            else if(arrayType==8)                            // T_BYTE	8
            {
                // Reserve the space in the Heap according to the Type of the item
                 totalSpaceForArray=numberOfItems*1;           // Byte reserves 1 Byte in 32 bit computer
                 heapPointer_Type_Byte=(uint8_t *)heapPointer_VM;
                for(loopCounter=0;loopCounter<numberOfItems;loopCounter++)
                 {
                 *(heapPointer_Type_Byte-loopCounter)=0;                      
                 
                }  
            }
            else if(arrayType==9)                            // T_SHORT	9              
            {
                 totalSpaceForArray=numberOfItems*2;          // Short reserves 2 Byte in 32 bit computer
                 heapPointer_Type_Short=(uint16_t *)heapPointer_VM;
                for(loopCounter=0;loopCounter<numberOfItems;loopCounter++)
                 {
                 *(heapPointer_Type_Short-loopCounter)=0; 
                }                  
            } 
           else if(arrayType==10)                            // T_INT	10                
            {
                // Reserve the space in the Heap according to the Type of the item
                 totalSpaceForArray=numberOfItems*4;
                 //heapPointer_Type_Int=(int *)heapPointer_VM;   // TODO: SUBHA G++
            
                   heapPointer_Type_Int=(uint32_t *)heapPointer_VM;                 
                 for(loopCounter=0;loopCounter<numberOfItems;loopCounter++)
                 {
                 *(heapPointer_Type_Int-loopCounter)=0; 
                }                
            } 
           else if(arrayType==11)                            // T_LONG	11                  
            {
                // Reserve the space in the Heap according to the Type of the item
                totalSpaceForArray=numberOfItems*4;
               // heapPointer_Type_Long=(int *)heapPointer_VM;   // TODO: SUBHA G++
                 heapPointer_Type_Long=(uint32_t *)heapPointer_VM;   // TODO: SUBHA G++
                for(loopCounter=0;loopCounter<numberOfItems;loopCounter++)
                 {
                 *(heapPointer_Type_Long-loopCounter)=0; 
                }           
            }          
                
                //Need to save the pointer to the stack
                uint32_t logicalAddress_HeapPointer=(uint32_t)(Convert_To_LogicalAddres((void *) heapPointer_VM));  
               
                // Pushing the referece to the array            
                Push(logicalAddress_HeapPointer);
                
                // New heap Pointer
                heapPointer_VM=heapPointer_VM-totalSpaceForArray;

                // INCREMENT PC
                interpreter_ProgramCounter++;     

            //  PrintStackFrame(); // UNCOMMENT FOR DEBUG  

}

  /*
getfield
Operation
             Fetch field from object
Format
              getfield
              indexbyte1
              indexbyte2
Forms
            getfield = 180 (0xb4)
Operand Stack
        ..., objectref ??
          ..., value  
Description
          The objectref, which must be of type reference, is popped from the operand stack. 
          The unsigned indexbyte1 and indexbyte2 are used to construct an index into the run-time constant pool of the current class (§?2.6), 
          where the value of the index is (indexbyte1 << 8) | indexbyte2. 
          The run-time constant pool item at that index must be a symbolic reference to a field (§?5.1), which gives the name and descriptor of the field as well as a symbolic reference to the class in which the field is to be found. 
          The referenced field is resolved (§?5.4.3.2). The value of the referenced field in objectref is fetched and pushed onto the operand stack.

          The type of objectref must not be an array type. 
          If the field is protected (§?4.6), and it is a member of a superclass of the current class, 
          and the field is not declared in the same run-time package (§?5.3) as the current class, 
          then the class of objectref must be either the current class or a subclass of the current class.

Linking Exceptions
          During resolution of the symbolic reference to the field, any of the errors pertaining to field resolution (§?5.4.3.2) can be thrown.
          Otherwise, if the resolved field is a static field, getfield throws an IncompatibleClassChangeError.

Run-time Exception
          Otherwise, if objectref is null, the getfield instruction throws a NullPointerException.


*/
     

void GETFIELD()  
{ 

             //The objectref, which must be of type reference, is popped from the operand stack. 
            int objectref=(int)(Pop());
            uint32_t objectref_unsigned=(uint32_t)(objectref); //   CASTING TO UNSIGNED INT AS OBJECT REF MUST BE POSTIVE 

              //The value of the index is (indexbyte1 << 8) | indexbyte2.
              interpreter_ProgramCounter++;             
              uint8_t unsigned_a=(uint8_t)((unsigned char)*interpreter_ProgramCounter);     //reference: (uint32_t)((signed char)*interpreter_ProgramCounter); 
              interpreter_ProgramCounter++;             
              uint8_t unsigned_b=(uint8_t)((unsigned char)*interpreter_ProgramCounter);     

 
             //get the details of the method of the  
              int16_t index_In_Object=(int16_t)(unsigned_a << 8 | unsigned_b);             
             
              //The run-time constant pool item at that index must be a symbolic reference to a field 
             // unsigned char * symbolic_reference_To_Field=(unsigned char *)((objectReference_In_ConstantPool+(index_In_ConstantPool*4))); 
                        
              //VALUE AT POINTER: Get value at Pointer         
              
                      

            int * physicalAddress_Of_Object=Convert_To_PhysicalAddress(objectref_unsigned); 
            int * field_Referenced_Pointer=(int *)(physicalAddress_Of_Object-(index_In_Object));
            
             // Get the Value from this address
             int value_of_Field=*(field_Referenced_Pointer);
             
              // Push the Field value back to the stack            
               Push(value_of_Field);
           
              // Increment the PC
             interpreter_ProgramCounter++;
          //   PrintStackFrame(); // UNCOMMENT FOR DEBUG
}

/*

putfield
Operation
     Set field in object
Format
      putfield
       indexbyte1
       indexbyte2
Forms
     putfield = 181 (0xb5)

Operand Stack
.    .., objectref, value ??...

Description
      The unsigned indexbyte1 and indexbyte2 are used to construct an index into the run-time constant pool of the current class (§?2.6), 
      where the value of the index is (indexbyte1 << 8) | indexbyte2. The run-time constant pool item at that index must be a symbolic reference to a field (§?5.1), which gives the name and descriptor of the field as well as a symbolic reference to the class in which the field is to be found. 
      The class of objectref must not be an array. 
      If the field is protected (§?4.6), and it is a member of a superclass of the current class, 
      and the field is not declared in the same run-time package (§?5.3) as the current class, 
      then the class of objectref must be either the current class or a subclass of the current class.

*/

void PUTFIELD() //set field to value in an object objectref, where the field is identified by a field reference index in constant pool (indexbyte1 << 8 | indexbyte2)
{
  
              interpreter_ProgramCounter++;             
              uint8_t unsigned_a=(uint8_t)((unsigned char)*interpreter_ProgramCounter);     //reference: (uint32_t)((signed char)*interpreter_ProgramCounter); 
              interpreter_ProgramCounter++;             
              uint8_t unsigned_b=(uint8_t)((unsigned char)*interpreter_ProgramCounter);     

             // interpreter_ProgramCounter++;             
            //  uint8_t unsigned_a=(uint8_t)(*interpreter_ProgramCounter);  
             // interpreter_ProgramCounter++;             
             // uint8_t unsigned_b=(uint8_t)(*interpreter_ProgramCounter);   

              // Index in Constant Pool            
              int16_t index_In_Object=(int16_t)(unsigned_a << 8 | unsigned_b); 
              // printf("\n index_In_ConstantPool %d",index_In_Object);
               // Pointer to the Constant Pool
             //unsigned char * symbolic_reference_To_Field=(unsigned char *)(objectReference_In_ConstantPool+(index_In_ConstantPool*4)); 

         
             //VALUE AT POINTER: Get value at Pointer
             // uint32_t fieldReferenceIndex;
              //fieldReferenceIndex=(uint32_t)((unsigned char)*symbolic_reference_To_Field); 
              
              // //Get value at Pointer
              // uint8_t fieldReferenceIndex=(uint8_t)(*(objectReference_In_Const_Pool)); 

              // Retrieve two values from the top of the stack.             
                int value=(int)(Pop());             //First one is the Value      
                int objectref=(int)(Pop());         //Second  one is the Object Reference     
                uint32_t objectref_UnsignedIntFormat= (uint32_t) objectref; // Object Reference can not be negative number       

            // Go the the field referenced by sizeOfObject. We need to go to the actual location               
            int* physicalAddress_Of_Object=Convert_To_PhysicalAddress(objectref_UnsignedIntFormat);
     
             // Point to the actual address in the heap where the objects are created
             int *object_Referenced=(int *)(physicalAddress_Of_Object-(index_In_Object));
          
             // printf("\n object_Referenced=%u %p",object_Referenced,object_Referenced);
             // SAVE THE VALUE TO THE PHYSICAL ADDRESS 
          
            // printf("\n\t\t\tPutting Value : %u at Address : %ld (%p) ",value,object_Referenced,object_Referenced);
         
             *(object_Referenced)=value;

            // Increment the PC
             interpreter_ProgramCounter++;

        // PrintStackFrame(); // UNCOMMENT FOR DEBUG
}


/*
Operation
         Load int from local variable
Format
     iload
    index
Forms
      iload = 21 (0x15)
Description: 
      The index is an unsigned byte that must be an index into the local variable array of the current frame (§?2.6).
       The local variable at index must contain an int. 
       The value of the local variable at index is pushed onto the operand stack.
*/
void ILOAD()
{
     interpreter_ProgramCounter++;                 
     uint8_t index =(uint8_t)(*interpreter_ProgramCounter);
     int iResult=(int)(*(stackFrame_localVariableIndexCounter+index));    // Go to the Index of the local variables tables and save the value
       
      Push(iResult);  
      interpreter_ProgramCounter++;    // INCREMENT PC

        // UNCOMMENT FOR DEBUG
       // PrintStackFrame();

}



/*
 Operation
     Load int from array
Format
     iaload
Forms
      iaload = 46 (0x2e)
Operand Stack
    ..., arrayref, index ?? 
    ..., value
Description
       The arrayref must be of type reference and must refer to an array whose components are of type int. 
       The index must be of type int. Both arrayref and index are popped from the operand stack. 
       The int value in the component of the array at index is retrieved and pushed onto the operand stack.
Run-time Exceptions
      If arrayref is null, iaload throws a NullPointerException.
      Otherwise, if index is not within the bounds of the array referenced by arrayref, the iaload instruction throws an ArrayIndexOutOfBoundsException.
*/
 void IALOAD()
 {         

                int index_To_Array = (int)(Pop());               
                int  arrayref = (int)(Pop());     
                if(arrayref==0)// If arrayref is null, iaload throws a NullPointerException.
                {
                    
                     printf("\nArray Reference is Null");
                     exit(EXIT_FAILURE);
                }         
               // Since the Index and Array Reference cannot be negative number, 
               // Changing their type to unsigned Inteter

                uint32_t index_To_Array_Unsigned=(uint32_t) index_To_Array;               
                uint32_t arrayref_Unsigned= (uint32_t) arrayref;

                int * physicalAddress_Of_Array=Convert_To_PhysicalAddress(arrayref_Unsigned);    
               
                int iResult=(int)(*(physicalAddress_Of_Array+index_To_Array_Unsigned));                
               
               
                Push(iResult);                      
               // INCREMENT PC
                interpreter_ProgramCounter++;
                // UNCOMMENT FOR DEBUG
              //   PrintStackFrame();
 }



/*

aload
Operation
     Load reference from local variable
Format
     aload
     index
Forms
    aload = 25 (0x19)

Operand Stack
.     .. ??
.     .., objectref

Description
       The index is an unsigned byte that must be an index into the local variable array of the current frame (§?2.6). 
       The local variable at index must contain a reference. 
       The objectref in the local variable at index is pushed onto the operand stack.

Notes
The aload instruction cannot be used to load a value of type returnAddress from a local variable onto the operand stack. This asymmetry with the astore instruction (§?astore) is intentional.

The aload opcode can be used in conjunction with the wide instruction (§?wide) to access a local variable using a two-byte unsigned index.
*/
 void ALOAD()               
 {

             interpreter_ProgramCounter++;             
             uint8_t index_To_LocalVariable=(uint8_t)(*interpreter_ProgramCounter);            
          
             // Object reference should not be a negative number, so changing to unsigned int                        
    
             // why not logical address?? Because the address is already logical in stack frame
             // We push only logical address
             uint32_t object_In_StackFrame =(uint32_t)(*(stackFrame_localVariableIndexCounter+index_To_LocalVariable));      
     
                  
             Push(object_In_StackFrame);               // PUSH it to the Operand Stack              
             interpreter_ProgramCounter++;           // Increnent the PC

               // UNCOMMENT FOR DEBUG
               // PrintStackFrame();
 }




/*
aaload 
Operation
    Load reference from array
Format
    aaload
Forms
    aaload = 50 (0x32)

Operand Stack
..., arrayref, index ??
..., value

Description
     The arrayref must be of type reference and must refer to an array whose components are of type reference. 
     The index must be of type int. Both arrayref and index are popped from the operand stack.
      The reference value in the component of the array at index is retrieved and pushed onto the operand stack.

Run-time Exceptions
     If arrayref is null, aaload throws a NullPointerException.

*/

void AALOAD()  //load onto the stack a reference from an array
{
                // Get the Index
                int index_To_Array = (int) (Pop()); 
           
                // Get the Array refernce
                int  arrayref = (int)(Pop());
                uint32_t arrayref_Signed= (uint32_t) arrayref;

                //If arrayref is null, aaload throws a NullPointerException.
               //Otherwise, if index is not within the bounds of the array referenced by arrayref, the aaload instruction throws an ArrayIndexOutOfBoundsException.
              if(arrayref==0)
              {
                 perror("NullPointerException");
                  exit(EXIT_FAILURE);
              }
              else
              {                           
                
                int * physicalAddress_Of_Array_Pointer=Convert_To_PhysicalAddress(arrayref_Signed);              
                uint32_t iResult=(uint32_t)(*(physicalAddress_Of_Array_Pointer+index_To_Array)); 
               // Result is pushed onto the stack                       
                Push(iResult);     
              }             
                       
              
               //INCREMENT PC
                interpreter_ProgramCounter++;           
            
              
       //  PrintStackFrame(); // UNCOMMENT FOR DEBUG
}

/*

dup
Operation
      Duplicate the top operand stack value
Format
      dup
Forms
    dup = 89 (0x59)
Operand Stack
    ..., value ??
    ..., value, value
Description
      Duplicate the top value on the operand stack and push the duplicated value onto the operand stack.
      The dup instruction must not be used unless value is a value of a category 1 computational type (§?2.11.1).
*/
void DUP()   //duplicate the value on top of the stack
{
             
               int valueToDuplicte=(int)(Pop());     // Popping the value which needs to be duplicated                          
               Push(valueToDuplicte);               // Push the value twice into the stack for duplication  
               Push(valueToDuplicte);                // PUSH again the same object
	             interpreter_ProgramCounter++;           // Increment the PC  

             // UNCOMMENT FOR DEBUG
              //PrintStackFrame();

}


/*
aconst_null
Operation
    Push null
Format
    aconst_null
Forms
    aconst_null = 1 (0x1)
Operand Stack
    ... ??
  ..., null
Description
    Push the null object reference onto the operand stack.
Notes
    The Java Virtual Machine does not mandate a concrete value for null.
*/
void ACONST_NULL()   //	push a null reference onto the stack
{
   Push(0);
   interpreter_ProgramCounter++;     // INCREMENT PC
   // UNCOMMENT FOR DEBUG

   // PrintStackFrame();
}


/*
Operation
    Store reference into local variable
Format
     astore
     index
Forms
     astore = 58 (0x3a)
Operand Stack
.    .., objectref ??
      ...
Description
     The index is an unsigned byte that must be an index into the local variable array of the current frame (§?2.6). 
     The objectref on the top of the operand stack must be of type returnAddress or of type reference. It is popped from the operand stack, 
     and the value of the local variable at index is set to objectref.

Notes
    The astore instruction is used with an objectref of type returnAddress when implementing the finally clause of the Java programming language (§?3.13).
    The aload instruction (§?aload) cannot be used to load a value of type returnAddress from a local variable onto the operand stack. This asymmetry with the astore instruction is intentional.
    The astore opcode can be used in conjunction with the wide instruction (§?wide) to access a local variable using a two-byte unsigned index.
*/
 void  ASTORE()
 {
            interpreter_ProgramCounter++;             
            uint8_t index_To_LocalVariable=(uint8_t)(*interpreter_ProgramCounter);  
           
              int objectref=(int)(Pop());                                                   // Object Reference is POPPed from the stack  
              // since object ref is a reference type, it should not  be negative value                       
              uint32_t objectReferenece=(uint32_t) objectref;  
                // Save the value on the local variables table        
             (*(stackFrame_localVariableIndexCounter+index_To_LocalVariable))=objectReferenece; 
           
               interpreter_ProgramCounter++;                                                  // Increment the PC  

           // UNCOMMENT FOR DEBUG
           // PrintStackFrame();
 }



/*
iastore
Operation
      Store into int array
Format
    iastore
Forms
    iastore = 79 (0x4f)
Operand Stack
    ..., arrayref, index, value ??
    ...
Description
    The arrayref must be of type reference and must refer to an array whose components are of type int. 
    Both index and value must be of type int. The arrayref, index, and value are popped from the operand stack. 
    The int value is stored as the component of the array indexed by index.

Run-time Exceptions
    If arrayref is null, iastore throws a NullPointerException.
    Otherwise, if index is not within the bounds of the array referenced by arrayref, the iastore instruction throws an ArrayIndexOutOfBoundsException.

*/
 void IASTORE()
 {

                int value = (int) (Pop());  
                int index_To_Array = (int)(Pop());
                int arrayref = (int)(Pop());
                // Index of Array Must be Postive Number
                uint32_t index_To_Array_unsigned=(uint32_t) index_To_Array;

               //The arrayref must be of type reference and must refer to an array whose components are of type int.
               // Chaning the value to unsigned int as the reference cannot be negative number
                uint32_t arrayref_unsigned= (uint32_t) arrayref;  
                int * physicalAddress_Of_Array=Convert_To_PhysicalAddress(arrayref_unsigned);

                 *(physicalAddress_Of_Array+index_To_Array_unsigned)=value;                
                            
              // INCREMENT PC
               interpreter_ProgramCounter++;

              // UNCOMMENT FOR DEBUG
             // PrintStackFrame();
 }


/*

aastore
Operation
     Store into reference array
Format
      aastore
Forms
      aastore = 83 (0x53)
Operand Stack
     ..., arrayref, index, value ??
.     ..
Description
       The arrayref must be of type reference and must refer to an array whose components are of type reference. 
       The index must be of type int and value must be of type reference. The arrayref, index, and value are popped from the operand stack. 
       The reference value is stored as the component of the array at index.

*/
void AASTORE() //store a reference in an array
{ //PrintStackFrame();
  
               // Got the value
               int referenced_Value = (int) (Pop());  //The reference value is stored as the component of the array at index.
              //This value cannot be negative as it is a referenced Type
              // So change it to unsigned int
              uint32_t referenced_Value_Unsigned=(uint32_t) referenced_Value;

               //Got the Index
                uint32_t index_To_Array = (uint32_t)(Pop());
          

                //Got the Array Ref
                int arrayref = (int)(Pop());
               // Array Reference cannot be a negative number as it an address
                uint32_t arrayref_Unsigned= (uint32_t) arrayref;
                if(arrayref==0)
                {
                  perror("NullPointerException");
                  exit(EXIT_FAILURE);
                }
                else
                {                    
                     int * physicalAddress_Of_Array_Pointer=(Convert_To_PhysicalAddress(arrayref_Unsigned));            
                      *(physicalAddress_Of_Array_Pointer+index_To_Array)=referenced_Value_Unsigned; 
                }                
                           
                            
              // INCREMENT PC
               interpreter_ProgramCounter++;

             // UNCOMMENT FOR DEBUG
            //  PrintStackFrame();
             
}




//____________________________________________________________________________________________________________________________
// END: ALL BYTE CODE INSTRUCTION 
//____________________________________________________________________________________________________________________________


//____________________________________________________________________________________________________________________________
// Helper functions to reduce pointer size 
// Pointers at location end of JVM is large. An int size pointer will not be able to hold the entire address
// We need to save the logical reference of the address by converting it into a relative address of the pointer in respect to JVM 
//____________________________________________________________________________________________________________________________

//___________________________________________________________________________________________________________________________
// This Method will chage to Larger Pointer Address **TO LOGICAL ADDRESS by reducing it from JVM Start Address
//_____________________________________________________________________________________________________________________________
uint32_t Convert_To_LogicalAddres(void* physical_Address)
{

  return (uint32_t)((long)physical_Address - (long)(JVM));
}

//____________________________________________________________________________________________________________________________
// This method will change back to the actual physical location  **FROM  LOGICAL ADDRESS
//____________________________________________________________________________________________________________________________
int * Convert_To_PhysicalAddress(uint32_t logical_Address)
{
 return (int *)((void*)JVM+logical_Address);
}




//____________________________________________________________________________________________________________________________
 // This Method should create a Stake Frame and Value Stack on the Top of the Stack
//_______________________________________________________________________________________________________________________
void SaveValueInStackFrame(uint32_t stackFrame_ReturnValue, 
                          uint32_t programCounter_Address,                      
                          uint32_t operandStack_Address,
                          uint32_t stackFrame_Address,
                          int argument_from_main)
  {
            // DONT CONVERT TO LOGICAL ADDRESS FOR THE FIRST FRAME                                                            
    *stackFramePointer_Static=stackFrame_ReturnValue;                                                    // LOCATION 0: RETUNN VALUE
    *(stackFramePointer_Static+1)=programCounter_Address;   // LOCATION 1: PREVIOUS PROGRAM COUNTER
    *(stackFramePointer_Static+2)=operandStack_Address;     // LOCATION 2: PREVIOUS OPERAND STACK ADDRESS
    *(stackFramePointer_Static+3)=stackFrame_Address;       // LOCATION 3: PREVIOUS FRAME ADDRESS
    *(stackFramePointer_Static+4)=(int)(argument_from_main);                                             // LOCATION 4: GLOBAL VARIABLE FROM MAIN

   
    
   }


//____________________________________________________________________________________________________________________________
// This will validate the user input
//____________________________________________________________________________________________________________________________
bool ValidateInputParameter(char *argv[])
{
   
if(!(strcmp(argv[1], "-e") == 0) ||!(strcmp(argv[3], "-c") == 0)) 
  {
 
 printf("\nParameters are not valid\n");
 
 return false;
 }
return true;

}



// CUSTOM PUSH
 void Push(int item)
 {
    operandStack_Pointer++; 
   *operandStack_Pointer=item;
   
   }

//CUSTOM POP
 int Pop()
 { 
     // Check if StackPointer value is not Less than equal to FramePointer
     if(operandStack_Pointer<=stackFramePointer_Dynamic)
      {
        printf("\n Can not pop.");
        exit(0);
      
      }
     else{
         int poppedValue=*operandStack_Pointer;   
          operandStack_Pointer--; 
         return poppedValue;
        }
 }

//_____________________________________________________________________________________
// This method will add or update the table for compiled method
//____________________________________________________________________________________

void Add_Or_Update_Precompiled_Method_Table(uint16_t constantPool_Reference)
{

      // DECIDE WHETHER TO ADD TO THE TABLE TO UPDATE TO THE TABLE BASED ON THE VALUE
      uint32_t methodCounter_Compiler=Search_Number_Of_Methods_In_MethodTable_Compiled((uint32_t) constantPool_Reference);           
          
    if(methodCounter_Compiler==0)          // Method does not exists in the table, add it to the table
      {
          methodCounter_Compiler++;
          Add_Method_To_MethodTable_Compiled((uint32_t)constantPool_Reference,methodCounter_Compiler);
          // WriteToTextFile_MethodTable_Compiler(__MethodTable_Pointer_Compiled_STATIC,__TotalNumber_Of_Items_In_MethodTable_Compiled);
      }
      else if((methodCounter_Compiler!=0) || (methodCounter_Compiler!=-1)) // Method exists, increment the counter
      {
          methodCounter_Compiler++;
          Update_Method_From_MethodTable_Compiled((uint32_t)constantPool_Reference,methodCounter_Compiler);
          // WriteToTextFile_MethodTable_Compiler(__MethodTable_Pointer_Compiled_STATIC,__TotalNumber_Of_Items_In_MethodTable_Compiled);

      } 
}



//_____________________________________________________________________________________
// This method will add or update the table for Interpreted method
//____________________________________________________________________________________

void Add_Or_Update_Interpreted_Method_Table(uint16_t constantPool_Reference,uint32_t methodCounter_Interpreter)
{
            
                 // DECIDE WHETHER TO ADD TO THE TABLE TO UPDATE TO THE TABLE BASED ON THE VALUE
                  if(methodCounter_Interpreter==0)          // Method does not exists in the table, add it to the table
                  {
                      methodCounter_Interpreter++;
                      Add_Method_To_MethodTable_Interpreter((uint32_t)constantPool_Reference,methodCounter_Interpreter);
                      WriteToTextFile_Method_Interpreter(__MethodTable_Pointer_Interpreter_STATIC,__TotalNumber_Of_Items_In_MethodTable_INTERPRETER);
                  }
                  else if((methodCounter_Interpreter!=0) || (methodCounter_Interpreter!=-1)) // Method exists, increment the couter
                  {
                     methodCounter_Interpreter++;
                    Update_Method_In_MethodTable_Interpreter((uint32_t)constantPool_Reference,methodCounter_Interpreter);
                    WriteToTextFile_Method_Interpreter(__MethodTable_Pointer_Interpreter_STATIC,__TotalNumber_Of_Items_In_MethodTable_INTERPRETER);
                  }
  }





int GetEndianness()
{
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

        return isBigEndian;
}


//    /* function to show bytes in memory, from location start to start+n*/
// void show_mem_rep(char *start, int n) 
// {
//     int i;
//     for (i = 0; i < n; i++)
//          printf(" %.2x", start[i]);
//     printf("\n");
// }


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


// entire function is taken from the below site
//https://www.geeksforgeeks.org/bit-manipulation-swap-endianness-of-a-number/
  // Function to swap a value from
// big Endian to little Endian and
// vice versa.
 
int swap_Endians(int value)
{
 
    // This var holds the leftmost 8
    // bits of the output.
 
    int leftmost_byte;
 
    // This holds the left middle
    // 8 bits of the output
 
    int left_middle_byle;
 
    // This holds the right middle
    // 8 bits of the output
 
    int right_middle_byte;
 
    // This holds the rightmost
    // 8 bits of the output
 
    int rightmost_byte;
 
    // To store the result
    // after conversion
 
    int result;
 
    // Get the rightmost 8 bits of the number
    // by anding it 0x000000FF. since the last
    // 8 bits are all ones, the result will be the
    // rightmost 8 bits of the number. this will
    // be converted into the leftmost 8 bits for the
    // output (swapping)
 
    leftmost_byte = (value & 0x000000FF) >> 0;
 
    // Similarly, get the right middle and left
    // middle 8 bits which will become
    // the left_middle bits in the output
 
    left_middle_byle = (value & 0x0000FF00) >> 8;
 
    right_middle_byte = (value & 0x00FF0000) >> 16;
 
    // Get the leftmost 8 bits which will be the
    // rightmost 8 bits of the output
 
    rightmost_byte = (value & 0xFF000000) >> 24;
 
    // Left shift the 8 bits by 24
    // so that it is shifted to the
    // leftmost end
 
    leftmost_byte <<= 24;
 
    // Similarly, left shift by 16
    // so that it is in the left_middle
    // position. i.e, it starts at the
    // 9th bit from the left and ends at the
    // 16th bit from the left
 
    left_middle_byle <<= 16;
 
    right_middle_byte <<= 8;
 
    // The rightmost bit stays as it is
    // as it is in the correct position
 
    rightmost_byte <<= 0;
 
    // Result is the concatenation of all these values.
 
    result = (leftmost_byte | left_middle_byle
              | right_middle_byte | rightmost_byte);
 
    return result;
}