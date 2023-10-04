#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<inttypes.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h> 
#include <stdbool.h>
#include<arpa/inet.h>
#include <assert.h>



// #define CAPACITY_METHOD_TABLE_INTERPRETER 800000 // Size of the Hash Table
#define CAPACITY_METHOD_TABLE_INTERPRETER 80 // Size of the Hash Table

#define CAPACITY_INTERPRETER_WRITE 500000 // Size of the Hash Table

// GLOBAL VARIABLE FOR ROOT SET
uint32_t * __MethodTable_Pointer_Interpreter_STATIC=NULL;           // This will points to the method Table , it will always points to the starting of the ClassPool
uint32_t * __MethodTable_Pointer_Interpreter_DYNAMIC=NULL;           // This will points to the method Table, it will always points to the current free space of ClassPool
uint32_t __TotalNumber_Of_Items_In_MethodTable_INTERPRETER=0;             // Counter to keep track of number of items in the table



uint32_t * __Pointer_Interpreter_Write_STATIC=NULL;           
uint32_t * __Pointer_Interpreter_Write_DYNAMIC=NULL;          
uint32_t __TotalNumber_Of_Items_In_Interpreter_Write=0;     



// TABLE
//           METHOD IDENTIFIER  |     NUMBER OF INTERPRETED METHOD | 
//             1                |             10                   |   
//             2                |             60                   |   
//____________________________________________________________________________________
//Initialized Method Invocation Table
//_________________________________________________________________________________
void Initialize_Method_In_MethodTable_Interpreter()
{
    uint32_t i=0; 
      // Changed to int to save negative value
    int * methodTable_Pointer_Interpreter=(int * )__MethodTable_Pointer_Interpreter_STATIC;   //ALWAYS START FROM THE STARTRING OF THE MEMORY LOCATION    
   
  //  uint32_t totalCapacity=CAPACITY_METHOD_TABLE_INTERPRETER;
    for(i=0;i<CAPACITY_METHOD_TABLE_INTERPRETER;i=i+2)
    {  
              *(methodTable_Pointer_Interpreter+i)=-1;         // Assign -1 to  Method Identifier / Index in Cosntat Pool
              *(methodTable_Pointer_Interpreter+i+1)=-1;       // Assign -1 to  Number of Interpreted Method                                    
             
    }
 
}

//____________________________________________________________________________________
//Add Objects to Method Invocation Table
//_________________________________________________________________________________
void Add_Method_To_MethodTable_Interpreter(uint32_t method_Identifier, uint32_t method_Counter)
{
   int method_Identifier_INT=(int)method_Identifier;  // changed to int to validate the negative number
   int method_Counter_INT=(int)method_Counter;        // changed to int to validate the negative number

    if((method_Identifier_INT!=-1) && ((int)method_Counter_INT!=-1))                    // ONLY ADD POSTION NUMBER, WE WILL ADD -1 WHEN WE DELETE
    {

    *__MethodTable_Pointer_Interpreter_DYNAMIC=method_Identifier;                                // Constant Poo Index=method_Identifier
    __MethodTable_Pointer_Interpreter_DYNAMIC=__MethodTable_Pointer_Interpreter_DYNAMIC+1;       //INCREMENT


    *__MethodTable_Pointer_Interpreter_DYNAMIC=method_Counter;                                     // Number of Interpretd methods invoked
    __MethodTable_Pointer_Interpreter_DYNAMIC=__MethodTable_Pointer_Interpreter_DYNAMIC+1;          //INCREMENT
        

    __TotalNumber_Of_Items_In_MethodTable_INTERPRETER++;
    __TotalNumber_Of_Items_In_MethodTable_INTERPRETER++;
    
    } 

}


//____________________________________________________________________________________
//Search Objects In Method Invocation Table
//_________________________________________________________________________________
uint32_t Search_Number_Of_Methods_In_MethodTable_Interpreter(uint32_t method_Identifier)
{
 uint32_t i=0;
 
 uint32_t method_Identifier_In_MethodTable=0;
 uint32_t methodCounter_In_MethodTable=0; 
 int method_Identifier_In_MethodTable_INT=0;   // int to validate negative number

 uint32_t * methodTable_Pointer_Interpreter=(uint32_t *)__MethodTable_Pointer_Interpreter_STATIC;   //ALWAYS START FROM THE STARTRING OF THE MEMORY LOCATION 
  

    for(i=0;i<__TotalNumber_Of_Items_In_MethodTable_INTERPRETER;i=i+2)
    {    
     
      method_Identifier_In_MethodTable_INT=(int)(*(methodTable_Pointer_Interpreter+i));           // Check negative value
     
          
      if(method_Identifier_In_MethodTable_INT!=-1)                           
      {        
         method_Identifier_In_MethodTable=*(methodTable_Pointer_Interpreter+i);

          if(method_Identifier==method_Identifier_In_MethodTable)
          {
              methodCounter_In_MethodTable=*(methodTable_Pointer_Interpreter+i+1);                // Address saved after one object
             
              break;
          }
        
      }
      else
      {
        
      }
      
    }
    return methodCounter_In_MethodTable;
}



//____________________________________________________________________________________
//UPDATE Method Entry IN Method Table
//_________________________________________________________________________________
void Update_Method_In_MethodTable_Interpreter(uint32_t method_Identifier,uint32_t method_Counter)
{
    uint32_t i=0; 
    uint32_t method_Identifier_In_MethodTable=0;
    //uint32_t methodCounter_In_MethodTable=0; 
    int method_Identifier_In_MethodTable_INT=0;    // convert to int to validate negtive number

    uint32_t * methodTable_Pointer_Interpreter=(uint32_t *)__MethodTable_Pointer_Interpreter_STATIC;   //ALWAYS START FROM THE STARTRING OF THE MEMORY LOCATION    

    for(i=0;i<__TotalNumber_Of_Items_In_MethodTable_INTERPRETER;i=i+2)
    {    
     
      method_Identifier_In_MethodTable_INT=(int)(*(methodTable_Pointer_Interpreter+i));           // Check negative value     
          
      if(method_Identifier_In_MethodTable_INT!=-1)                           
      {        
         method_Identifier_In_MethodTable=*(methodTable_Pointer_Interpreter+i);

          if(method_Identifier==method_Identifier_In_MethodTable)
          {         
              *(methodTable_Pointer_Interpreter+i+1)=method_Counter; 
              break;
          }        
      }
      else
      {
        
      }
      
    }

}




//____________________________________________________________________________________
//Delete Method Entry from Method Table
//_________________________________________________________________________________
void Delete_Method_From_MethodTable_Interpreter(uint32_t method_Identifier)
{
    uint32_t i=0; 
    uint32_t method_Identifier_In_MethodTable=0;
  //  uint32_t methodCounter_In_MethodTable=0; 
    int method_Identifier_In_MethodTable_INT=0; // convert to int to validate negtive number

    uint32_t * methodTable_Pointer_Interpreter=(uint32_t *)__MethodTable_Pointer_Interpreter_STATIC;   //ALWAYS START FROM THE STARTRING OF THE MEMORY LOCATION    

    for(i=0;i<__TotalNumber_Of_Items_In_MethodTable_INTERPRETER;i=i+2)
    {    
     
      method_Identifier_In_MethodTable_INT=(int)(*(methodTable_Pointer_Interpreter+i));           // Check negative value
     
          
      if(method_Identifier_In_MethodTable_INT!=-1)                           
      {        
         method_Identifier_In_MethodTable=*(methodTable_Pointer_Interpreter+i);

          if(method_Identifier==method_Identifier_In_MethodTable)
          {
              *(methodTable_Pointer_Interpreter+i)=-1;         // Assign -1 to ID
              *(methodTable_Pointer_Interpreter+i+1)=-1;       // Assign -1 to Address
             
              break;
          }
        
      }
      else
      {
        
      }
      
    }

}

void PrintMethodTable_Interpreter(uint32_t number_of_items)
{


     uint32_t * methodTable_Pointer_Interpreter=(uint32_t *)__MethodTable_Pointer_Interpreter_STATIC;   //ALWAYS START FROM THE STARTRING OF THE MEMORY LOCATION   

     uint32_t loopCounter;

    for(loopCounter=0;loopCounter<number_of_items;loopCounter=loopCounter+2)
    {

       printf("\nIndex [%d]  |\t\tMethod ID     : %d\t\t|          Address: %ld (%p) \n",loopCounter,*(methodTable_Pointer_Interpreter+loopCounter),(long int)(methodTable_Pointer_Interpreter+loopCounter),(void *)(methodTable_Pointer_Interpreter+loopCounter));
       printf("\nIndex [%d]  |\t\tCounter       : %d\t\t|          Address: %ld (%p) \n",loopCounter+1,*(methodTable_Pointer_Interpreter+loopCounter+1),(long int)(methodTable_Pointer_Interpreter+loopCounter+1),(void *)(methodTable_Pointer_Interpreter+loopCounter+1));
    }

}


// write to method table interpreter
void WriteToTextFile_Method_Interpreter(uint32_t* pointer_to_contents,uint32_t lenght_of_contents)
{
 
     if(lenght_of_contents>0)
     {
        FILE *filePointer0;     

        uint32_t i; 
        // Open the Text File 
        filePointer0=fopen("Output_MethodTable_Interpreter.txt","w"); 
        // Validate if File can be opened or not

        if(filePointer0 == NULL)
        {
               exit(1);
        }
         
        for(i=0;i<lenght_of_contents;i++)
        {
            fprintf(filePointer0, "%d\n",*(pointer_to_contents+i));
            
        }
        fclose(filePointer0);

     }
     
}

//________________________________________________________________________________________________
// If a method is refered for the first time, add it
//If there is duplicate entry then update it
//_________________________________________________________________________________________________
void Add_Entry_To_Method_Table_Interpreter(uint32_t method_Identifier,uint32_t method_Counter)
{

   int method_Identifier_INT=(int)method_Identifier;  // changed to int to validate the negative number
   int method_Counter_INT=(int)method_Counter;        // changed to int to validate the negative number



    uint32_t i=0; 
    uint32_t method_Identifier_In_MethodTable=0;
    //uint32_t methodCounter_In_MethodTable=0; 
    int method_Identifier_In_MethodTable_INT=0;    // convert to int to validate negtive number

    uint32_t * methodTable_Pointer_Interpreter=(uint32_t *)__MethodTable_Pointer_Interpreter_STATIC;   //ALWAYS START FROM THE STARTRING OF THE MEMORY LOCATION  
   
   if(method_Counter==0)                             // Method is refered for the first time, add the method to the method table
   {
           if((method_Identifier_INT!=-1) && ((int)method_Counter_INT!=-1))                         // ONLY ADD POSTION NUMBER, WE WILL ADD -1 WHEN WE DELETE
          {

          *__MethodTable_Pointer_Interpreter_DYNAMIC=method_Identifier;                                // Constant Poo Index=method_Identifier
          __MethodTable_Pointer_Interpreter_DYNAMIC=__MethodTable_Pointer_Interpreter_DYNAMIC+1;       //INCREMENT

         // increment the method counter before add
          method_Counter++;  
          *__MethodTable_Pointer_Interpreter_DYNAMIC=method_Counter;                                     // Number of Interpretd methods invoked
          __MethodTable_Pointer_Interpreter_DYNAMIC=__MethodTable_Pointer_Interpreter_DYNAMIC+1;          //INCREMENT
              

          __TotalNumber_Of_Items_In_MethodTable_INTERPRETER++;
          __TotalNumber_Of_Items_In_MethodTable_INTERPRETER++;
          
          }
   }
   else if(method_Counter>0)
   {
       for(i=0;i<__TotalNumber_Of_Items_In_MethodTable_INTERPRETER;i=i+2)
        {            
          method_Identifier_In_MethodTable_INT=(int)(*(methodTable_Pointer_Interpreter+i));           // Check negative value     
              
          if(method_Identifier_In_MethodTable_INT!=-1)                           
          {        
            method_Identifier_In_MethodTable=*(methodTable_Pointer_Interpreter+i);

              if(method_Identifier==method_Identifier_In_MethodTable)
              {       
                // increment the method counter before add
                  method_Counter++;  
                  *(methodTable_Pointer_Interpreter+i+1)=method_Counter; 
                  break;
              }        
          }
          else
          {
            
          }
          
        }
   }  

}












//____________________________________________________________________________________
//Add entry to hold the value of write operation of interpreter
//_________________________________________________________________________________
void Add_Entry_To_Interpreter_Write(int output)
{

    *__Pointer_Interpreter_Write_DYNAMIC=output;                               
    __Pointer_Interpreter_Write_DYNAMIC=__Pointer_Interpreter_Write_DYNAMIC+1;      
     
    __TotalNumber_Of_Items_In_Interpreter_Write++; 


}




// write to method table compiler
void WriteToTextFile_Output_InterpreterWrite(uint32_t * pointer_to_contents1,uint32_t lenght_of_contents1)
{
    if(lenght_of_contents1>0)
    {
        FILE *filePointer1;
       // int fileLength1; 

        uint32_t i; 
        // Open the Text File 
        filePointer1=fopen("Output_InterpreterWrite.txt","w"); 
        // Validate if File can be opened or not

        if(filePointer1 == NULL)
        {
          exit(1);
        }
           
        for(i=0;i<lenght_of_contents1;i++)
        {
            fprintf(filePointer1, "%d\n",*(pointer_to_contents1+i));
        }

        fclose(filePointer1);
    }

       
}
