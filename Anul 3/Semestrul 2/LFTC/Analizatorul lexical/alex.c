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
		char c;//folosit pt CT_CHAR
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
	if(text[j]!='\"')
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
int getNextToken(){
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
						tk->c='\a';
						break;
						case 'b':
						tk->c='\b';
						break;
						case 'f':
						tk->c='\f';
						break;
						case 'n':
						tk->c='\n';
						break;
						case 'r':
						tk->c='\r';
						break;
						case 't':
						tk->c='\t';
						break;
						case 'v':
						tk->c='\v';
						break;
						case '\'':
						tk->c='\'';
						break;
						case '?':
						tk->c='\?';
						break;
						case '\"':
						tk->c='\"';
						break;
						case '\\':
						tk->c='\\';
						break;
						case '\0':
						tk->c='\0';
						break;
					}
				}
				else
				{
					tk->c=pStartCh[1];
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
				printf(" CT_CHAR:%c",token->c);
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
		showAtoms();
		terminare();
		return 0;

	}
