#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>



#define SAFEALLOC(var,Type)if((var=(Type*)malloc(sizeof(Type)))==NULL)err("Not enough memory");

void err(const char *fmt,...)
{
	va_list va;
	va_start(va,fmt);
	fprintf(stderr,"Error:");
	vfprintf(stderr,fmt,va);
	fputc('\n',stderr);
	va_end(va);
	exit(-1);
}

enum{ID,CT_INT,CT_REAL,CT_CHAR,CT_STRING,BREAK,CHAR,DOUBLE,ELSE,FOR,IF,INT,RETURN,STRUCT,VOID,WHILE,COMMA,
	SEMICOLON,LPAR,RPAR,LBRACKET,RBRACKET,LACC,RACC,ADD,SUB,MUL,DIV,DOT,AND,OR,NOT,ASSIGN,EQUAL,NOTEQ,LESS,LESSEQ,GREATER,GREATEREQ,END};

	typedef struct _Token{
		int code;
		union{
		char *text;//folosit pt ID,CT_STRING
		long int i;//folosit pt CT_INT
		double r;//folosit pt CT_REAL
	};
	int line; //linia din fisierul de intrare
	struct _Token *next; //inlantuire la urmatorul AL
}Token;

Token *tokens=NULL,*lastToken=NULL;

void tkerr(const Token *tk,const char *fmt,...)
{
	va_list va;
	va_start(va,fmt);
	fprintf(stderr,"Error in line %d:",tk->line);
	vfprintf(stderr,fmt,va);
	fputc('\n',stderr);
	va_end(va);
	exit(-1);
}

int line;

Token *addTk(int code)
{
	Token *tk;
	SAFEALLOC(tk,Token);
	tk->code=code;
	tk->line=line;
	tk->next=NULL;
	if(lastToken){
		lastToken->next=tk;
	}
	else
	{
		tokens=tk;
	}
	lastToken=tk;
	return tk;
	
}

char buf[30000],*pCrtCh;

char* createString(const char *i,char *s){
	int nCh=s-i;
	int j=0;
	char *text;
	if((text=(char*)malloc(nCh+1))==NULL)
	{
		err("Not enough memory");
	}
	if((*i)=='\"')
	{
		i++;
		nCh--;
	}
	while(j!=nCh)
	{
		if((*i)=='\\')
		{
			i++;
			switch((*i)){
				case 'a':
				text[j]='\a';
				break;
				case 'b':
				text[j]='\b';
				break;
				case 'f':
				text[j]='\f';
				break;
				case 'n':
				text[j]='\n';
				break;
				case 'r':
				text[j]='\r';
				break;
				case 't':
				text[j]='\t';
				break;
				case 'v':
				text[j]='\v';
				break;
				case '\'':
				text[j]='\'';
				break;
				case '?':
				text[j]='\?';
				break;
				case '\"':
				text[j]='\"';
				break;
				case '\\':
				text[j]='\\';
				break;
				case '\0':
				text[j]='\0';
				break;
			}
			nCh--;		
		}
		else
		{
			text[j]=(*i);
		}
		i++;
		j++;
		

	}
	if(text[j-1]!='\"')
		text[j]='\0';
	else
	{
		text[j-1]='\0';
	}
	
	return text;
}

int convertToInt(const char *i,char *s)
{
	int nCh=s-i;
	int j=0,oc=0,hex=0;
	char nr[1024];
	if((*i)=='0')
	{
		oc=1;	
	}
	while(j!=nCh)
	{
		nr[j]=*i;
		if(tolower(*i)=='x')
			hex=1;
		i++;
		j++;
	}
	nr[j]='\0';
	if(hex==1)
	{
		return (int)strtol(nr,NULL,0);
	}
	else
	{
		if(oc==1)
		{
			return (int)strtol(nr,NULL,8);
		}
		else
		{
			return atol(nr);
		}
		
	}
	
	
}

double convertToDouble(const char *i,char *s)
{
	int nCh=s-i;
	int j=0;
	char nr[1024];
	while(j!=nCh)
	{
		nr[j]=*i;
		i++;
		j++;
	}
	nr[j]='\0';
	return atof(nr);
}

int getNextToken()
{
	int state=0;
	int nCh;
	char ch;
	const char *pStartCh;
	Token *tk;
	while(1){
		ch=*pCrtCh;
		switch (state)
		{
			case 0:
			if(isalpha(ch)||ch=='_'){
				pStartCh=pCrtCh;
				pCrtCh++;
				state=1;
			}
			else
			{
				if(ch=='\'')
				{
					pStartCh=pCrtCh;
					pCrtCh++;
					state=3;
				}
				else
				{
					if(ch=='\"'){
						pStartCh=pCrtCh;
						pCrtCh++;
						state=7;
					}
					else
					{
						if(ch>='1' && ch <='9')
						{
							pStartCh=pCrtCh;
							pCrtCh++;
							state=10;
						}
						else
						{
							if(ch=='0')
							{
								pStartCh=pCrtCh;
								pCrtCh++;
								state=12;
							}
							else
							{
								if(ch=='\n')
								{
									pCrtCh++;
									line++;
								}
								else
									if(ch=='\r'||ch=='\t'||ch==' ')
									{
										pCrtCh++;
									}
									else
									{
										if(ch=='/')
										{
											pCrtCh++;
											state=23;
										}
										else
										{
											if(ch==',')
											{
												pCrtCh++;
												state=27;
											}
											else
											{
												if(ch==';')
												{
													pCrtCh++;
													state=28;
												}
												else
												{
													if(ch=='(')
													{
														pCrtCh++;
														state=29;
													}
													else
													{
														if(ch==')')
														{
															pCrtCh++;
															state=30;
														}
														else
														{
															if(ch=='[')
															{
																pCrtCh++;
																state=31;
															}
															else
															{
																if(ch==']')
																{
																	pCrtCh++;
																	state=32;
																}
																else
																{
																	if(ch=='{')
																	{
																		pCrtCh++;
																		state=33;
																	}
																	else
																	{
																		if(ch=='}')
																		{
																			pCrtCh++;
																			state=34;
																		}
																		else
																		{
																			if(ch=='+')
																			{
																				pCrtCh++;
																				state=35;	
																			}
																			else
																			{
																				if(ch=='-')
																				{
																					pCrtCh++;
																					state=36;
																				}
																				else
																				{
																					if(ch=='*')
																					{
																						pCrtCh++;
																						state=37;
																					}
																					else
																					{
																						
																						if(ch=='.')
																						{
																							pCrtCh++;
																							state=39;
																						}
																						else
																						{
																							if(ch=='&')
																							{
																								pCrtCh++;
																								state=40;
																							}
																							else
																							{
																								if(ch=='|')
																								{
																									pCrtCh++;
																									state=42;
																								}
																								else
																								{
																									if(ch=='!')
																									{
																										pCrtCh++;
																										state=44;
																									}
																									else
																									{
																										if(ch=='=')
																										{
																											pCrtCh++;
																											state=47;
																										}
																										else
																										{
																											if(ch=='<')
																											{
																												pCrtCh++;
																												state=50;
																											}
																											else
																											{
																												if(ch=='>')
																												{
																													pCrtCh++;
																													state=53;
																												}
																												else
																												{	
																													if(ch=='\0')
																													{
																														addTk(END);
																														return END;
																													}
																													else
																													{
																														tkerr(addTk(END),"Invalid character");
																													}
																													
																													
																												}
																												
																											}
																											
																										}
																										
																									}
																									
																								}
																								
																							}
																							
																						}
																						
																						
																						
																					}
																					
																				}
																				
																			}
																			
																		}
																		
																	}
																	
																}
																
															}
															
														}
														
													}
													
												}
												
											}
											
										}
										
									}
									
									
								}
								
							}
							
						}
						
					}
					
				}
				
				break;
				case 1:
				if(isalnum(ch)||ch=='_')
					pCrtCh++;
				else
				{
					state=2;
				}
				break;
				case 2:
				nCh=pCrtCh-pStartCh;
				if(nCh==5 && !memcmp(pStartCh,"break",5)){
					addTk(BREAK);
					return BREAK;
				}
				else{
					if(nCh==4 && !memcmp(pStartCh,"char",4)){
						addTk(CHAR);
						return CHAR;
					}
					else{
						if(nCh==6 && !memcmp(pStartCh,"double",6)){
							addTk(DOUBLE);
							return DOUBLE;
						}
						else{
							if(nCh==4 && !memcmp(pStartCh,"else",4)){
								addTk(ELSE);
								return ELSE;
							}
							else{
								if(nCh==3 && !memcmp(pStartCh,"for",3)){
									addTk(FOR);
									return FOR;
								}
								else{
									if(nCh==2 && !memcmp(pStartCh,"if",2)){
										addTk(IF);
										return IF;
									}
									else{
										if(nCh==3 && !memcmp(pStartCh,"int",3)){
											addTk(INT);
											return INT;
										}
										else{
											if(nCh==6 && !memcmp(pStartCh,"return",6)){
												addTk(RETURN);
												return RETURN;
											}
											else{
												if(nCh==6 && !memcmp(pStartCh,"struct",6)){
													addTk(STRUCT);
													return STRUCT;
												}
												else{
													if(nCh==4 && !memcmp(pStartCh,"void",4)){
														addTk(VOID);
														return VOID;
													}
													else{
														if(nCh==5 && !memcmp(pStartCh,"while",5)){
															addTk(WHILE);
															return WHILE;
														}
														else
														{
															tk=addTk(ID);
															tk->text=createString(pStartCh,pCrtCh);
														// printf("ID:%s\n",tk->text);
															return tk->code;
														}
														
													}
												}
											}
										}
									}

								}
							}
						}
					}
				}
				case 3:
				if(ch=='\\')
				{
					pCrtCh++;
					state=4;
				}
				else
				{
					if(ch!='\'' && ch!='\\')
					{
						pCrtCh++;
						state=5;
					}
					else
					{
						tkerr(addTk(END),"Invalid character");
					}
					
					
				}
				break;
				case 4:
				if(strchr("abfnrtv'?\"\\0",ch))
				{
					pCrtCh++;
					state=5;
				}
				else
				{
					tkerr(addTk(END),"Invalid character");
				}
				
				break;
				case 5:
				if(ch=='\'')
				{
					pCrtCh++;
					state=6;
				}
				else
				{
					tkerr(addTk(END),"Invalid character");
				}
				
				break;
				case 6:
				tk=addTk(CT_CHAR);
				if(pStartCh[1]=='\\')
				{
					switch(pStartCh[2]){
						case 'a':
						tk->i='\a';
						break;
						case 'b':
						tk->i='\b';
						break;
						case 'f':
						tk->i='\f';
						break;
						case 'n':
						tk->i='\n';
						break;
						case 'r':
						tk->i='\r';
						break;
						case 't':
						tk->i='\t';
						break;
						case 'v':
						tk->i='\v';
						break;
						case '\'':
						tk->i='\'';
						break;
						case '?':
						tk->i='\?';
						break;
						case '\"':
						tk->i='\"';
						break;
						case '\\':
						tk->i='\\';
						break;
						case '\0':
						tk->i='\0';
						break;
					}
				}
				else
				{
					tk->i=pStartCh[1];
				}
				
			// printf("CT_CHAR:%c\n",tk->c);
				return tk->code;
				case 7:
				if(ch=='\\')
				{
					pCrtCh++;
					state=8;
				}
				else
				{
					if(ch=='\"')
					{
						pCrtCh++;
						state=9;
						// printf("%c\n",ch);
					}
					else
					{
						if(ch!='\"' && ch !='\\')
						{
							pCrtCh++;
							// printf("%c\n",ch);
						}
					}
					
				}	
				break;
				case 8:
				if(strchr("abfnrtv'?\"\\0",ch))
				{
					pCrtCh++;
					state=7;
				}
				else
				{
					tkerr(addTk(END),"Invalid character");
				}
				
				break;
				case 9:
				tk=addTk(CT_STRING);
				tk->text=createString(pStartCh,pCrtCh);
			// printf("CT_STRING:%s\n",tk->text);
				return tk->code;
				case 10:
				if(isdigit(ch))
				{
					pCrtCh++;
				}
				else{
					if(tolower(ch)=='e')
					{
						pCrtCh++;
						state=20;
					}
					else
					{
						if(ch=='.')
						{
							pCrtCh++;
							state=17;
						}
						else
						{
							state=11;
						}
						
						
					}
				}
				break;
				case 11:
				tk=addTk(CT_INT);
				tk->i=convertToInt(pStartCh,pCrtCh);
			// printf("CT_INT:%li\n",tk->i);
				return tk->code;
				case 12:
				if(ch=='x')
				{
					pCrtCh++;
					state=14;
				}
				else
				{
					if(ch=='.')	
					{
						pCrtCh++;
						state=17;
					}
					else
					{
						state=13;
					}
					
				}
				break;
				case 13:
				if(ch>='0' && ch<='7')
				{
					pCrtCh++;
				}
				else
				{
					if(ch=='8' || ch=='9')
					{
						state=16;
						pCrtCh++;
					}
					else
					{
						if(ch=='.')
						{
							pCrtCh++;
							state=17;
						}
						else
						{
							state=11;	
							
						}
						
					}
				}
				break;
				case 14:
				if(isdigit(ch)||(tolower(ch)>='a' && tolower(ch)<='f'))
				{
					pCrtCh++;
					state=15;
				}
				else
				{
					tkerr(addTk(END),"Invalid character");
				}
				
				break;	
				case 15:
				if(isdigit(ch)||(tolower(ch)>='a' && tolower(ch)<='f'))
				{
					pCrtCh++;
				}
				else
				{
					if(tolower(ch)=='e')
					{
						pCrtCh++;
						state=20;
					}
					else
					{					
						state=11;	
					}

				}
				break;
				case 16:
				if(isdigit(ch))
				{
					pCrtCh++;
				}
				else
				{
					if(ch=='.')
					{
						state=17;
						pCrtCh++;
					}
					else
					{
						tkerr(addTk(END),"Invalid character");
						
					}
					
				}
				break;
				case 17:
				if(isdigit(ch))
				{
					state=18;
					pCrtCh++;
				}
				else
				{
					tkerr(addTk(END),"Invalid character");
				}
				
				break;
				case 18:
				if(isdigit(ch))
				{
					pCrtCh++;
				}
				else
				{
					if(tolower(ch)=='e')
					{
						state=20;
						pCrtCh++;
					}
					else
					{
						state=19;
					}
					
				}
				break;
				case 19:
				tk=addTk(CT_REAL);
				tk->r=convertToDouble(pStartCh,pCrtCh);
			// printf("CT_REAL:%lf\n",tk->r);
				return tk->code;
				case 20:
				if(ch=='+'||ch=='-')
				{
					state=21;
					pCrtCh++;
				}
				else
				{
					state=21;
				}
				break;
				case 21:
				if(isdigit(ch))
				{
					state=22;
					pCrtCh++;
				}
				else
				{
					tkerr(addTk(END),"Invalid character");
				}
				
				break;
				case 22:
				if(isdigit(ch))
				{
					pCrtCh++;
				}
				else
				{
					state=19;
				}
				break;
				case 23:
				if(ch=='/')
				{
					state=24;
					pCrtCh++;
				}
				else
				{
					if(ch=='*')
					{
						state=25;
						pCrtCh++;
					}
					else
					{
						state=38;
					}
					
				}
				break;
				case 24:
				if(ch!='\n' && ch!='\r' && ch!='\0')
				{
					pCrtCh++;
				}
				else
				{
					state=0;
				}
				
				break;
				case 25:
				if(ch!='*')
				{
					pCrtCh++;
				}
				else
				{
					if(ch=='*')
					{
						pCrtCh++;
						state=26;
					}
				}
				break;
				case 26:
				if(ch=='*')
				{
					pCrtCh++;
				}
				else
				{
					if(ch=='/')
					{
						state=0;
						pCrtCh++;
					}
					else
					{
						if(ch!='*' && ch!='/')
						{
							state=25;
							pCrtCh++;
						}
					}
					
				}
				break;
				
				case 27:
				addTk(COMMA);
				return COMMA;
				case 28:
				addTk(SEMICOLON);
				return SEMICOLON;
				case 29:
				addTk(LPAR);
				return LPAR;
				case 30:
				addTk(RPAR);
				return RPAR;
				case 31:
				addTk(LBRACKET);
				return LBRACKET;
				case 32:
				addTk(RBRACKET);
				return RBRACKET;
				case 33:
				addTk(LACC);
				return LACC;
				case 34:
				addTk(RACC);
				return RACC;
				case 35:
				addTk(ADD);
				return ADD;
				case 36:
				addTk(SUB);
				return SUB;
				case 37:
				addTk(MUL);
				return MUL;
				case 38:
				addTk(DIV);
				return DIV;
				case 39:
				addTk(DOT);
				return DOT;
				case 40:
				if(ch=='&')
				{
					pCrtCh++;
					state=41;
				}
				else
				{
					tkerr(addTk(END),"Invalid character");
				}
				
				break;
				case 41:
				addTk(AND);
				return AND;
				case 42:
				if(ch=='|')
				{
					pCrtCh++;
					state=43;
				}
				else
				{
					tkerr(addTk(END),"Invalid character");
				}
				
				break;
				case 43:
				addTk(OR);
				return OR;
				case 44:
				if(ch=='=')
				{
					pCrtCh++;
					state=45;
				}
				else
				{
					state=46;
				}
				break;
				case 45:
				addTk(NOTEQ);
				return NOTEQ;
				case 46:
				addTk(NOT);
				return NOT;
				case 47:
				if(ch=='=')
				{
					pCrtCh++;
					state=48;
				}
				else
				{
					state=49;
				}
				break;
				case 48:
				addTk(EQUAL);
				return EQUAL;
				case 49:
				addTk(ASSIGN);
				return ASSIGN;
				case 50:
				if(ch=='=')
				{
					pCrtCh++;
					state=51;
				}
				else
				{
					state=52;
				}
				break;
				case 51:
				addTk(LESSEQ);
				return LESSEQ;
				case 52:
				addTk(LESS);
				return LESS;
				case 53:
				if(ch=='=')
				{
					pCrtCh++;
					state=54;
				}
				else
				{
					state=55;
				}
				break;
				case 54:
				addTk(GREATEREQ);
				return GREATEREQ;
				case 55:
				addTk(GREATER);
				return GREATER;
				default:
				err("Invalid state");
			}
		}
	}

	void terminare()
	{
		while(tokens)
		{
			Token *t;
			t=tokens;
			if(t->code==4)
			{
				free(t->text);
			}
			free(t);
			tokens=tokens->next;
		}
		tokens=NULL;
		lastToken=NULL;
	}

	void showAtoms(){
		Token *token;
		token=tokens;
		while(token)
		{
			printf("%d",token->code);
			switch(token->code){
				case 0:
				printf(" ID:%s",token->text);
				break;
				case 1:
				printf(" CT_INT:%li",token->i);
				break;
				case 2:
				printf(" CT_REAL:%lf",token->r);
				break;
				case 3:
				printf(" CT_CHAR:%c",(char)token->i);
				break;
				case 4:
				printf(" CT_STRING:%s",token->text);
				break;
				case 5:
				printf(" BREAK");
				break;
				case 6:
				printf(" CHAR");
				break;
				case 7:
				printf(" DOUBLE");
				break;
				case 8:
				printf(" ELSE");
				break;
				case 9:
				printf(" FOR");
				break;
				case 10:
				printf(" IF");
				break;
				case 11:
				printf(" INT");
				break;
				case 12:
				printf(" RETURN");
				break;
				case 13:
				printf(" STRUCT");
				break;
				case 14:
				printf(" VOID");
				break;
				case 15:
				printf(" WHILE");
				break;
				case 16:
				printf(" COMMA");
				break;
				case 17:
				printf(" SEMICOLON");
				break;
				case 18:
				printf(" LPAR");
				break;
				case 19:
				printf(" RPAR");
				break;
				case 20:
				printf(" LBRACKET");
				break;
				case 21:
				printf(" RBRACKET");
				break;
				case 22:
				printf(" LACC");
				break;
				case 23:
				printf(" RACC");
				break;
				case 24:
				printf(" ADD");
				break;
				case 25:
				printf(" SUB");
				break;
				case 26:
				printf(" MUL");
				break;
				case 27:
				printf(" DIV");
				break;
				case 28:
				printf(" DOT");
				break;
				case 29:
				printf(" AND");
				break;
				case 30:
				printf(" OR");
				break;
				case 31:
				printf(" NOT");
				break;
				case 32:
				printf(" ASSIGN");
				break;
				case 33:
				printf(" EQUAL");
				break;
				case 34:
				printf(" NOTEQ");
				break;
				case 35:
				printf(" LESS");
				break;
				case 36:
				printf(" LESSEQ");
				break;
				case 37:
				printf(" GREATER");
				break;
				case 38:
				printf(" GREATEREQ");
				break;
				case 39:
				printf(" END");
				break;

			}
			printf("\n");
			token=token->next;
		}
	}

	char *codeName(int code){
		switch(code)
		{
			case 0:
				return "ID";
			case 1:
				return "CT_INT";
			case 2:
				return "CT_REAL";
			case 3:
				return "CT_CHAR";
			case 4:
				return "CT_STRING";
			case 5:
				return "BREAK";
			case 6:
				return "CHAR";
			case 7:
				return "DOUBLE";
			case 8:
				return "ELSE";
			case 9:
				return "FOR";
			case 10:
				return "IF";
			case 11:
				return "INT";
			case 12:
				return "RETURN";
			case 13:
				return "STRUCT";
			case 14:
				return "VOID";
			case 15:
				return "WHILE";
			case 16:
				return "COMMA";
			case 17:
				return "SEMICOLON";
			case 18:
				return "LPAR";
			case 19:
				return "RPAR";
			case 20:
				return "LBRACKET";
			case 21:
				return "RBRACKET";
			case 22:
				return "LACC";
			case 23:
				return "RACC";
			case 24:
				return "ADD";
			case 25:
				return "SUB";
			case 26:
				return "MUL";
			case 27:
				return "DIV";
			case 28:
				return "DOT";
			case 29:
				return "AND";
			case 30:
				return "OR";
			case 31:
				return "NOT";
			case 32:
				return "ASSIGN";
			case 33:
				return "EQUAL";
			case 34:
				return "NOTEQ";
			case 35:
				return "LESS";
			case 36:
				return "LESSEQ";
			case 37:
				return "GREATER";
			case 38:
				return "GREATEREQ";
			case 39:
				return "END";
		}
		return NULL;
	}

	int unit();
	int declStruct();
	int declVar();
	int typeBase();
	int arrayDecl();
	int typeName();
	int declFunc();
	int funcArg();
	int stm();
	int stmCompound();
	int expr();
	int exprAssign();
	int exprOr();
	int exprAnd();
	int exprEq();
	int exprRel();
	int exprAdd();
	int exprMul();
	int exprCast();
	int exprUnary();
	int exprPostfix();
	int exprPrimary();

	Token *consumedTk,*crtTk;

	int consume(int code){
		printf("Consume(%s)",codeName(code));
		if(crtTk->code==code){
			consumedTk=crtTk;
			crtTk=crtTk->next;
			printf("=>consumed\n");
			return 1;
		}
		printf("=>something else(%s)\n",codeName(crtTk->code));
		return 0;
	}

	int unit(){
		Token *startTk=crtTk;
		printf("@unit %s\n",codeName(crtTk->code));
		while(1)
		{
			if(declStruct()){}
			else
				if(declFunc()){}
				else
					if(declVar()){}
					else
						break;
		}
		if(consume(END))
			return 1;
		crtTk=startTk;
		return 0;
	}

	int declStruct(){
		Token *startTk=crtTk;
		printf("@declStruct %s\n",codeName(crtTk->code));
		if(consume(STRUCT))
		{
			if(consume(ID))
			{
				if(consume(LACC))
				{
					while(declVar());
					if(consume(RACC))
					{
						if(consume(SEMICOLON))
							return 1;
					}
				}
			}
		}
		crtTk=startTk;
		return 0;
	}

	int declVar(){
		Token *startTk=crtTk;
		printf("@declVar %s\n",codeName(crtTk->code));
		if(typeBase())
		{
			if(consume(ID))
			{
				arrayDecl();
				while(1)
				{
					if(consume(COMMA))
					{
						if(consume(ID))
						{
							arrayDecl();
						}
						else
						{
							break;
						}
						
					}
					else
					{
						break;
					}
					
				}
				if(consume(SEMICOLON))
				{
					return 1;
				}
			}
		}
		crtTk=startTk;
		return 0;
	}

	int typeBase(){
		Token *startTk=crtTk;
		printf("@typeBase %s\n",codeName(crtTk->code));
		if(consume(INT)){
			return 1;
		}
		if(consume(DOUBLE))
			return 1;
		if(consume(CHAR))
			return 1;
		if(consume(STRUCT))
			{
				if(consume(ID))
					return 1;
			}
		crtTk=startTk;
		return 0;
	}

	int arrayDecl(){
		Token *startTk=crtTk;
		printf("@arrayDecl %s\n",codeName(crtTk->code));
		if(consume(LBRACKET))
		{
			expr();
			if(consume(RBRACKET))
				return 1;
		}
		crtTk=startTk;
		return 0;
	}

	int typeName(){
		Token *startTk=crtTk;
		printf("@typeName %s\n",codeName(crtTk->code));
		if(typeBase()){
			arrayDecl();
			return 1;
		}
		crtTk=startTk;
		return 0;
	}

	int declFunc(){
		Token *startTk=crtTk;
		printf("@declFunc %s\n",codeName(crtTk->code));
		int ok=0;
		if(typeBase())
		{	
			ok=1;
			consume(MUL);
		}
		else
		{
			if(consume(VOID))
				ok=1;
		}
		if(ok==1){
			if(consume(ID))
			{
				if(consume(LPAR))
				{
					funcArg();
					while(1){
						if(consume(COMMA))
						{
							if(funcArg()){}
							else
								break;
						}
						else break;
					}
					if(consume(RPAR))
					{
						if(stmCompound())
							return 1;
					}
				}
			}
		}
		crtTk=startTk;
		return 0;
		
	}

	int funcArg(){
		Token *startTk=crtTk;
		printf("@funcArg %s\n",codeName(crtTk->code));
		if(typeBase()){
			if(consume(ID))
			{
				arrayDecl();
				return 1;
			}
		}
		crtTk=startTk;
		return 0;
	}

	int stm(){
		Token *startTk=crtTk;
		printf("@stm %s\n",codeName(crtTk->code));
		if(stmCompound())
			return 1;
		if(consume(IF))
			{
				if(consume(LPAR))
				{
					if(expr())
					{
						if(consume(RPAR))
						{
							if(stm())
							{
								if(consume(ELSE))
								{
									stm();
								}
								return 1;
							}
						}
					}
				}
			}
		if(consume(WHILE))
			{
				if(consume(LPAR))
				{
					if(expr())
					{
						if(consume(RPAR))
						{
							if(stm())
							{
								return 1;
							}
						}
					}
				}
			}
		if(consume(FOR))
			{
				if(consume(LPAR))
					{
						expr();
						if(consume(SEMICOLON))
						{
							expr();
							if(consume(SEMICOLON))
							{
								expr();
								if(consume(RPAR))
								{
									if(stm())
									{
										return 1;
									}
								}
							}
						}
					}
				}
		if(consume(BREAK))
			{
				if(consume(SEMICOLON))
					{
						return 1;
					}
			}
		if(consume(RETURN))
			{
				expr();
				if(consume(SEMICOLON))
					{
						return 1;
					}
			}
		else
			{
				expr();
				if(consume(SEMICOLON))
					{
						return 1;
					}
			}
		crtTk=startTk;
		return 0;
	}

	int stmCompound(){
		Token *startTk=crtTk;
		printf("@stmCompound %s\n",codeName(crtTk->code));
		if(consume(LACC))
		{
			while(declVar() || stm());
			if(consume(RACC))
			{
				return 1;
			}
		}
		crtTk=startTk;
		return 0;
	}

	int expr(){
		Token *startTk=crtTk;
		printf("@expr %s\n",codeName(crtTk->code));
		if(exprAssign())
			return 1;
		crtTk=startTk;
		return 0;	
	}

	int exprAssign(){/////////////////////////////
		Token *startTk=crtTk;
		printf("@exprAssign %s\n",codeName(crtTk->code));
		if(exprUnary())
		{
			if(consume(ASSIGN))
			{
				if(exprAssign())
				{
					return 1;
				}
			}
		}
		crtTk=startTk;
		if(exprOr())
		{
			return 1;
		}
		crtTk=startTk;
		return 0;
	}

	int exprOrPrim(){
		printf("@exprOrPrim %s\n",codeName(crtTk->code));
		if(consume(OR))
		{
			if(exprAnd())
			{
				if(exprOrPrim())
				{
					return 1;
				}
			}
		}
		return 1;
	}

	int exprOr(){
		Token *startTk=crtTk;
		printf("@exprOr %s\n",codeName(crtTk->code));
		if(exprAnd())
		{
			if(exprOrPrim())
			{
				return 1;
			}
		}
		crtTk=startTk;
		return 0;

	}

	int exprAndPrim(){
		printf("@exprAndPrim %s\n",codeName(crtTk->code));
		if(consume(AND))
		{
			if(exprEq())
			{
				if(exprAndPrim())
				{
					return 1;
				}
			}
		}
		return 1;
	}

	int exprAnd(){
		Token *startTk=crtTk;
		printf("@exprAnd %s\n",codeName(crtTk->code));
		if(exprEq())
		{
			if(exprAndPrim())
			{
				return 1;
			}
		}
		crtTk=startTk;
		return 0;
	}

	int exprEqPrim(){
		printf("@exprEqPrim %s\n",codeName(crtTk->code));
		if(consume(EQUAL)||consume(NOTEQ))
		{
			if(exprRel())
			{
				if(exprEqPrim())
				{
					return 1;
				}
			}
		}
		return 1;
	}

	int exprEq(){
		Token *startTk=crtTk;
		printf("@exprEq %s\n",codeName(crtTk->code));
		if(exprRel())
		{
			if(exprEqPrim())
			{
				return 1;
			}
		}
		crtTk=startTk;
		return 0;
	}

	int exprRelPrim(){
		printf("@exprRelPrim %s\n",codeName(crtTk->code));
		if(consume(LESS)||consume(LESSEQ)||consume(GREATER)||consume(GREATEREQ))
		{
			if(exprAdd())
			{
				if(exprRelPrim())
				{
					return 1;
				}
			}
		}
		return 1;
	}

	int exprRel(){
		Token *startTk=crtTk;
		printf("@exprRel %s\n",codeName(crtTk->code));
		if(exprAdd())
		{
			if(exprRelPrim())
			{
				return 1;
			}
		}
		crtTk=startTk;
		return 0;
	}

	int exprAddPrim(){
		printf("@exprAddPrim %s\n",codeName(crtTk->code));
		if(consume(ADD)||consume(SUB))
		{
			if(exprMul())
			{
				if(exprAddPrim())
				{
					return 1;
				}
			}
		}
		return 1;
	}
	
	int exprAdd(){
		Token *startTk=crtTk;
		printf("@exprAdd %s\n",codeName(crtTk->code));
		if(exprMul())
		{
			if(exprAddPrim())
			{
				return 1;
			}
		}
		crtTk=startTk;
		return 0;
	}

	int exprMulPrim(){
		printf("@exprMulPrim %s\n",codeName(crtTk->code));
		if(consume(MUL)||consume(DIV))
		{
			if(exprCast())
			{
				if(exprMulPrim())
				{
					return 1;
				}
			}
		}
		return 1;
	}

	int exprMul(){
		Token *startTk=crtTk;
		printf("@exprMul %s\n",codeName(crtTk->code));
		if(exprCast())
		{
			if(exprMulPrim())
			{
				return 1;
			}
		}
		crtTk=startTk;
		return 0;
	}

	int exprCast(){
		Token *startTk=crtTk;
		printf("@exprCast %s\n",codeName(crtTk->code));
		if(consume(LPAR))
		{
			if(typeName())
			{
				if(consume(RPAR))
				{
					if(exprCast())
					{
						return 1;
					}
				}
			}
		}
		if(exprUnary())
			return 1;
		crtTk=startTk;
		return 0;
	}

	int exprUnary(){
		Token *startTk=crtTk;
		printf("@exprUnary %s\n",codeName(crtTk->code));
		if(consume(SUB)||consume(NOT))
		{
			if(exprUnary())
			{
				return 1;
			}
		}
		if(exprPostfix())
			return 1;
		crtTk=startTk;
		return 0;
	}

	int exprPostfixPrim(){
		printf("@exprPostfixPrim %s\n",codeName(crtTk->code));
		if(consume(LBRACKET))
		{
			if(expr())
			{
				if(consume(RBRACKET))
				{
					if(exprPostfixPrim())
					{
						return 1;
					}
				}
			}
		}
		else
		{
			if(consume(DOT))
			{
				if(consume(ID))
				{
					if(exprPostfixPrim())
					{
						return 1;
					}
				}
			}
		}
		return 1;
	}

	int exprPostfix(){
		Token *startTk=crtTk;
		printf("@exprPostfix %s\n",codeName(crtTk->code));
		if(exprPrimary())
		{
			if(exprPostfixPrim())
			{
				return 1;
			}
		}
		crtTk=startTk;
		return 0;
	}

	int exprPrimary(){
		Token *startTk=crtTk;
		printf("@exprPrimary %s\n",codeName(crtTk->code));
		if(consume(ID))
		{
			if(consume(LPAR))
			{
				expr();
				while(consume(COMMA) && expr());
				if(consume(RPAR))
				{

				}
			}
			return 1;
		}
		if(consume(CT_INT))
			return 1;
		if(consume(CT_REAL))
			return 1;
		if(consume(CT_CHAR))
			return 1;
		if(consume(CT_STRING))
			return 1;
		if(consume(LPAR))
		{
			if(expr())
			{
				if(consume(RPAR))
				{
					return 1;
				}
			}
		}
		crtTk=startTk;
		return 0;
	}



	int main(int argc,char*argv[])
	{	
		if(argc!=2)
		{
			err("Usage:./prog fisier");
		}
		FILE *fd;
		fd=fopen(argv[1],"r");
		if(fd==NULL)
		{
			err("Unable to open the file for reading");
		}
		int nread=fread(buf,1,30000,fd);
		if(fclose(fd)!=0)
		{
			err("Unable to close the file");
		}
		buf[nread]='\0';
		pCrtCh=buf;
		while(getNextToken()!=END);
		// showAtoms();
		crtTk=tokens;
		if(unit())
		{
			printf("Syntax is correct\n");
		}
		else
		{
			tkerr(crtTk,"Incorrect syntax");
		}
		terminare();
		return 0;

	}
