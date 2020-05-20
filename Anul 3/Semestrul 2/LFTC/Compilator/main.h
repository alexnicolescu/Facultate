#ifndef MAIN
#define MAIN

#define SAFEALLOC(var, Type)                          \
    if ((var = (Type *)malloc(sizeof(Type))) == NULL) \
        err("Not enough memory");

enum
{
    ID,
    CT_INT,
    CT_REAL,
    CT_CHAR,
    CT_STRING,
    BREAK,
    CHAR,
    DOUBLE,
    ELSE,
    FOR,
    IF,
    INT,
    RETURN,
    STRUCT,
    VOID,
    WHILE,
    COMMA,
    SEMICOLON,
    LPAR,
    RPAR,
    LBRACKET,
    RBRACKET,
    LACC,
    RACC,
    ADD,
    SUB,
    MUL,
    DIV,
    DOT,
    AND,
    OR,
    NOT,
    ASSIGN,
    EQUAL,
    NOTEQ,
    LESS,
    LESSEQ,
    GREATER,
    GREATEREQ,
    END
};

typedef struct _Token
{
    int code;
    union {
        char *text; //folosit pt ID,CT_STRING
        long int i; //folosit pt CT_INT si CT_CHAR
        double r;   //folosit pt CT_REAL
    };
    int line;            //linia din fisierul de intrare
    struct _Token *next; //inlantuire la urmatorul AL
} Token;

#define STACK_SIZE (32 * 1024)
char stack[STACK_SIZE];
char *SP;         // Stack Pointer
char *stackAfter; // first byte after stack; used for stack limit tests

typedef struct _Instr
{
    int opcode; // O_*
    union {
        int i; // int, char
        double d;
        void *addr;
    } args[2];
    struct _Instr *last, *next; // links to last, next instructions
} Instr;
Instr *instructions, *lastInstruction; // double linked list

#define GLOBAL_SIZE (32 * 1024)
char globals[GLOBAL_SIZE];
int nGlobals;

void err(const char *fmt, ...);

void tkerr(const Token *tk, const char *fmt, ...);

void terminare();

char *codeName(int code);

void read_file(char *file);

Token *getTokens();

enum
{
    TB_INT,
    TB_DOUBLE,
    TB_CHAR,
    TB_STRUCT,
    TB_VOID
};

struct _Symbol;
typedef struct _Symbol Symbol;

typedef struct
{
    int typeBase;  // TB_*
    Symbol *s;     //struct definition for TB_STRUCT
    int nElements; //>0 array of given size,0=array without size,<0 non array
} Type;

typedef struct
{
    Symbol **begin; //the beggining of the symbols, or NULL
    Symbol **end;   //the position after the last symbol
    Symbol **after; //the position after the allocated space
} Symbols;

enum
{
    CLS_VAR,
    CLS_FUNC,
    CLS_EXTFUNC,
    CLS_STRUCT
};

enum
{
    MEM_GLOBAL,
    MEM_ARG,
    MEM_LOCAL
};

typedef struct _Symbol
{
    const char *name; // a reference to the name stored in a token
    int cls;          //CLS_*
    int mem;          //MEM_*
    Type type;
    int depth; //0-global, 1-in function,2... -nested blocks in function
    union {
        Symbols args;    // used only for functions
        Symbols members; // unsed only for structs
    };
    union {
        void *addr; //vm: the memory address for global symbols
        int offset; // vm: the stack offset for local symbols
    };
} Symbol;

typedef union {
    long int i;      //int, char
    double d;        //double
    const char *str; //char[]
} CtVal;

typedef struct
{
    Type type;   //type of the result
    int isLVal;  //if it is a LVal
    int isCtVal; //if it is a constant value(int, real, char, char[])
    CtVal ctVal; //the constant value
} RetVal;

Symbols symbols;
int crtDepth;
extern Symbol *crtFunc, *crtStruct;
extern Token *consumedTk, *crtTk;
extern int offset;

void initSymbols(Symbols *symbols);

Symbol *addSymbol(Symbols *symbols, const char *name, int cls);

Symbol *findSymbol(Symbols *symbols, const char *name);

Symbol *requireSymbol(Symbols *symbols, const char *name);

void deleteSymbolsAfter(Symbols *symbols, Symbol *start);

void printSymbols(Symbols *symbols);

Type createType(int typeBase, int nElements);

Symbol *addExtFunc(const char *name, Type type, void *addr);

void addExtFuncs();

void addVar(Token *tkName, Type *t);

void cast(Type *dst, Type *src);

Type getArithType(Type *s1, Type *s2);

enum // all opcodes; each one starts with O_
{
    O_ADD_C,
    O_ADD_D,
    O_ADD_I,
    O_AND_A,
    O_AND_C,
    O_AND_D,
    O_AND_I,
    O_CALL,
    O_CALLEXT,
    O_CAST_C_D,
    O_CAST_C_I,
    O_CAST_D_C,
    O_CAST_D_I,
    O_CAST_I_C,
    O_CAST_I_D,
    O_DIV_C,
    O_DIV_D,
    O_DIV_I,
    O_DROP,
    O_ENTER,
    O_EQ_A,
    O_EQ_C,
    O_EQ_D,
    O_EQ_I,
    O_GREATER_C,
    O_GREATER_D,
    O_GREATER_I,
    O_GREATEREQ_C,
    O_GREATEREQ_D,
    O_GREATEREQ_I,
    O_HALT,
    O_INSERT,
    O_JF_A,
    O_JF_C,
    O_JF_D,
    O_JF_I,
    O_JMP,
    O_JT_A,
    O_JT_C,
    O_JT_D,
    O_JT_I,
    O_LESS_C,
    O_LESS_D,
    O_LESS_I,
    O_LESSEQ_C,
    O_LESSEQ_D,
    O_LESSEQ_I,
    O_LOAD,
    O_MUL_C,
    O_MUL_D,
    O_MUL_I,
    O_NEG_C,
    O_NEG_D,
    O_NEG_I,
    O_NOP,
    O_NOT_A,
    O_NOT_C,
    O_NOT_D,
    O_NOT_I,
    O_NOTEQ_A,
    O_NOTEQ_C,
    O_NOTEQ_D,
    O_NOTEQ_I,
    O_OFFSET,
    O_OR_A,
    O_OR_C,
    O_OR_D,
    O_OR_I,
    O_PUSHFPADDR,
    O_PUSHCT_A,
    O_PUSHCT_C,
    O_PUSHCT_D,
    O_PUSHCT_I,
    O_RET,
    O_STORE,
    O_SUB_C,
    O_SUB_D,
    O_SUB_I,
};

int popi();

void pushd(double d);

double popd();

void pusha(void *a);

void *popa();

void pushi(int i);

int popi();

void pushc(char c);

char popc();

Instr *createInstr(int opcode);

void insertInstrAfter(Instr *after, Instr *i);

Instr *addInstr(int opcode);

Instr *addInstrAfter(Instr *after, int opcode);

Instr *addInstrA(int opcode, void *addr);

Instr *addInstrI(int opcode, int val);

Instr *addInstrII(int opcode, int val1, int val2);

void deleteInstructionsAfter(Instr *start);

Instr *appendInstr(Instr *i);

void *allocGlobal(int size);

void run(Instr *IP);

void showInstructions();

Instr *createCondJmp(RetVal *rv);

int typeBaseSize(Type *type);

int typeFullSize(Type *type);

int typeArgSize(Type *type);

Instr *getRVal(RetVal *rv);

void addCastInstr(Instr *after, Type *actualType, Type *neededType);

#endif