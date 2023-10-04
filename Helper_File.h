#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<inttypes.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h> 
#include <stdbool.h>
#include<arpa/inet.h>
#include <dlfcn.h>
#include <libintl.h>
#include <time.h>
#include <assert.h>

//____________________________________________________________________________________________________________________________
// This function will load the File Containing Byte Code from the argument
// Files must the in the same location where our C output file exists
//____________________________________________________________________________________________________________________________
int LoadBinaryFileIntoVM(char *argv[],int argmentPostion,int IndexForCopy,int * JVM)
{

FILE *filePointer;
//unsigned char *instuctionStart;
//unsigned char *instructionEnd;
unsigned char *program_Pointer_VM;
long fileLength; 
unsigned char *pointerBinaryFile; // This is a pointer to the binay file that will be loaded 
int i; 
// Open the Binary File 
filePointer=fopen(argv[argmentPostion],"rb"); 
// Validate if File can be opened or not
if (!filePointer)
{
perror("fopen");
exit(EXIT_FAILURE);
}
fseek(filePointer,0,SEEK_END);
fileLength=ftell(filePointer); 
rewind(filePointer);
pointerBinaryFile=(unsigned char *)malloc((fileLength+1)*sizeof(char)); // will return the address to the space 
//printf("\nReading  File from File Location Specified by args...."); 
for(i=0;i<fileLength;i++)
{ 
fread(pointerBinaryFile+i,1,1,filePointer);
} 
fclose(filePointer); 
// Assign this value to the pointer to start it with 1024 location
program_Pointer_VM= (unsigned char *)JVM+IndexForCopy;


//printf("\n File LENGTH: %d SOURCE ADDRESS : %u TARGET ADDRESS : %u\n",fileLength, pointerBinaryFile,program_Pointer_VM);

//instuctionStart=program_Pointer_VM;
//printf("\n ......Copying File to VM at memery location %d\n",IndexForCopy);
memcpy(program_Pointer_VM,pointerBinaryFile,fileLength); // Copied the files to the 1024 location of our VM

int iCount1=0;

// Printing the File 
for(iCount1=0;iCount1<fileLength;iCount1++)
{ 
//printf("%hhu\t",*program_Pointer_VM); 
program_Pointer_VM++;
}
printf("\n");

//instructionEnd=--program_Pointer_VM; // Adjusting the Last Pointer 

free(pointerBinaryFile);
return fileLength;
}



//____________________________________________________________________________________________________________________________
// This function will get the Length of the Argv
//____________________________________________________________________________________________________________________________
int  argv_length(char** argv)
{
    int count = 0;
    while(argv[++count]);
    return count;
}


char * Get_Function_Name_CompiledCode(uint32_t function_Identifier)
{
char function_number[1000]; 

char func[]="func_";
//itoa(function_Identifier,function_number,10);
sprintf(function_number,"%d",function_Identifier);
int lengh_of_String=strlen(function_number);

strncat(func,function_number,lengh_of_String);
//printf("\n new %s",func);
char* fp=func;
///printf("\n fp=%s",fp);

return fp;

}

//__________________________________________________________________________________________
// This will give the name of the library in ./name/so format
//__________________________________________________________________________

 char * Get_Library_Name_CompiledCode(char * sharedObject_FileName)
{


char soPrefix[]="./";
char soPostFix[]=".so";


strcat(soPrefix,sharedObject_FileName);
strcat(soPrefix,soPostFix);


printf("\n libraryName=%s",soPrefix);


 char * librayPointer=soPrefix;
//const char * librayPointer=sharedObject_FileName;
return librayPointer;
//return soPrefix;

}
