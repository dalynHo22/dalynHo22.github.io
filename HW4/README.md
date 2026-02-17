/*
Dalyn Ho 
April 14, 2025
COP 3402: Systems Software 
Homework 4: PL/0 Compiler and VM
*/

# Homework 4: PL/0 Compiler and VM

## Description
This assignment, part of COP 3402: Systems Software (Homework 4: PL/0 Compiler and Virtual Machine), extends the HW3 compiler to support additional PL/0 grammar features including procedure declarations, call statements, and if-else-fi control structures. It also enhances the HW1 virtual machine to handle the modulo operation (OPR 0 11). The compiler reads a PL/0 source file, performs recursive descent parsing with error detection, manages a symbol table across lexical levels using a marking algorithm, and generates intermediate assembly code. If no errors are found, the compiler outputs a readable assembly listing, displays the symbol table, and produces a numeric instruction file (elf.txt) for execution by the VM. Any lexical, syntactic, or semantic error halts compilation immediately with an appropriate message.

## Compilation Instructions
To compile the program, use the following command:
gcc -std=c17 -Wall -o hw4compiler hw4compiler.c


## Usage
To run the program, use the following command:
./hw4compiler  <input_file>

## Example
Input File (pl0.txt):
var x, y;
begin
x := y * 2;
end.

Sample Command:
./hw4compiler  pl0.txt

Expected Output in Terminal:
No errors, program is syntactically correct.

Assembly Code:
Line OP  L M
0    JMP 0 13
1    INC 0 5
2    LOD 0 4
3    LIT 0 2
4    OPR 0 4
5    STO 0 3
6    SIO 0 3

Symbol Table:
Kind | Name | Value | Level | Address | Mark
-----------------------------------------------
2    | x    | 0     | 0     | 3       | 1
2    | y    | 0     | 0     | 4       | 1

Expected Output in elf.txt file:
7 0 13
6 0 5
3 0 4
1 0 2
2 0 4
4 0 3
9 0 3

## Team Information (if applicable)
- N/A

## Contact Information
Dalyn Ho, dalynho22@gmail.com, (615) 922 - 9077