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
	char linie[256],*pCrtCh;
	char* createString(const char *i,char *s){
		int nCh=s-i;
		int j=0;
		char *text;
		if((text=(char*)malloc(nCh+1))==NULL)
		{
			err("Not enough memory");
		}
		while(j!=nCh)
		{
			if((*i)=='\\')
		{
			i++;
			nCh--;
			if((*i)=='\\')
			{
			  i++;
			  if((*i)=='t')
			  	text[j]='\t';
			  else
			  	if((*i)=='r')
				  text[j]='\r';
				else
					text[j]=*i;

			  nCh--;	
				
			}
		}
		else
			{
				text[j]=*i;
				i++;
				j++;
			}
		}
		text[j]='\0';
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
		int state=0,nCh;
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
							state=8;
							// printf("%c\n",ch);
						}
						else
						{
							if(ch>='1' && ch <='9')
							{
								pStartCh=pCrtCh;
								pCrtCh++;
								state=12;
							}
							else
							{
								if(ch=='0')
								{
									pStartCh=pCrtCh;
									pCrtCh++;
									state=14;
								}
								else
								{
									if(ch=='\n'||ch=='\r'||ch=='\t')
									{
										pCrtCh++;
									}
									else
									{
										if(ch=='\\')
										{
											pCrtCh++;
											state=25;
										}
										else
										{
											if(ch==',')
											{
												pCrtCh++;
												state=29;
											}
											else
											{
												if(ch==';')
												{
												pCrtCh++;
												state=30;
												}
												else
												{
													if(ch=='(')
													{
														pCrtCh++;
														state=31;
													}
													else
													{
														if(ch==')')
														{
															pCrtCh++;
															state=32;
														}
														else
														{
															if(ch=='[')
															{
																pCrtCh++;
																state=33;
															}
															else
															{
																if(ch==']')
																{
																	pCrtCh++;
																	state=34;
																}
																else
																{
																	if(ch=='{')
																	{
																		pCrtCh++;
																		state=35;
																	}
																	else
																	{
																		if(ch=='}')
																		{
																			pCrtCh++;
																			state=36;
																		}
																		else
																		{
																			if(ch=='+')
																			{
																				pCrtCh++;
																				state=37;	
																			}
																			else
																			{
																				if(ch=='-')
																				{
																					pCrtCh++;
																					state=38;
																				}
																				else
																				{
																					if(ch=='*')
																					{
																						pCrtCh++;
																						state=39;
																					}
																					else
																					{
																						if(ch=='/')
																						{
																							pCrtCh++;
																							state=40;
																						}
																						else
																						{
																							if(ch=='.')
																							{
																								pCrtCh++;
																								state=41;
																							}
																							else
																							{
																								if(ch=='&')
																								{
																									pCrtCh++;
																									state=42;
																								}
																								else
																								{
																									if(ch=='|')
																									{
																										pCrtCh++;
																										state=44;
																									}
																									else
																									{
																										if(ch=='!')
																										{
																											pCrtCh++;
																											state=46;
																										}
																										else
																										{
																											if(ch=='=')
																											{
																												pCrtCh++;
																												state=49;
																											}
																											else
																											{
																												if(ch=='<')
																												{
																													pCrtCh++;
																													state=52;
																												}
																												else
																												{
																													if(ch=='>')
																													{
																														pCrtCh++;
																														state=55;
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
															printf("ID:%s\n",tk->text);
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
					if(ch!='\'' || ch!='\\')
					{
						pCrtCh++;
						state=6;
					}
				}
				break;
			case 4:
				if(ch=='\\')
					{
						pCrtCh++;
						state=5;
					}
				break;
			case 5:
				if(strchr("abfnrtv'?\"\\0",ch))
				{
					pCrtCh++;
					state=6;
				}
				break;
			case 6:
				if(ch=='\'')
				{
					pCrtCh++;
					state=7;
				}
				break;
			case 7:
				tk=addTk(CT_CHAR);
				if(pStartCh[1]=='\\' && pStartCh[2]=='\\')
				{
					if(pStartCh[3]=='n')
					tk->c='\n';
					else
						if(pStartCh[3]=='r')
							tk->c='\r';
						else
							if(pStartCh[3]=='t')
							tk->c='\t';
							else
							tk->c=pStartCh[3];
						
				}
				else
				{
					tk->c=pStartCh[3];
				}
				
				printf("CT_CHAR:%c\n",tk->c);
				return tk->code;
			case 8:
				if(ch=='\\')
					{
						pCrtCh++;
						state=9;
					}
					else
					{
						if(ch=='\"')
						{
							pCrtCh++;
							state=11;
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
			case 9:
				if(ch=='\\')
					{
						pCrtCh++;
						state=10;
					}
				break;
			case 10:
				if(strchr("abfnrtv'?\"\\0",ch))
				{
					pCrtCh++;
				}
				else{
					state=8;
				}
				break;
			case 11:
				tk=addTk(CT_STRING);
				tk->text=createString(pStartCh,pCrtCh);
				printf("CT_STRING:%s\n",tk->text);
				return tk->code;
			case 12:
				if(isdigit(ch))
				{
					pCrtCh++;
				}
				else{
					if(tolower(ch)=='e')
					{
						pCrtCh++;
						state=22;
					}
					else
					{
						if(ch=='.')
						{
							pCrtCh++;
							state=19;
						}
						else
						{
							state=13;
						}
						
						
					}
				}
				break;
			case 13:
				tk=addTk(CT_INT);
				tk->i=convertToInt(pStartCh,pCrtCh);
				printf("CT_INT:%li\n",tk->i);
				return tk->code;
			case 14:
				if(ch>='0' && ch<='7')
				{
					pCrtCh++;
					state=15;
				}
				else
				{
					if(ch=='x')
					{
						pCrtCh++;
						state=16;
					}
					else
					{
						if(ch=='.')
						{
							pCrtCh++;
							state=19;
						}
					}
					
				}
				
				break;
			case 15:
				if(ch>='0' && ch<='7')
				{
					pCrtCh++;
				}
				else
				{
					if(ch=='8' || ch=='9')
					{
						state=18;
						pCrtCh++;
					}
					else
					{
						if(ch=='.')
						{
							pCrtCh++;
							state=19;
						}
						else
						{
							if(tolower(ch)=='e')
							{
								pCrtCh++;
								state=22;
							}
							else
							{
								state=13;
							}
							
							
						}
						
					}
				}
				break;
			case 16:
				if(isdigit(ch)||(tolower(ch)>='a' && tolower(ch)<='f'))
				{
					pCrtCh++;
					state=17;
				}
				break;	
			case 17:
				if(isdigit(ch)||(tolower(ch)>='a' && tolower(ch)<='f'))
				{
					pCrtCh++;
				}
				else
				{
					state=13;
				}
				break;
			case 18:
				if(isdigit(ch))
				{
					pCrtCh++;
				}
				else
				{
					if(ch=='.')
					{
						state=19;
						pCrtCh++;
					}
				}
				break;
			case 19:
				if(isdigit(ch))
				{
					state=20;
					pCrtCh++;
				}
				break;
			case 20:
				if(isdigit(ch))
				{
					pCrtCh++;
				}
				else
				{
					if(tolower(ch)=='e')
					{
						state=22;
						pCrtCh++;
					}
					else
					{
						state=21;
					}
					
				}
				break;
			case 21:
				tk=addTk(CT_REAL);
				tk->r=convertToDouble(pStartCh,pCrtCh);
				printf("CT_REAL:%lf\n",tk->r);
				return tk->code;
			case 22:
				if(ch=='+'||ch=='-')
				{
					state=23;
					pCrtCh++;
				}
				else
				{
					state=23;
				}
			break;
			case 23:
				if(isdigit(ch))
				{
					state=24;
					pCrtCh++;
				}
				break;
			case 24:
				if(isdigit(ch))
				{
					pCrtCh++;
				}
				else
				{
					state=21;
				}
			break;
			case 25:
				if(ch=='\\')
				{
					state=26;
					pCrtCh++;
				}
				else
				{
					if(ch=='*')
					{
						state=27;
						pCrtCh++;
					}
				}
				break;
			case 26:
				if(ch!='\n' && ch!='\r' && ch!='\0')
				{
					state=0;
					pCrtCh++;
				}
				break;
			case 27:
				if(ch!='*')
				{
					pCrtCh++;
				}
				else
				{
					if(ch=='*')
					{
						pCrtCh++;
						state=28;
					}
				}
				break;
			case 28:
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
							state=27;
							pCrtCh++;
						}
					}
					
				}
				break;
		
			case 29:
				addTk(COMMA);
				return COMMA;
			case 30:
				addTk(SEMICOLON);
				return SEMICOLON;
			case 31:
				addTk(LPAR);
				return LPAR;
			case 32:
				addTk(RPAR);
				return RPAR;
			case 33:
				addTk(LBRACKET);
				return LBRACKET;
			case 34:
				addTk(RBRACKET);
				return RBRACKET;
			case 35:
				addTk(LACC);
				return LACC;
			case 36:
				addTk(RACC);
				return RACC;
			case 37:
				addTk(ADD);
				return ADD;
			case 38:
				addTk(SUB);
				return SUB;
			case 39:
				addTk(MUL);
				return MUL;
			case 40:
				addTk(DIV);
				return DIV;
			case 41:
				addTk(DOT);
				return DOT;
			case 42:
				if(ch=='&')
				{
					pCrtCh++;
					state=43;
				}
				break;
			case 43:
				addTk(AND);
				return AND;
			case 44:
				if(ch=='|')
				{
					pCrtCh++;
					state=45;
				}
				break;
			case 45:
				addTk(OR);
				return OR;
			case 46:
				if(ch=='=')
				{
					pCrtCh++;
					state=47;
				}
				else
				{
					state=48;
				}
				break;
			case 47:
				addTk(NOTEQ);
				return NOTEQ;
			case 48:
				addTk(NOT);
				return NOT;
			case 49:
				if(ch=='=')
				{
					pCrtCh++;
					state=50;
				}
				else
				{
					state=51;
				}
				break;
			case 50:
				addTk(EQUAL);
				return EQUAL;
			case 51:
				addTk(ASSIGN);
				return ASSIGN;
			case 52:
				if(ch=='=')
				{
					pCrtCh++;
					state=53;
				}
				else
				{
					state=54;
				}
				break;
			case 53:
				addTk(LESSEQ);
				return LESSEQ;
			case 54:
				addTk(LESS);
				return LESS;
			case 55:
				if(ch=='=')
				{
					pCrtCh++;
					state=56;
				}
				else
				{
					state=57;
				}
				break;
			case 56:
				addTk(GREATEREQ);
				return GREATEREQ;
			case 57:
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
			free(t);
			tokens=tokens->next;
		}
		tokens=NULL;
		lastToken=NULL;
	}
	int main(int argc,char*argv[])
	{	
		if(argc!=2)
		{
			err("Usage:./prog fisier");
		}
		FILE *fd;
		fd=fopen(argv[1],"r+");
		if(fd==NULL)
		{
			err("Unable to open the file for reading");
		}
		while(fgets(linie,256,fd)){
			linie[strlen(linie)-1]='\0';
			// printf("%s ",linie);
			pCrtCh=linie;
			line++;
			getNextToken();
			strcpy(linie,"");
			// printf("%s\n",linie);
		}
		if(fclose(fd)!=0)
		{
			err("Unable to close the file");
		}
		terminare();


	}
