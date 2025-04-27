# SIL Compiler
This repository contains a compiler written in C for SIL language using lex and yacc.
It converts the SIL language into MIPS assembly language.

This compiler supports the following constructs:
- Assignment statements
- One Dimensional Arrays
- Read and Write Function calls
- If-else statement
- For loop
- Do while loop
- break and continue statements

## Structure of the Compiler
```
|-calculator
|   |-calulator.l
|   |-calculator.y
|   |-Makefile
|
|-compiler
|   |-bin
|   |
|   |-include
|   |   |-CodeGeneration.h
|   |   |-Queue.h
|   |   |-Stack.h
|   |   |-Symboltable.h
|   |   |-SyntaxTree.h
|   |   
|   |-src
|   |   |-CodeGeneration.c
|   |   |-compiler.l
|   |   |-compiler.y
|   |   |-Queue.c
|   |   |-Stack.c
|   |   |-Symboltable.c
|   |   |-SyntaxTree.c
|   |   
|   |-Makefile
|   
|-test-2
|-CHANGELOG.md
|-README.md
```

## Install the required tools:
Run the following commands:
```
$ sudo apt install flex yacc
$ sudo apt install binutils-mips-linux-gnu
$ sudo apt install gcc-10-mips-linux-gnu
$ sudo apt install qemu-user-static
$ sudo apt  install qemu-user
```


## To build the compiler , follow these steps : 

1. Navigate to the compiler folder
```
$ cd compiler
```
2. Build the compiler
```
$ make
```
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;This will generate the executable file as compiler inside the bin folder

3. Run the compiler
```
$ <path of the compiler executable>  <filename with path>.sil
```

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Ex. if you are currently at the compiler folder of this project and let the sample file be sample4.sil , then type
```
$ ./bin/compiler sample4.sil
```
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Note: It will generate the .s file in the folder in which you invoked the compiler executable and also it will print the AST and the Symbol Table in the terminal.

4. Make the program executable 
```
$ mips-linux-gnu-gcc-10  sample4.s -o sample4.out
```
5. Run the executable
```
$  qemu-mips -L /usr/mips-linux-gnu/ sample4.out
```

## Sample Program

```
begindecl
    integer array[5];
    integer key, found;
    integer index;
enddecl

begin
    read(array[0], array[1], array[2]);
    read(array[3], array[4]);
    read(key);

    found = 0;
    for (index = 0; index < 4; index++) {
        if (array[index] == key) then
        found = 1;
        endif
    }

    if (found == 1) then
        write(1);
    else 
        write(0);
    endif
end
```

## Abstract Syntax Tree and Symbol Table

```
Abstract Syntax Tree (AST):
===========================

--> PROG
	|--> GDECL_LIST
	|	|--> GDECL
	|	|	|--> integer
	|	|	|--> array[5]
	|	|--> GDECL
	|	|	|--> integer
	|	|	|--> key
	|	|	|--> found
	|	|--> GDECL
	|	|	|--> integer
	|	|	|--> index
	|--> STMT_LIST
	|	|--> CALL
	|	|	|--> READ
	|	|	|--> array[0]
	|	|	|--> array[1]
	|	|	|--> array[2]
	|	|--> CALL
	|	|	|--> READ
	|	|	|--> array[3]
	|	|	|--> array[4]
	|	|--> CALL
	|	|	|--> READ
	|	|	|--> key
	|	|--> ASSIGN_STMT
	|	|	|--> found
	|	|	|--> 0
	|	|--> COND_STMT
	|	|	|--> FOR_STMT
	|	|	|	|--> for_init
	|	|	|	|	|--> ASSIGN_STMT
	|	|	|	|	|	|--> index
	|	|	|	|	|	|--> 0
	|	|	|	|--> for_condition
	|	|	|	|	|--> LESSTHAN
	|	|	|	|	|	|--> index
	|	|	|	|	|	|--> 4
	|	|	|	|--> for_expr
	|	|	|	|	|--> ASSIGN_STMT
	|	|	|	|	|	|--> index
	|	|	|	|	|	|--> PLUSPLUS
	|	|	|	|--> for_body
	|	|	|	|	|--> COND_STMT
	|	|	|	|	|	|--> IF_STMT
	|	|	|	|	|	|	|--> if_condition
	|	|	|	|	|	|	|	|--> EQUALEQUAL
	|	|	|	|	|	|	|	|	|--> array
	|	|	|	|	|	|	|	|	|	|--> index
	|	|	|	|	|	|	|	|	|--> key
	|	|	|	|	|	|	|--> if_body
	|	|	|	|	|	|	|	|--> ASSIGN_STMT
	|	|	|	|	|	|	|	|	|--> found
	|	|	|	|	|	|	|	|	|--> 1
	|	|--> COND_STMT
	|	|	|--> IF_STMT
	|	|	|	|--> if_condition
	|	|	|	|	|--> EQUALEQUAL
	|	|	|	|	|	|--> found
	|	|	|	|	|	|--> 1
	|	|	|	|--> if_body
	|	|	|	|	|--> CALL
	|	|	|	|	|	|--> WRITE
	|	|	|	|	|	|--> 1
	|	|	|--> ELSE_STMT
	|	|	|	|--> else_body
	|	|	|	|	|--> CALL
	|	|	|	|	|	|--> WRITE
	|	|	|	|	|	|--> 0



Symbol Table: 
=============
+---------------+---------------+------------------+-------------+------------------------------------+
|   Variables   |     Type      |     VarType      | Initialized |        Values                      |
+---------------+---------------+------------------+-------------+------------------------------------+
|   index       |   int         |       int        |      0      |   0                                |
|   key         |   int         |       int        |      0      |   0                                |
|   array       |   array       |       int        |      0      |   0 , 0 , 0 , 0 , 0                |
|   found       |   int         |       int        |      0      |   0                                |
+---------------+---------------+------------------+-------------+------------------------------------+

=======================================================================================================
```

## Sample Input

```
$ 2 4 6 8 10
$ 9
```


## Sample Output

```
$ 0
```


## Submitted by:
```
Name : Chirag Varshney 
Roll No. : 112201023
```
