/*
Dalyn Ho 
April 14, 2025
COP 3402: Systems Software 
Homework 4: PL/0 Compiler and VM
*/ 

//need to update to recognize and execute the modulo (mod) instruction (OPR 0 11).
//currently no changes from hw1 vm.c

#include <stdio.h>
#include <stdlib.h>

FILE *fileOp;
int PAS[500] = {0};
int ActRec[500] = {0};
const int ARRAY_SIZE = 500;
int ARC = 0;

//Instruction register, IR 
typedef struct IR {
    int OP;
    int L;
    int M;
} IR;

//Function headers
void printer(IR IR, int PC, int BP, int SP);
int base(int BP, int L);

int main(int argc, char **argv) {

    //Initialize base values for registers
    int BP = 499;
    int SP = BP + 1;
    int PC = 10;
    //End-of-program flag
    int EOP = 1;

    //Open the input file containing PM/0 instructions
    fileOp = fopen(argv[1], "r");
    //Check if the file was opened successfully 
    if (fileOp == NULL) {
        printf("Error: Could not open file %s\n", argv[1]);
        return 1;
    }

    //Read instructions from the file and load into PAS
    int haltIndex;
    while (fscanf(fileOp, "%d", &PAS[PC]) == 1) {
        PC++;
    }
    haltIndex = PC - 1; // track the final instruction address
    PC = 10;


    //Print initial values of registers
    printf("\t\tPC\tBP\tSP\tStack\n");
    printf("Initial values: %d\t%d\t%d\n\n", PC, BP, SP);

    IR IR; //IR to hold the current instruction
    while (EOP) {


        //Fetch cycle: Load the next instruction into the IR
        IR.OP = PAS[PC];
        IR.L = PAS[PC + 1];
        IR.M = PAS[PC + 2];
        if (PC + 2 > haltIndex) {
            printf("Error: Attempted to fetch beyond instruction bounds.\n");
            break;
        }
        PC = PC + 3;

        //Execute cycle: Perform operation specified by the instruction
        switch (IR.OP) {
            case 1: //LIT (Load constant)
                SP = SP - 1;
                PAS[SP] = IR.M; //Push constant value onto the stack
                printf("LIT");
                printer(IR, PC, BP, SP);
                break;

            case 2: //OPR (Operation)
                switch (IR.M) {
                    case 0: //RTN (Return from subroutine)
                        SP = BP + 1;
                        BP = PAS[SP - 2];
                        PC = PAS[SP - 3];
                        ARC--;
                        ActRec[ARC] = 0; //Clear activation record
                        printf("RTN");
                        printer(IR, PC, BP, SP);
                        break;
                    case 1: //ADD (Addition)
                        PAS[SP + 1] = PAS[SP + 1] + PAS[SP]; //Add the top two stack elements
                        SP = SP + 1;
                        printf("ADD");
                        printer(IR, PC, BP, SP);
                        break;
                    case 2: //SUB (Subtraction)
                        PAS[SP + 1] = PAS[SP + 1] - PAS[SP];
                        SP = SP + 1;
                        printf("SUB");
                        printer(IR, PC, BP, SP);
                        break;
                    case 3: //MUL (Multiplication)
                        PAS[SP + 1] = PAS[SP + 1] * PAS[SP];
                        SP = SP + 1;
                        printf("MUL");
                        printer(IR, PC, BP, SP);
                        break;
                    case 4: //DIV (Division)
                        PAS[SP + 1] = PAS[SP + 1] / PAS[SP];
                        SP = SP + 1;
                        printf("DIV");
                        printer(IR, PC, BP, SP);
                        break;
                    case 5: //EQL (Equal)
                        PAS[SP + 1] = PAS[SP + 1] == PAS[SP];
                        SP = SP + 1;
                        printf("EQL");
                        printer(IR, PC, BP, SP);
                        break;
                    case 6: //NEQ (Not Equal)
                        PAS[SP + 1] = PAS[SP + 1] != PAS[SP];
                        SP = SP + 1;
                        printf("NEQ");
                        printer(IR, PC, BP, SP);
                        break;
                    case 7: //LSS (Less Than)
                        PAS[SP + 1] = PAS[SP + 1] < PAS[SP];
                        SP = SP + 1;
                        printf("LSS");
                        printer(IR, PC, BP, SP);
                        break;
                    case 8: //LEQ (Less Than or Equal)
                        PAS[SP + 1] = PAS[SP + 1] <= PAS[SP];
                        SP = SP + 1;
                        printf("LEQ");
                        printer(IR, PC, BP, SP);
                        break;
                    case 9: //GTR (Greater Than)
                        PAS[SP + 1] = PAS[SP + 1] > PAS[SP];
                        SP = SP + 1;
                        printf("GTR");
                        printer(IR, PC, BP, SP);
                        break;
                    case 10: //GEQ (Greater Than or Equal)
                        PAS[SP + 1] = PAS[SP + 1] >= PAS[SP];
                        SP = SP + 1;
                        printf("GEQ");
                        printer(IR, PC, BP, SP);
                        break;
                    //Modified Code 
                    case 11: // MOD (Modulo)
                        PAS[SP + 1] = PAS[SP + 1] % PAS[SP];
                        SP = SP + 1;
                        printf("MOD");
                        printer(IR, PC, BP, SP);
                        break;
                }
                break;

            case 3: //LOD (Load)
                SP = SP - 1;
                PAS[SP] = PAS[base(BP, IR.L) - IR.M]; //Load value from memory onto the stack
                printf("LOD");
                printer(IR, PC, BP, SP);
                break;

            case 4: //STO (Store)
                PAS[base(BP, IR.L) - IR.M] = PAS[SP]; //Store value from the stack into memory
                SP = SP + 1;
                printf("STO");
                printer(IR, PC, BP, SP);
                break;

            case 5: //CAL (Call)
                //Prepare the new activation record 
                PAS[SP - 1] = base(BP, IR.L); //SL
                PAS[SP - 2] = BP; //DL
                PAS[SP - 3] = PC; //RA
                BP = SP - 1;
                PC = IR.M; //Set PC to the procedure address 
                printf("CAL");
                printer(IR, PC, BP, SP);
                ActRec[ARC] = SP; //Track the new AR
                ARC++;
                break;

            case 6: //INC (Increment)
                //Increment stack pointer by 3
                SP = SP - IR.M; //Allocate space for local variables
                printf("INC");
                printer(IR, PC, BP, SP);
                break;

            case 7: //JMP (Jump)
                PC = IR.M; //Set PC to the jump address
                printf("JMP");
                printer(IR, PC, BP, SP);
                break;

            case 8: //JPC (Jump Conditional)
                //Jump to the specified address, instruction M, if top of stack is 0
                if (PAS[SP] == 0) {
                    PC = IR.M;
                }
                SP = SP + 1;
                printf("JPC");
                printer(IR, PC, BP, SP);
                break;

            case 9: //SYS (System Operation)
                switch (IR.M) {
                    case 1: //Output result with top stack element 
                        printf("Output result is: %d\n", PAS[SP]);
                        SP = SP + 1;
                        printf("SYS");
                        printer(IR, PC, BP, SP);
                        break;
                    case 2: //Read input and store it to top of the stack
                        SP = SP - 1;
                        printf("Please enter an integer: ");
                        fscanf(stdin, "%d", &PAS[SP]);
                        printf("SYS");
                        printer(IR, PC, BP, SP);
                        break;
                    case 3: //End of program
                        EOP = 0; //Set the EOP flag to 0
                        printf("SYS");
                        printer(IR, PC, BP, SP);
                        break;
                }
                break;
        }
    }

    return 0;
}

//Prints output
void printer(IR IR, int PC, int BP, int SP) {
  printf(" %d\t%d\t%d\t%d\t%d\t", IR.L, IR.M, PC, BP, SP);
  for (int i = 499; i >= SP; i--) {
    printf("%d ", PAS[i]);

    //Iterates through activation records
    for (int j = ARC; j >= 0; j--) {
      //Checking for match 
      if (i == ActRec[j]) {
        printf("|"); //Separate AR with a bar
        break;
      }
    }
  }
  printf("\n");
}


//Computes base address for level L relative to BP
int base(int BP, int L) {
    int arb = BP; //Activation record base starting from current BP

    //Traverse L levels down the static chain to find the base address
    while (L > 0) {
        arb = PAS[arb]; //Follow static link to the next AR
        L--; 
    }

    return arb; 
}
