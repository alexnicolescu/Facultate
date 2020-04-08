#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "alex.h"

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
			if(declVar()){}
			else
				if(declStruct()){}
				else
					if(declFunc()){}
					else
						break;
		}
		if(consume(END))
			return 1;
		else
			tkerr(crtTk,"Syntax error");
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
						else
							tkerr(crtTk,"Missing ; after }");
						
					}
					else
						tkerr(crtTk,"Invalid declaration of the struct or missing }");
					
				}
				else
					tkerr(crtTk,"Missing { afer struct name");
				
			}
			else
				tkerr(crtTk,"Missing name after struct keyword");
		}
		crtTk=startTk;
		return 0;
	}

	int declVar(){
		Token *startTk=crtTk;
		int isDeclVar;
		printf("@declVar %s\n",codeName(crtTk->code));
		if(typeBase())
		{
			if(consume(ID))
			{
				isDeclVar=arrayDecl();
				while(1)
				{
					if(consume(COMMA))
					{
						isDeclVar=1;
						if(consume(ID))
						{
							arrayDecl();
						}
						else
							tkerr(crtTk,"Missing name of the variable after ,");
						
					}
					else
						break;
					
				}
				if(consume(SEMICOLON))
				{
					return 1;
				}
				else
				{
					if(isDeclVar)
						tkerr(crtTk,"Missing ; after the declaration of the variable");
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
		else
		if(consume(DOUBLE))
			return 1;
		else
		if(consume(CHAR))
			return 1;
		else
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
			else
				tkerr(crtTk,"Invalid expression or missing ]");
			
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
		int ok=0,isDeclFunc=0;
		Token *tkopr;
		if(typeBase())
		{	
			ok=1;
			isDeclFunc=consume(MUL);
			tkopr=consumedTk;
		}
		else
		{
			if(consume(VOID))
			{
				ok=1;
				isDeclFunc=1;
				tkopr=consumedTk;
			}
		
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
								tkerr(crtTk,"Missing function argument after ,");
						}
						else break;
					}
					if(consume(RPAR))
					{
						if(stmCompound())
							return 1;
						else
							tkerr(crtTk,"Invalid function argument or missing the body of the function");
						
					}
					else
						tkerr(crtTk,"Invalid function argument or missing )");
				}
			}
			else
			{
				if(isDeclFunc)
					tkerr(crtTk,"Missing name of the function after %s",codeName(tkopr->code));
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
			else
				tkerr(crtTk,"Missing name of the argument");
		}
		crtTk=startTk;
		return 0;
	}

	int ifStm(){
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
									if(stm()){}
									else
										tkerr(crtTk,"Invalid expression in the body of else or missing its body");
								}
								return 1;
							}
							else
								tkerr(crtTk,"Invalid condition in the body of if or missing its body");
						}
						else
							tkerr(crtTk,"Invalid condition or missing )");
					}
					else
						tkerr(crtTk,"Invalid expression after (");
				}
				else
					tkerr(crtTk,"Missing ( after if");
			}
		return 0;

	}

	int whileStm(){
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
							else
								tkerr(crtTk,"There's an error in the body of while or its body is missing");
						}
						else
							tkerr(crtTk,"Invalid condition in while or ) is missing");
					}
					else
						tkerr(crtTk,"Invalid condition for while");
				}
				else
					tkerr(crtTk,"Missing ( after while");
			}
		return 0;	
	}

	int forStm(){
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
									else
										tkerr(crtTk,"There's an error in the body of for or its body is missing");
								}
								else
									tkerr(crtTk,"Invalid expression in for or missing a )");
							}
							else
								tkerr(crtTk,"Invalid for initialization or missing a ;");
						}
						else
							tkerr(crtTk,"Invalid for initialization or missing a ;");
					}
					else
						tkerr(crtTk,"Missing ( after for");
			}
		return 0;
	}

	int breakStm(){
		if(consume(BREAK))
			{
				if(consume(SEMICOLON))
					{
						return 1;
					}
				else
					tkerr(crtTk,"Missing ; after break");
			}
		return 0;
	}

	int returnStm(){
		if(consume(RETURN))
			{
				expr();
				if(consume(SEMICOLON))
					{
						return 1;
					}
				else
					tkerr(crtTk,"Invalid expression or missing ;");
			}
		return 0;
	}

	int stm(){
		Token *startTk=crtTk;
		printf("@stm %s\n",codeName(crtTk->code));
		if(stmCompound())
			return 1;
		else
		if(ifStm())
			return 1;
		else
		if(whileStm())
			return 1;
		else
		if(forStm())
			return 1;
		else
		if(breakStm())
			return 1;
		else
		if(returnStm())
			return 1;
		else
			{	int isExpr;
				isExpr=expr();
				if(consume(SEMICOLON))
					{
						return 1;
					}
				else
				{
					if(isExpr)
						tkerr(crtTk,"Invalid expression or missing ;");
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
			else
				tkerr(crtTk,"Syntax error or missing a }");
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

	int exprAssign(){
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
				else
					tkerr(crtTk,"Invalid expression after =");
			}
			
			crtTk=startTk;
		}
		
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
			else
				tkerr(crtTk,"Invalid expression after ||");
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
			else
				tkerr(crtTk,"Invalid expression after &&");
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
			Token *tkopr = consumedTk;
			if(exprRel())
			{
				if(exprEqPrim())
				{
					return 1;
				}
			}
			else
				tkerr(crtTk,"Invalid expression after %s",codeName(tkopr->code));
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
			Token *tkopr = consumedTk;
			if(exprAdd())
			{
				if(exprRelPrim())
				{
					return 1;
				}
			}
			else
				tkerr(crtTk,"Invalid expression after %s",codeName(tkopr->code));
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
			Token *tkopr = consumedTk;
			if(exprMul())
			{
				if(exprAddPrim())
				{
					return 1;
				}
			}
			else
				tkerr(crtTk,"Invalid expression after %s",codeName(tkopr->code));
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
			Token *tkopr = consumedTk;
			if(exprCast())
			{
				if(exprMulPrim())
				{
					return 1;
				}
			}
			else
				tkerr(crtTk,"Invalid expression after %s",codeName(tkopr->code));
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
					else
						tkerr(crtTk,"Invalid cast expression");
				}
				else
					tkerr(crtTk,"Missing a )");
			}
			else
				tkerr(crtTk,"Invalid type name");
			crtTk=startTk;
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
			Token *tkopr = consumedTk;
			if(exprUnary())
			{
				return 1;
			}
			else
				tkerr(crtTk,"Invalid unary expression after %s",codeName(tkopr->code));
		}
		else
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
				else
					tkerr(crtTk,"Invalid expression or missing ]");
			}
			else
				tkerr(crtTk,"Invalid expression after [");
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
				else
					tkerr(crtTk,"Missing field name after .");
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
				while(1)
				{
					if(consume(COMMA))
					{
						if(expr()){}
						else
							tkerr(crtTk,"Invalid expression after ,");
					}
					else
						break;
				}
				if(consume(RPAR)){}
				else
					tkerr(crtTk,"Invalid expression or missing a )");
			}
			return 1;
		}
		else
		if(consume(CT_INT))
			return 1;
		else
		if(consume(CT_REAL))
			return 1;
		else
		if(consume(CT_CHAR))
			return 1;
		else
		if(consume(CT_STRING))
			return 1;
		else
		if(consume(LPAR))
		{
			if(expr())
			{
				if(consume(RPAR))
				{
					return 1;
				}
				else
					tkerr(crtTk,"Invalid expression or missing a )");
				
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
		read_file(argv[1]);
		crtTk=getTokens();
		if(unit())
		{
			printf("Syntax is correct\n");
		}
		
		terminare();
		return 0;

	}
