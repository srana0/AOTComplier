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
#include "Helper_Global.h"


  void Initialize_Method_In_MethodTable_AllMethods()
  {

      uint32_t i=0; 
      // Changed to int to save negative value
      int * methodTable_Pointer_AllMethods=(int * )__MethodTable_Pointer_AllMethods_STATIC;   //ALWAYS START FROM THE STARTRING OF THE MEMORY LOCATION    
   
  
    for(i=0;i<CAPACITY_ALL_METHODS;i++)
    {  
          *(methodTable_Pointer_AllMethods+i)=-1;         // Assign -1                                            
             
    }
 
  }

  

//____________________________________________________________________________________
//Add entry to hold the value of write operation of interpreter
//_________________________________________________________________________________
void Add_Entry_To_AllMethods(int method_Identifier)
{

    *__MethodTable_Pointer_AllMethods_DYNAMIC=method_Identifier;                               
    __MethodTable_Pointer_AllMethods_DYNAMIC=__MethodTable_Pointer_AllMethods_DYNAMIC+1;     
     
    __TotalNumber_Of_Items_In_AllMethods++; 

}



// write to method table compiler
void WriteToTextFile_Output_AllMethods(uint32_t * pointer_to_contents,uint32_t lenght_of_contents)
{
    if(lenght_of_contents>0)
    {
        FILE *filePointer;
       // int fileLength1; 

        uint32_t i; 
        // Open the Text File 
        filePointer=fopen("Output_MethodTable_AllMethods.txt","w"); 
        // Validate if File can be opened or not

        if(filePointer == NULL)
        {
          exit(1);
        }
           
        for(i=0;i<lenght_of_contents;i++)
        {
            fprintf(filePointer, "%d\n",*(pointer_to_contents+i));
        }

        fclose(filePointer);
    }       
}




//____________________________________________________________________________________
//Search Objects In All Methods
//_________________________________________________________________________________
bool Is_Method_Exists_In_Table(uint32_t method_Identifier)
{
  bool isMethodExists=false;
 
   uint32_t i=0;
 
 uint32_t method_Identifier_In_MethodTable=0;
 int method_Identifier_In_MethodTable_INT=0;   // int to validate negative number

 uint32_t * methodTable_Pointer_AllMethods=(uint32_t *)__MethodTable_Pointer_AllMethods_STATIC;   //ALWAYS START FROM THE STARTRING OF THE MEMORY LOCATION 
  
    for(i=0;i<__TotalNumber_Of_Items_In_AllMethods;i++)
    {    
     
      method_Identifier_In_MethodTable_INT=(int)(*(methodTable_Pointer_AllMethods+i));           // Check negative value     
          
      if(method_Identifier_In_MethodTable_INT!=-1)                           
      {        
         method_Identifier_In_MethodTable=*(methodTable_Pointer_AllMethods+i);

          if(method_Identifier==method_Identifier_In_MethodTable)
          {
                 isMethodExists=true; 
                 break;
          }        
      }
      else
      {
        
      }
      
    }
    return isMethodExists;
}
