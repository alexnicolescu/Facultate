#ifndef ALEX
#define ALEX

#define SAFEALLOC(var,Type)if((var=(Type*)malloc(sizeof(Type)))==NULL)err("Not enough memory");


enum{ID,CT_INT,CT_REAL,CT_CHAR,CT_STRING,BREAK,CHAR,DOUBLE,ELSE,FOR,IF,INT,RETURN,STRUCT,VOID,WHILE,COMMA,
	SEMICOLON,LPAR,RPAR,LBRACKET,RBRACKET,LACC,RACC,ADD,SUB,MUL,DIV,DOT,AND,OR,NOT,ASSIGN,EQUAL,NOTEQ,LESS,LESSEQ,GREATER,GREATEREQ,END};

typedef struct _Token{
		int code;
		union{
		char *text;//folosit pt ID,CT_STRING
		long int i;//folosit pt CT_INT si CT_CHAR
		double r;//folosit pt CT_REAL
	};
	int line; //linia din fisierul de intrare
	struct _Token *next; //inlantuire la urmatorul AL
}Token;

void err(const char *fmt,...);

void tkerr(const Token *tk,const char *fmt,...);


void terminare();


char *codeName(int code);

void read_file(char *file);

Token* getTokens();

#endif