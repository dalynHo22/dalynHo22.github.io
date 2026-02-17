/*
Dalyn Ho 
April 14, 2025
COP 3402: Systems Software 
Homework 4: PL/0 Compiler and VM
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAXNUM 5
#define MAXID 11
#define MAXTOK 999
#define MAX_SYMBOL_TABLE_SIZE 500

//Declaration for Token Types
typedef enum 
{ 
    modsym = 1, identsym = 2, numbersym = 3, plussym = 4, minussym = 5,
    multsym = 6, slashsym = 7, fisym = 8, eqlsym = 9, neqsym = 10, 
    lessym = 11, leqsym = 12, gtrsym = 13, geqsym = 14, lparentsym = 15, 
    rparentsym = 16, commasym = 17, semicolonsym = 18, periodsym = 19, becomessym = 20,
    beginsym = 21, endsym = 22, ifsym = 23, thensym = 24, whilesym = 25, 
    dosym = 26, callsym = 27, constsym = 28, varsym = 29, procsym = 30, 
    writesym = 31, readsym = 32, elsesym = 33
} token_type;

// Structure for Token Representation
typedef struct
{
    token_type type;
    char id[100]; // Stores the lexeme (variable names, numbers, etc.)
    int idLen;
    int value;
} Token;

// Symbol Table Entry Structure
typedef struct
{
    int kind; // const = 1, var = 2, proc = 3
    char name[10]; // name up to 11 chars
    int val; // number (ASCII value)
    int level; // L level
    int addr; // M address
    int mark; // to indicate unavailable or deleted
} symbol;

// Instruction Structure for Code Generation
typedef struct
{
    char* OP_code;
    int OP;
    int L;
    int M;
} Instruction;

// Global Declarations
Token lex[MAXTOK];
Token token;
symbol table[MAX_SYMBOL_TABLE_SIZE];
Instruction code[MAXTOK];

// Function Prototypes
int checkEq(char *id, char *word);
void printSource(FILE *fp);
void undec(Token token);
void error(char *text);
Token getTok(FILE *fp);
void getNextToken();
void addSymbol(int kind, char* name, int val, int level, int addr);
int checkTable(Token token, int kind);
void emit(char* OP_code, int OP, int L, int M); 
void program();
void block();
void constDec();
int varDec();
void statement();
void condition();
void expression();
void term();
void factor();
void procDec();

// File handling and state variables
FILE *fp;
int eof = 0; 
int flag = 0;
int tokenCT = 0;
int tokIdx = 0;
int tx = 0;
int cx = 0;
int level = -1;

// Function to check if two strings are equal
int checkEq(char *id, char *word)
{
    return strcmp(id, word) == 0;
}

// print source code
void printSource(FILE *fp)
{
    char c;

    // print source code
    while ((c = fgetc(fp)) != EOF)
    {
        printf("%c", c);
    }
    printf("\n\n");

    // reset file pointer
    rewind(fp);
}

// Prints error for undeclared identifier and exits
void undec(Token token)
{
    // Prints a message if undec. identifier
    printf("Error: undeclared identifier %s.", token.id);
    exit(1);
}

// Prints a general error message and exits
void error(char *text)
{
    printf("Error: %s\n", text);
    exit(1);
}

// Gets the next token from input
Token getTok(FILE *fp)
{
    // Make sure not including the whitespace
    Token token = {0};
    int c;
    do{
        c = fgetc(fp);
        if(c == EOF)
        {
            eof = 1;
            return token;
        }
        
    } while (isspace(c));

    // Checks if reserve word or symbol
    if (isalpha(c))
    {
        int i = 0;
        do {
            token.id[i] = c;
            c = fgetc(fp);
            i++;
        } while ((isalpha(c) || isdigit(c)));

        // check len
        if(i > MAXID)
            {
                error("Identifier too long.");
            }
        else
            {
                token.idLen = i;
            }
        
        // Checks if its in list of reserves
        token.idLen = i;
        if (checkEq(token.id, "const"))
        {
            token.type = constsym;
        }
        else if (checkEq(token.id, "mod"))
        {
            token.type = modsym;
        }
        else if (checkEq(token.id, "var"))
        {
            token.type = varsym;
        }
        else if (checkEq(token.id, "procedure"))
        {
            token.type = procsym;
        }
        else if (checkEq(token.id, "call"))
        {
            token.type = callsym;
        }
        else if (checkEq(token.id, "begin"))
        {
            token.type = beginsym;
        }
        else if (checkEq(token.id, "end"))
        {
            token.type = endsym;
        }
        else if (checkEq(token.id, "if"))
        {
            token.type = ifsym;
        }
        else if (checkEq(token.id, "fi"))
        {
            token.type = fisym;
        }
        else if (checkEq(token.id, "then"))
        {
            token.type = thensym;
        }
        else if (checkEq(token.id, "while"))
        {
            token.type = whilesym;
        }
        else if (checkEq(token.id, "do"))
        {
            token.type = dosym;
        }
        else if (checkEq(token.id, "read"))
        {
            token.type = readsym;
        }
        else if (checkEq(token.id, "write"))
        {
            token.type = writesym;
        }
        else if (checkEq(token.id, "else"))
        {
            token.type = elsesym;
        }
        else
        {
            token.type = identsym;
        }

        // Pushes last char back onto input
        ungetc(c, fp);
    }


    // Checks if digit
    else if (isdigit(c))
    {
        int i = 0;
        do {
            token.id[i] = c;
            c = fgetc(fp);
            i++;
        } while (isdigit(c));

        token.type = numbersym;
        token.value = atoi(token.id);

        // check number length
        if(i > MAXNUM)
        {
            error("Number too long.");
        }
        else
        {
            token.idLen = i;
        }

        ungetc(c, fp);
    }

    // Reserve symbol
    else
    {
        token.id[0] = c;
        switch (c) {
            case '+':
                token.type = plussym;
                break;
            case '-':
                token.type = minussym;
                break;
            case '*':
                if(flag)
                {
                    c = fgetc(fp);
                    if(c == '/')
                    {
                        flag = 0;
                        token = getTok(fp);
                    }
                }
                else
                {
                    token.type = multsym;
                }
                break;
            case '/':
                c = fgetc(fp);
                if(c == '*')
                    flag = 1;
                else
                {
                    token.type = slashsym;
                    ungetc(c, fp);
                }
                break;
            case '=':
                token.type = eqlsym;
                break;
            case '<':
                c = fgetc(fp);
                if (c == '>') 
                {
                    token.type = neqsym;
                    token.id[1] = c;
                } 
                else if (c == '=') 
                {
                    token.type = leqsym;
                    token.id[1] = c;
                } 
                else 
                {
                    token.type = lessym;
                    ungetc(c, fp);
                }
                break;
            case '>':
                c = fgetc(fp);
                if (c == '=') 
                {
                    token.type = geqsym;
                    token.id[1] = c;
                } 
                else 
                {
                    token.type = gtrsym;
                    ungetc(c, fp);
                }
                break;
            case '(':
                token.type = lparentsym;
                break;
            case ')':
                token.type = rparentsym;
                break;
            case ',':
                token.type = commasym;
                break;
            case ';':
                token.type = semicolonsym;
                break;
            case '.':
                token.type = periodsym;
                break;
            case ':':
                c = fgetc(fp);
                if (c == '=') 
                {
                    token.type = becomessym;
                    token.id[1] = c;
                } 
                else 
                {
                    error("Invalid Symbol.");
                }
                break;
            default:
                error("Invalid Symbol.");
        }
    }

    return token;
}

// Advances to the next token from the token array
void getNextToken()
{
    token = lex[tokIdx];
    tokIdx++;
}

// Adds a new symbol to the symbol table
void addSymbol(int kind, char* name, int val, int level, int addr)
{
    symbol newSym = {0};
    newSym.kind = kind;
    strcpy(newSym.name, name);
    newSym.val = val;
    newSym.level = level;
    newSym.addr = addr;
    newSym.mark = 0;
    table[tx] = newSym;
    tx++;
}

// Searches for an identifier in the symbol table, returns index if found, -1 otherwise
int checkTable(Token token, int kind)
{
    for(int i = tx-1; i >= 0; i--)
    {
        if(checkEq(token.id, table[i].name) && table[i].kind == kind)
        {
            return i;
        }
    }
    return -1;
}

// Emits an instruction to the code array
void emit(char* OP_code, int OP, int L, int M)
{
    Instruction inst = {0};
    inst.OP_code = strdup(OP_code);  // Make a safe copy
    inst.OP = OP;
    inst.L = L;
    inst.M = M;
    code[cx++] = inst;
}


// Main program function that initiates parsing
void program()
{
    getNextToken();
    block();
    // Ensure program ends with a period
    if(token.type != periodsym)
    {
        error("program must end with period");
    }
    emit("SYS", 9, 0, 3);

    // Mark symbols at the current level
    for(int i = tx - 1; i >= 0; i--)
    {
        if (table[i].level == level)
            table[i].mark = 1;
    }
}

// Handles declarations
void block()
{
    level++;

    int dx = 3;

    // Emit placeholder JMP to skip procedures
    int jmpIdx = cx;
    emit("JMP", 7, 0, 0);

    // Handle declarations
    constDec();

    dx += varDec();
    procDec();

    // Fix the JMP instruction to jump to main code after declarations
    code[jmpIdx].M = cx * 3 + 10;

    // Reserve space for variables
    emit("INC", 6, 0, dx);

    // Main body
    statement();

    // Mark all symbols from this level as unavailable
    for (int i = tx - 1; i >= 0; i--)
    {
        if (table[i].level == level)
            table[i].mark = 1;
    }

    level--;
}

// Reads constants then puts them in symbol table
void constDec()
{
    // Checks to make sure its a constant 
    if(token.type == constsym)
    {
        do{
            // Handles the constant declarations
            getNextToken();
            if(token.type != identsym)
            {
                error("identifier expected after const");
            }
            int constSx = checkTable(token, 1), varSx = checkTable(token, 2);
            if((constSx != -1) || (varSx != -1))
            {
                error("identifier already declared");
            }
            // Stores name
            char name[MAXID];
            strcpy(name, token.id);
            getNextToken();
            if(token.type != eqlsym)
            {
                error("equal sign expected after const");
            }
            getNextToken();
            if(token.type != numbersym)
            {
                error("integer expected for const assignment");
            }
            // Adds constants to symbol table
            addSymbol(1, name, token.value, level, 0);
            getNextToken();
        }
        while(token.type == commasym);

        if(token.type != semicolonsym)
        {
            error("semicolon expected after const declaration");
        }
        getNextToken();
    }
}

// Parses a variable declaration section
int varDec()
{
    int numVars = 0;
    if(token.type == varsym)
    {
        do{
            numVars++;
            getNextToken();
            if(token.type != identsym)
            {
                error("identifier expected after var");
            }
            // Checks for duplicate
            int constSx = checkTable(token, 1), varSx = checkTable(token, 2);
            if((varSx != -1 && table[varSx].level >= level) || (constSx != -1))
            {
                error("identifier already declared");
            }
            addSymbol(2, token.id, 0, level, numVars + 2);
            getNextToken();
        }
        // Check for commas
        while(token.type == commasym);

        if(token.type != semicolonsym)
        {
            error("semicolon expected after var declaration");
        }
        getNextToken();
    }
    return numVars;
}

// Processe statements
void statement()
{
    if(token.type == identsym)
    {
        int symIdx = checkTable(token, 2);
        if(symIdx == -1) 
        {
            if(checkTable(token, 1) != -1 || checkTable(token, 3) != -1)
            {
                error("Assignment to constant or procedure not allowed");
            }
            undec(token);
        }
        getNextToken();
        if(token.type != becomessym)
        {
            error("assignment statements must use :=");
        }
        getNextToken();
        expression();
        emit("STO", 4, level-table[symIdx].level, table[symIdx].addr);
        return;
    }

    if(token.type == callsym)
    {
        getNextToken();
        if(token.type != identsym)
        {
            error("Identifier expected after call");
        }
        int symIdx = checkTable(token, 3);
        if(symIdx == -1)
        {
            if(checkTable(token, 1) != -1 || checkTable(token, 2) != -1)
            {
                error("Unable to call constant or variable");
            }
            undec(token);    
        }
        emit("CAL", 5, level - table[symIdx].level, table[symIdx].addr * 3 + 10);
        getNextToken();
        return;
    }

    if(token.type == beginsym)
    {
        do{
            getNextToken();
            statement();
        }while(token.type == semicolonsym);
        if(token.type != endsym)
        {
            error("begin must be followed by end");
        }
        getNextToken();
        return;
    }

    if (token.type == ifsym)
    {
        getNextToken();           // consume 'if'
        condition();
    
        int jpcIdx = cx;
        emit("JPC", 8, 0, 0);
    
        if (token.type != thensym)
            error("then expected");
    
        getNextToken();           // consume 'then'
        statement();              // parse the THEN block
    
        int jmpIdx = cx;
        emit("JMP", 7, 0, 0);
    
        int elseStart = cx;  // capture start of ELSE
        code[jpcIdx].M = elseStart * 3 + 10; // backpatch false jump
        
        //printf("DEBUG: token after THEN stmt = %d (%s)\n", token.type, token.id);
    
        // Check for elsesym IMMEDIATELY after then-statement
        if (token.type != elsesym)
        {
            error("Missing else");
        }
    
        getNextToken();           // consume 'else'
        statement();              // parse the ELSE block
    
        code[jmpIdx].M = cx * 3 + 10;
    
        if (token.type != fisym)
            error("Missing fi");
    
        getNextToken();           // consume 'fi'
        return;
    }


    if(token.type == whilesym)
    {
        getNextToken();
        int loopIdx = cx;
        condition();
        if(token.type != dosym)
        {
            error("Do expected");
        }
        getNextToken();
        int jpcIdx = cx;
        emit("JPC", 8, 0, 0);
        statement();
        emit("JMP", 7, 0, loopIdx);
        code[jpcIdx].M = cx * 3;
        return;
    }

    if(token.type == readsym)
    {
        getNextToken();
        if(token.type != identsym)
        {
            error("identifier expected after read");
        }
        int symIdx = checkTable(token, 2);
        if(symIdx == -1)
        {
            if(checkTable(token, 1) != -1 || checkTable(token, 3) != -1)
            {
                error("Assignment to constant or procedure not allowed");
            }
            undec(token);
        }
        getNextToken();
        emit("SYS", 9, 0, 2);
        emit("STO", 4, level-table[symIdx].level, table[symIdx].addr);
        return;
    }

    if(token.type == writesym)
    {
        getNextToken();
        expression();
        emit("SYS", 9, 0, 1);
        return;
    }
}

// Evaluates conditions
void condition()
{
    // All conditions begin with an expression
    expression();

    // Must be followed by a relational operator
    if (token.type == eqlsym) {
        getNextToken();
        expression();
        emit("OPR", 2, 0, 5); // EQL
    }
    else if (token.type == neqsym) {
        getNextToken();
        expression();
        emit("OPR", 2, 0, 6); // NEQ
    }
    else if (token.type == lessym) {
        getNextToken();
        expression();
        emit("OPR", 2, 0, 7); // LSS
    }
    else if (token.type == leqsym) {
        getNextToken();
        expression();
        emit("OPR", 2, 0, 8); // LEQ
    }
    else if (token.type == gtrsym) {
        getNextToken();
        expression();
        emit("OPR", 2, 0, 9); // GTR
    }
    else if (token.type == geqsym) {
        getNextToken();
        expression();
        emit("OPR", 2, 0, 10); // GEQ
    }
    else {
        error("condition must contain comparison operator");
    }
}

// Evals expressions
void expression()
{
    // Parses first term
    term();
    while(token.type == plussym || token.type == minussym)
    {
        // If plus operator
        if(token.type == plussym)
        {
            getNextToken();
            term();
            emit("OPR", 2, 0, 1);
        }
        // If minus operator
        else
        {
            getNextToken();
            term();
            emit("OPR", 2, 0, 2);
        }
    }
}

// Handles multiplication and division
void term()
{
    factor();
    while(token.type == multsym || token.type == slashsym || token.type == modsym)
    {
        // Checks if mult operator
        if(token.type == multsym)
        {
            getNextToken();
            factor();
            emit("OPR", 2, 0, 3);
        }
        // If division operator
        else if(token.type == slashsym )
        {
            getNextToken();
            factor();
            emit("OPR", 2, 0, 4);
        }
        // If mod operator 
        else if (token.type == modsym)
        {
            getNextToken();
            factor(); 
            emit("OPR", 2, 0, 11);
        }
    }
}

// Evals expressions with factors
void factor()
{
    // Makes sure the identifier exists in table
    if(token.type == identsym)
    {
        int constSymIdx = checkTable(token, 1), varSymIdx = checkTable(token, 2);
        if(constSymIdx == -1 && varSymIdx == -1) 
        {
            if(checkTable(token, 3) != -1)
            {
                error("Expression must not contain procedure identifier");
            }
            undec(token);
        }
        if(constSymIdx > varSymIdx) // Constant with that identifier is more local
        {
            emit("LIT", 1, 0, table[constSymIdx].val);
        }
        else
        {
            emit("LOD", 3, level-table[varSymIdx].level, table[varSymIdx].addr);
        }
        getNextToken();
    }

    else if(token.type == numbersym)
    {
        emit("LIT", 1, 0, token.value);
        getNextToken();
    }

    // Makes sure ) comes after (
    else if(token.type == lparentsym)
    {
        getNextToken();
        expression();
        if(token.type != rparentsym)
        {
            error("Right parenthesis must follow left parenthesis");
        }
        getNextToken();
    }

    else
    {
        error("Must conatin operands, parentheses, numbers, or symbols");
    }
}

// Newly added
void procDec()
{
    // Checks for proc sym
    if(token.type == procsym)
    {
        do{
            getNextToken();
            if(token.type != identsym)
            {
                error("Identifier expected after proc");
            }
            // Checks for an already existant ident
            int sx = checkTable(token, 3);
            if(sx != -1 && table[sx].level >= level)
            {
                error("Identifier already declared");
            }
            // Add to symbol table
            addSymbol(3, token.id, token.value, level, cx);
            getNextToken();
            // Missing ; after dec.
            if(token.type != semicolonsym)
            {
                error("Semicolon expected after proc declaration");
            }
            getNextToken();
            block();
            if(token.type != semicolonsym)
            {
                error("Semicolon expected");
            }
            getNextToken();
            emit("OPR", 2, 0, 0);
        }while(token.type == procsym);
    }
    return;
}

int main(int argc, char **argv)
{
    // Opesn file
    fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        error("file not found");
    }

    FILE *elf = fopen("elf.txt", "w");

    // Makes sure not a comment or end of file
    while(!eof)
    {
        token = getTok(fp);
        if(!flag && !eof)
        {
            lex[tokenCT] = token;
            tokenCT++;
        }
    }

    program();

    printf("No errors, program is syntactically correct.\n");

    printf("\nAssembly Code:\n\nLine\tOP\tL\tM");
    for(int i = 0; i < cx; i++)
    {
        fprintf(elf, "%d %d %d\n", code[i].OP, code[i].L, code[i].M);
        printf("\n%d\t%s\t%d\t%d", i, code[i].OP_code, code[i].L, code[i].M);
    }

    printf("\n\nSymbol Table:\n\n");
    printf("Kind | Name       | Value | Level | Address | Mark\n");
    printf("---------------------------------------------------\n");

    for (int i = 0; i < tx; i++)
    {
        printf("%-4d | %-10s | %-5d | %-5d | %-7d | %-4d\n",
            table[i].kind,
            table[i].name,
            table[i].val,
            table[i].level,
            table[i].addr,
            table[i].mark);
    }

    return 0;
}