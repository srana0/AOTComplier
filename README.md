# AOTComplier
<a name="br1"></a> 


Your code is expected to be compiled and executed on a linux machine with a gcc compiler.

The standard configuration in our labs. Please ensure this is the case before you submit as

the TA needs to be able to run your code and cannot do so if it is not in this environment.

For this assignment we are going back to our interpreter and will focus on ahead-of-time

compilation. You will remember that const\_pool is a constant pool that contains a simple

table of methods that exist in the program. The construction of the table is as follows:

Reference to method Number of args (8-

(32-bit hexadecimal bit hexadecimal

Bit encoding of arg

types (8-bit

hexadecimal

number)

Return value

encoding (8-bit

hexadecimal

number)

number)

number)

There is one row for each method that exists in the program. The reference is the address

of the method once loaded. The number of args is a maximum of 8. The bitwise encoding

of the hexadecimal number for argument types is 0 for integer and 1 for a reference (the

two data types we support). The return value is an unsigned byte 0 for an int, 1 for a

reference, and 2 for no return value. The constant pool is loaded at address 256. You can

assume that the constant pool will not collide with the executable at address 1024.

In this assignment, you will add support for replacing a method invocation that jumps to

the bytecode for method interpretation with a reference for a compiled method to perform

the equivalent operation. There are a few enhancements you need to make to support this:

1\. For each of the methods that are in the const\_pool you need to keep a counter.

This counter will start at 0 and increment every time the method is invoked. Once

it reaches a threshold then you are to check if a compiled version of the method

exists. If so, then you are to make note of this in the const\_pool and from then on

invoke the compiled method instead of the interpreted one.

2\. An additional flag with argument -a numwill be provided on the command line

where numis the threshold.

1



<a name="br2"></a> 

3\. The compiled methods will be provided in a previously compiled library that are

specified on the command line as -l librarywhere libraryis the filename

of the library in the current directory.

4\. You can open, identify and close the library file using the dlopen(), dlsym()

and dlclose()functions. You may also find dlerror()helpful to determine

if the method is found after calling dlsym().

5\. In order to use dlsym()you need the symbolic name of the method that you are

searching for. The symbolic name for a method that is at index x in the

const\_poolis func\_x. If dlsym()fails to return a handle to the function

then you can assume that an AoT compiled version of the method does not exist.

If a compiled version exists, then you must always invoke the compiled version

from that point forward.

6\. At the end of executing the application you are interpreting, you are to print

statistics on: total execution time, and for each method that was invoked - the

number of interpreted and compiled invocations separately.

Note: When you invoke the compiled method be aware that you need to take care of the

parameters, local variables and return values on the stack. Also, you are not required to

remove changes you made for assignment #2 to the const\_pool.

While not expected, students are free to submit their own developed tests with their

submissions. Additional test cases are welcomed from the students to use in testing class

submissions.

NOTE: THE INPUT AND OUTPUT OF YOUR PROGRAM IS SPECIFIED SUCH

THAT THE MARKER CAN AUTO TEST YOUR SUBMISSIONS. PLEASE

FOLLOW THE SPECIFICATIONS! INPUT WILL BE READ VIA stdin (EG.

KEYBOARD). OUTPUT SHOULD BE PRINTED TO stdout (EG. MONITOR).

YOU CAN FIND THE SAMPLE INPUT FILES WITH THE ASSIGNMENT ON

D2L. YOU CAN CHECK WITH EACH OTHER ON RESULTS FROM VARIOUS

RUNS TO SEE IF YOU ARE CORRECT. BE SURE YOUR PROGRAM

READS/PRINTS THE PROPER INPUT/OUTPUT FORMATTING!

2


