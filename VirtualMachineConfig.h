

// DELETE ME


// Method Defination for Push and Pop
int Pop();
void Push(int item);
  
int * Convert_To_PhysicalAddress(uint32_t logical_Address);
uint32_t Convert_To_LogicalAddres(void* physical_Address);

// CONVERSTION HELPER
int* ConvertAddress_From_LocalMachine(uint32_t input);
uint32_t ConvertAddress_To_LocalMachine(void* address);

void SaveValueInStackFrame(uint32_t stackFrame_ReturnValue, uint32_t programCounter_Address,uint32_t operandStack_Address,uint32_t stackFrame_Address,int argument_from_main);

// PRINT HELPERS
void Highlight_ConstantPoolReference(unsigned char * referenceToNewMethod);
void Highlight_ProgramByteCode(int indexToHighlight);

// void WriteToTextFile_Method_Interpreter(uint32_t * pointer_to_contents0, uint32_t lenght_of_contents0);
// void WriteToTextFile_MethodTable_Compiler(uint32_t * pointer_to_contents1,uint32_t lenght_of_contents1);

void PrintMethodTable_Interpreter(uint32_t number_of_items);

void PrintVirtualMachine(int * JVM,unsigned char * program_Pointer_VM,int execFileLength,unsigned char * constantPool_Pointer_VM,int constantPoolFileLength,int * stackFramePointer_Static);
void PrintOpcode(uint32_t opcode,char *opcode_Name);
void PrintExecutionOutput(double totalExecutionTime);

void PrintStackFrame();
void Printheader(char *heading);

// FILE HELPERS
int LoadBinaryFileIntoVM(char *argv[],int argmentPostion,int IndexForCopy,int * JVM);
char * Get_Function_Name_CompiledCode(uint32_t function_Identifier);
char * Get_Library_Name_CompiledCode(char * sharedObject_FileName);

// OPCODE HELPER
void BIPUSH();
void ISTORE();
void ICONST_0();
void ILOAD();
void IFGT();
void GOTO();
void ISUB();
void IADD();
void ICMPEQ();
void IINC();
void IWRITE();
void IALOAD();
void IAND();
void IASTORE();
void IDIV();
void IFEQ();
void IFLT();
void IFGE();
void IFLE();
void IFNULL();
void IMULL();
void INEG(); 
void IOR();
void POP_OperandStack();
void IRETURN();
void INVOKE_STATIC();
void IREAD();
void RETURN();
void NEW();
void ANEWARRAY(); 
void NEWARRAY();
void GETFIELD();
void PUTFIELD();
void ALOAD();
void ASTORE();
void DUP();
void ACONST_NULL();
void AASTORE();
void AALOAD();


void Add_Or_Update_Interpreted_Method_Table(uint16_t constantPool_Reference,uint32_t methodCounter_Interpreter);
void Add_Or_Update_Precompiled_Method_Table(uint16_t constantPool_Reference);




void Add_Entry_To_AllMethods(int method_Identifier);
void WriteToTextFile_Output_AllMethods(uint32_t * pointer_to_contents,uint32_t lenght_of_contents);
bool Is_Method_Exists_In_Table(uint32_t method_Identifier);
void Initialize_Method_In_MethodTable_AllMethods();

void Initialize_Method_In_MethodTable_Interpreter();
void Add_Method_To_MethodTable_Interpreter(uint32_t method_Identifier, uint32_t method_Counter);
uint32_t Search_Number_Of_Methods_In_MethodTable_Interpreter(uint32_t method_Identifier);
void Update_Method_In_MethodTable_Interpreter(uint32_t method_Identifier,uint32_t method_Counter);
void Delete_Method_From_MethodTable_Interpreter(uint32_t method_Identifier);
void PrintMethodTable_Interpreter(uint32_t number_of_items);
void WriteToTextFile_Method_Interpreter(uint32_t* pointer_to_contents,uint32_t lenght_of_contents);
void Add_Entry_To_Method_Table_Interpreter(uint32_t method_Identifier,uint32_t method_Counter);

void Initialize_Method_In_MethodTable_Compiled();
void Add_Method_To_MethodTable_Compiled(uint32_t method_Identifier, uint32_t method_Counter);
uint32_t Search_Number_Of_Methods_In_MethodTable_Compiled(uint32_t method_Identifier);
void Update_Method_From_MethodTable_Compiled(uint32_t method_Identifier,uint32_t method_Counter);
void Delete_Method_From_MethodTable_Compiled(uint32_t method_Identifier);
void PrintMethodTable_Compiled(int number_of_Objects);
void WriteToTextFile_MethodTable_Compiler(uint32_t * pointer_to_contents1,uint32_t lenght_of_contents1);
void Add_Entry_To_Method_Table_Compiler(uint32_t method_Identifier,uint32_t method_Counter);


