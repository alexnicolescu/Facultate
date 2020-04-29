#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "alex.h"
#include "adom.h"

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

Token *consumedTk, *crtTk;
Symbol *crtFunc = NULL, *crtStruct = NULL;

int consume(int code)
{
	printf("Consume(%s)", codeName(code));
	if (crtTk->code == code)
	{
		consumedTk = crtTk;
		crtTk = crtTk->next;
		printf("=>consumed\n");
		return 1;
	}
	printf("=>something else(%s)\n", codeName(crtTk->code));
	return 0;
}

int unit()
{
	Token *startTk = crtTk;
	printf("@unit %s\n", codeName(crtTk->code));
	while (1)
	{
		if (declStruct())
		{
		}
		else if (declFunc())
		{
		}
		else if (declVar())
		{
		}
		else
			break;
	}
	if (consume(END))
		return 1;
	else
		tkerr(crtTk, "Syntax error");
	crtTk = startTk;
	return 0;
}

void declStructDom(Token *tkName)
{
	if (findSymbol(&symbols, tkName->text))
		tkerr(crtTk, "Symbol redefinition: %s", tkName->text);
	crtStruct = addSymbol(&symbols, tkName->text, CLS_STRUCT);
	initSymbols(&crtStruct->members);
}

int declStruct()
{
	Token *startTk = crtTk;
	printf("@declStruct %s\n", codeName(crtTk->code));
	Token *tkName;
	if (consume(STRUCT))
	{
		if (consume(ID))
		{
			tkName = consumedTk;
			if (consume(LACC))
			{
				declStructDom(tkName);
				while (declVar())
					;
				if (consume(RACC))
				{
					if (consume(SEMICOLON))
					{
						crtStruct = NULL;
						return 1;
					}
					else
						tkerr(crtTk, "Missing ; after }");
				}
				else
					tkerr(crtTk, "Invalid declaration of the struct or missing }");
			}
			// else
			// tkerr(crtTk, "Missing { afer struct name");
		}
		else
			tkerr(crtTk, "Missing name after struct keyword");
	}
	crtTk = startTk;
	return 0;
}

int declVar()
{
	Token *startTk = crtTk;
	int isDeclVar;
	printf("@declVar %s\n", codeName(crtTk->code));
	Type t;
	Token *tkName;
	if (typeBase(&t))
	{
		if (consume(ID))
		{
			tkName = consumedTk;
			isDeclVar = arrayDecl(&t);
			if (!isDeclVar)
			{
				t.nElements = -1;
			}
			addVar(tkName, &t);
			while (1)
			{
				if (consume(COMMA))
				{
					isDeclVar = 1;
					if (consume(ID))
					{
						tkName = consumedTk;
						if (arrayDecl(&t))
						{
						}
						else
							t.nElements = -1;
						addVar(tkName, &t);
					}
					else
						tkerr(crtTk, "Missing name of the variable after ,");
				}
				else
					break;
			}
			if (consume(SEMICOLON))
			{
				return 1;
			}
			else
			{
				if (isDeclVar)
					tkerr(crtTk, "Missing ; after the declaration of the variable");
			}
		}
	}
	crtTk = startTk;
	return 0;
}

void typeBaseDom(Token *tkName, Type *ret)
{
	Symbol *s = findSymbol(&symbols, tkName->text);
	if (s == NULL)
		tkerr(crtTk, "Undefined symbol: %s", tkName->text);
	if (s->cls != CLS_STRUCT)
		tkerr(crtTk, "%s is not a struct", tkName->text);
	ret->typeBase = TB_STRUCT;
	ret->s = s;
}

int typeBase(Type *ret)
{
	Token *startTk = crtTk;
	printf("@typeBase %s\n", codeName(crtTk->code));
	Token *tkName;
	if (consume(INT))
	{
		ret->typeBase = TB_INT;
		return 1;
	}
	else if (consume(DOUBLE))
	{
		ret->typeBase = TB_DOUBLE;
		return 1;
	}
	else if (consume(CHAR))
	{
		ret->typeBase = TB_CHAR;
		return 1;
	}
	else if (consume(STRUCT))
	{
		if (consume(ID))
		{
			tkName = consumedTk;
			typeBaseDom(tkName, ret);
			return 1;
		}
		else
			tkerr(crtTk, "Missing name after struct");
	}
	crtTk = startTk;
	return 0;
}

int arrayDecl(Type *ret)
{
	Token *startTk = crtTk;
	printf("@arrayDecl %s\n", codeName(crtTk->code));
	if (consume(LBRACKET))
	{
		RetVal rv;
		if (expr(&rv))
		{
			if (!rv.isCtVal)
			{
				tkerr(crtTk, "The array size is not a constant");
			}
			if (rv.type.typeBase != TB_INT)
			{
				tkerr(crtTk, "The array size is not an integer");
			}
			ret->nElements = rv.ctVal.i;
		}
		else
		{
			ret->nElements = 0;
		}
		if (consume(RBRACKET))
			return 1;
		else
			tkerr(crtTk, "Invalid expression or missing ]");
	}
	crtTk = startTk;
	return 0;
}

int typeName(Type *ret)
{
	Token *startTk = crtTk;
	printf("@typeName %s\n", codeName(crtTk->code));
	if (typeBase(ret))
	{
		if (arrayDecl(ret))
		{
		}
		else
			ret->nElements = -1;
		return 1;
	}
	crtTk = startTk;
	return 0;
}

void declFuncDom(Token *tkName, Type t)
{
	if (findSymbol(&symbols, tkName->text))
		tkerr(crtTk, "Symbol redefinition: %s", tkName->text);
	crtFunc = addSymbol(&symbols, tkName->text, CLS_FUNC);
	initSymbols(&crtFunc->args);
	crtFunc->type = t;
	crtDepth++;
}

int declFunc()
{
	Token *startTk = crtTk;
	printf("@declFunc %s\n", codeName(crtTk->code));
	int ok = 0, isDeclFunc = 0;
	Token *tkopr, *tkName;
	Type t;
	if (typeBase(&t))
	{
		ok = 1;
		isDeclFunc = consume(MUL);
		if (!isDeclFunc)
			t.nElements = -1;
		else
			t.nElements = 0;
		tkopr = consumedTk;
	}
	else
	{
		if (consume(VOID))
		{
			ok = 1;
			isDeclFunc = 1;
			tkopr = consumedTk;
			t.typeBase = TB_VOID;
		}
	}
	if (ok == 1)
	{
		if (consume(ID))
		{
			tkName = consumedTk;
			if (consume(LPAR))
			{
				declFuncDom(tkName, t);
				funcArg();
				while (1)
				{
					if (consume(COMMA))
					{
						if (funcArg())
						{
						}
						else
							tkerr(crtTk, "Missing function argument after ,");
					}
					else
						break;
				}
				if (consume(RPAR))
				{
					crtDepth--;
					if (stmCompound())
					{
						deleteSymbolsAfter(&symbols, crtFunc);
						crtFunc = NULL;
						return 1;
					}
					else
						tkerr(crtTk, "Invalid function argument or missing the body of the function");
				}
				else
					tkerr(crtTk, "Invalid function argument or missing )");
			}
		}
		else
		{
			if (isDeclFunc)
				tkerr(crtTk, "Missing name of the function after %s", codeName(tkopr->code));
		}
	}
	crtTk = startTk;
	return 0;
}

void funcArgDom(Token *tkName, Type t)
{
	Symbol *s = addSymbol(&symbols, tkName->text, CLS_VAR);
	s->mem = MEM_ARG;
	s->type = t;
	s = addSymbol(&crtFunc->args, tkName->text, CLS_VAR);
	s->mem = MEM_ARG;
	s->type = t;
}

int funcArg()
{
	Token *startTk = crtTk;
	printf("@funcArg %s\n", codeName(crtTk->code));
	Type t;
	Token *tkName;
	if (typeBase(&t))
	{
		if (consume(ID))
		{
			tkName = consumedTk;
			if (arrayDecl(&t))
			{
			}
			else
				t.nElements = -1;
			funcArgDom(tkName, t);
			return 1;
		}
		else
			tkerr(crtTk, "Missing the name of the argument");
	}
	crtTk = startTk;
	return 0;
}

int ifStm()
{
	if (consume(IF))
	{
		if (consume(LPAR))
		{
			RetVal rv;
			if (expr(&rv))
			{
				if (rv.type.typeBase == TB_STRUCT)
				{
					tkerr(crtTk, "A structure cannot be logically tested");
				}
				if (consume(RPAR))
				{
					if (stm())
					{
						if (consume(ELSE))
						{
							if (stm())
							{
							}
							else
								tkerr(crtTk, "Invalid expression in the body of else or missing its body");
						}
						return 1;
					}
					else
						tkerr(crtTk, "Invalid condition in the body of if or missing its body");
				}
				else
					tkerr(crtTk, "Invalid condition or missing )");
			}
			else
				tkerr(crtTk, "Invalid expression after (");
		}
		else
			tkerr(crtTk, "Missing ( after if");
	}
	return 0;
}

int whileStm()
{
	if (consume(WHILE))
	{
		if (consume(LPAR))
		{
			RetVal rv;
			if (expr(&rv))
			{
				if (rv.type.typeBase == TB_STRUCT)
				{
					tkerr(crtTk, "A structure cannot be logically tested");
				}
				if (consume(RPAR))
				{
					if (stm())
					{
						return 1;
					}
					else
						tkerr(crtTk, "There's an error in the body of while or its body is missing");
				}
				else
					tkerr(crtTk, "Invalid condition in while or ) is missing");
			}
			else
				tkerr(crtTk, "Invalid condition for while");
		}
		else
			tkerr(crtTk, "Missing ( after while");
	}
	return 0;
}

int forStm()
{
	if (consume(FOR))
	{
		if (consume(LPAR))
		{
			RetVal rv1;
			expr(&rv1);
			if (consume(SEMICOLON))
			{
				RetVal rv2;
				if (expr(&rv2))
				{
					if (rv2.type.typeBase == TB_STRUCT)
					{
						tkerr(crtTk, "A structure cannot be logically tested");
					}
				}
				if (consume(SEMICOLON))
				{
					RetVal rv3;
					expr(&rv3);
					if (consume(RPAR))
					{
						if (stm())
						{
							return 1;
						}
						else
							tkerr(crtTk, "There's an error in the body of for or its body is missing");
					}
					else
						tkerr(crtTk, "Invalid expression in for or missing a )");
				}
				else
					tkerr(crtTk, "Invalid for initialization or missing a ;");
			}
			else
				tkerr(crtTk, "Invalid for initialization or missing a ;");
		}
		else
			tkerr(crtTk, "Missing ( after for");
	}
	return 0;
}

int breakStm()
{
	if (consume(BREAK))
	{
		if (consume(SEMICOLON))
		{
			return 1;
		}
		else
			tkerr(crtTk, "Missing ; after break");
	}
	return 0;
}

int returnStm()
{
	if (consume(RETURN))
	{
		RetVal rv;
		if (expr(&rv))
		{
			if (rv.type.typeBase == TB_VOID)
			{
				tkerr(crtTk, "A void function cannot return a value");
			}
			cast(&crtFunc->type, &rv.type);
		}
		if (consume(SEMICOLON))
		{
			return 1;
		}
		else
			tkerr(crtTk, "Invalid expression or missing ;");
	}
	return 0;
}

int stm()
{
	Token *startTk = crtTk;
	printf("@stm %s\n", codeName(crtTk->code));
	if (stmCompound())
		return 1;
	else if (ifStm())
		return 1;
	else if (whileStm())
		return 1;
	else if (forStm())
		return 1;
	else if (breakStm())
		return 1;
	else if (returnStm())
		return 1;
	else
	{
		int isExpr;
		RetVal rv;
		isExpr = expr(&rv);
		if (consume(SEMICOLON))
		{
			return 1;
		}
		else
		{
			if (isExpr)
				tkerr(crtTk, "Invalid expression or missing ;");
		}
	}
	crtTk = startTk;
	return 0;
}

int stmCompound()
{
	Token *startTk = crtTk;
	printf("@stmCompound %s\n", codeName(crtTk->code));
	Symbol *start = symbols.end[-1];
	if (consume(LACC))
	{
		crtDepth++;
		while (declVar() || stm())
			;
		if (consume(RACC))
		{
			crtDepth--;
			deleteSymbolsAfter(&symbols, start);
			return 1;
		}
		else
			tkerr(crtTk, "Syntax error or missing a }");
	}
	crtTk = startTk;
	return 0;
}

int expr(RetVal *rv)
{
	Token *startTk = crtTk;
	printf("@expr %s\n", codeName(crtTk->code));
	if (exprAssign(rv))
		return 1;
	crtTk = startTk;
	return 0;
}

int exprAssign(RetVal *rv)
{
	Token *startTk = crtTk;
	printf("@exprAssign %s\n", codeName(crtTk->code));
	if (exprUnary(rv))
	{
		if (consume(ASSIGN))
		{
			RetVal rve;
			if (exprAssign(&rve))
			{
				if (!rv->isLVal)
				{
					tkerr(crtTk, "Cannot assign to a non-lval");
				}
				if (rv->type.nElements > -1 || rve.type.nElements > -1)
				{
					tkerr(crtTk, "The arrays cannot be assigned");
				}
				cast(&rv->type, &rve.type);
				rv->isCtVal = rv->isLVal = 0;

				return 1;
			}
			else
				tkerr(crtTk, "Invalid expression after =");
		}

		crtTk = startTk;
	}

	if (exprOr(rv))
	{
		return 1;
	}
	crtTk = startTk;
	return 0;
}

int exprOrPrim(RetVal *rv)
{
	printf("@exprOrPrim %s\n", codeName(crtTk->code));
	if (consume(OR))
	{
		RetVal rve;
		if (exprAnd(&rve))
		{
			if (rv->type.typeBase == TB_STRUCT || rve.type.typeBase == TB_STRUCT)
			{
				tkerr(crtTk, "A structure cannot be logically tested");
			}
			rv->type = createType(TB_INT, -1);
			rv->isCtVal = rv->isLVal = 0;
			if (exprOrPrim(rv))
			{
				return 1;
			}
		}
		else
			tkerr(crtTk, "Invalid expression after ||");
	}
	return 1;
}

int exprOr(RetVal *rv)
{
	Token *startTk = crtTk;
	printf("@exprOr %s\n", codeName(crtTk->code));
	if (exprAnd(rv))
	{
		if (exprOrPrim(rv))
		{
			return 1;
		}
	}
	crtTk = startTk;
	return 0;
}

int exprAndPrim(RetVal *rv)
{
	printf("@exprAndPrim %s\n", codeName(crtTk->code));
	if (consume(AND))
	{
		RetVal rve;
		if (exprEq(&rve))
		{
			if (rv->type.typeBase == TB_STRUCT || rve.type.typeBase == TB_STRUCT)
			{
				tkerr(crtTk, "A structure cannot be logically tested");
			}
			rv->type = createType(TB_INT, -1);
			rv->isCtVal = rv->isLVal = 0;
			if (exprAndPrim(rv))
			{
				return 1;
			}
		}
		else
			tkerr(crtTk, "Invalid expression after &&");
	}
	return 1;
}

int exprAnd(RetVal *rv)
{
	Token *startTk = crtTk;
	printf("@exprAnd %s\n", codeName(crtTk->code));
	if (exprEq(rv))
	{
		if (exprAndPrim(rv))
		{
			return 1;
		}
	}
	crtTk = startTk;
	return 0;
}

int exprEqPrim(RetVal *rv)
{
	printf("@exprEqPrim %s\n", codeName(crtTk->code));
	if (consume(EQUAL) || consume(NOTEQ))
	{
		Token *tkopr = consumedTk;
		RetVal rve;
		if (exprRel(&rve))
		{
			if (rv->type.typeBase == TB_STRUCT || rve.type.typeBase == TB_STRUCT)
				tkerr(crtTk, "A structure cannot be compared");
			rv->type = createType(TB_INT, -1);
			rv->isCtVal = rv->isLVal = 0;
			if (exprEqPrim(rv))
			{
				return 1;
			}
		}
		else
			tkerr(crtTk, "Invalid expression after %s", codeName(tkopr->code));
	}
	return 1;
}

int exprEq(RetVal *rv)
{
	Token *startTk = crtTk;
	printf("@exprEq %s\n", codeName(crtTk->code));
	if (exprRel(rv))
	{
		if (exprEqPrim(rv))
		{
			return 1;
		}
	}
	crtTk = startTk;
	return 0;
}

int exprRelPrim(RetVal *rv)
{
	printf("@exprRelPrim %s\n", codeName(crtTk->code));
	if (consume(LESS) || consume(LESSEQ) || consume(GREATER) || consume(GREATEREQ))
	{
		Token *tkopr = consumedTk;
		RetVal rve;
		if (exprAdd(&rve))
		{
			if (rv->type.nElements > -1 || rve.type.nElements > -1)
				tkerr(crtTk, "An array cannot be compared");
			if (rv->type.typeBase == TB_STRUCT || rve.type.typeBase == TB_STRUCT)
				tkerr(crtTk, "A structure cannot be compared");
			rv->type = createType(TB_INT, -1);
			rv->isCtVal = rv->isLVal = 0;
			if (exprRelPrim(rv))
			{
				return 1;
			}
		}
		else
			tkerr(crtTk, "Invalid expression after %s", codeName(tkopr->code));
	}
	return 1;
}

int exprRel(RetVal *rv)
{
	Token *startTk = crtTk;
	printf("@exprRel %s\n", codeName(crtTk->code));
	if (exprAdd(rv))
	{
		if (exprRelPrim(rv))
		{
			return 1;
		}
	}
	crtTk = startTk;
	return 0;
}

int exprAddPrim(RetVal *rv)
{
	printf("@exprAddPrim %s\n", codeName(crtTk->code));
	if (consume(ADD) || consume(SUB))
	{
		Token *tkopr = consumedTk;
		RetVal rve;
		if (exprMul(&rve))
		{
			if (rv->type.nElements > -1 || rve.type.nElements > -1)
				tkerr(crtTk, "An array cannot be added or subtracted");
			if (rv->type.typeBase == TB_STRUCT || rve.type.typeBase == TB_STRUCT)
				tkerr(crtTk, "A structure cannot be added or subtracted");
			rv->type = getArithType(&rv->type, &rve.type);
			rv->isCtVal = rv->isLVal = 0;
			if (exprAddPrim(rv))
			{
				return 1;
			}
		}
		else
			tkerr(crtTk, "Invalid expression after %s", codeName(tkopr->code));
	}
	return 1;
}

int exprAdd(RetVal *rv)
{
	Token *startTk = crtTk;
	printf("@exprAdd %s\n", codeName(crtTk->code));
	if (exprMul(rv))
	{
		if (exprAddPrim(rv))
		{
			return 1;
		}
	}
	crtTk = startTk;
	return 0;
}

int exprMulPrim(RetVal *rv)
{
	printf("@exprMulPrim %s\n", codeName(crtTk->code));
	if (consume(MUL) || consume(DIV))
	{
		Token *tkopr = consumedTk;
		RetVal rve;
		if (exprCast(&rve))
		{
			if (rv->type.nElements > -1 || rve.type.nElements > -1)
				tkerr(crtTk, "An array cannot be multiplied or divided");
			if (rv->type.typeBase == TB_STRUCT || rve.type.typeBase == TB_STRUCT)
				tkerr(crtTk, "A structure cannot be multiplied or divided");
			rv->type = getArithType(&rv->type, &rve.type);
			rv->isCtVal = rv->isLVal = 0;
			if (exprMulPrim(rv))
			{
				return 1;
			}
		}
		else
			tkerr(crtTk, "Invalid expression after %s", codeName(tkopr->code));
	}
	return 1;
}

int exprMul(RetVal *rv)
{
	Token *startTk = crtTk;
	printf("@exprMul %s\n", codeName(crtTk->code));
	if (exprCast(rv))
	{
		if (exprMulPrim(rv))
		{
			return 1;
		}
	}
	crtTk = startTk;
	return 0;
}

int exprCast(RetVal *rv)
{
	Token *startTk = crtTk;
	printf("@exprCast %s\n", codeName(crtTk->code));
	Type t;
	if (consume(LPAR))
	{
		if (typeName(&t))
		{
			if (consume(RPAR))
			{
				RetVal rve;
				if (exprCast(&rve))
				{
					cast(&t, &rve.type);
					rv->type = t;
					rv->isCtVal = rv->isLVal = 0;
					return 1;
				}
				else
					tkerr(crtTk, "Invalid cast expression");
			}
			else
				tkerr(crtTk, "Missing a )");
		}
		else
			tkerr(crtTk, "Invalid type name");
		crtTk = startTk;
	}
	if (exprUnary(rv))
		return 1;
	crtTk = startTk;
	return 0;
}

int exprUnary(RetVal *rv)
{
	Token *startTk = crtTk;
	printf("@exprUnary %s\n", codeName(crtTk->code));
	if (consume(SUB) || consume(NOT))
	{
		Token *tkopr = consumedTk;
		if (exprUnary(rv))
		{
			if (tkopr->code == SUB)
			{
				if (rv->type.nElements >= 0)
					tkerr(crtTk, "Unary '-' cannot be applied to an array");
				if (rv->type.typeBase == TB_STRUCT)
					tkerr(crtTk, "Unary '-' cannot be applied to a struct");
			}
			else
			{ // NOT
				if (rv->type.typeBase == TB_STRUCT)
					tkerr(crtTk, "'!' cannot be applied to a struct");
				rv->type = createType(TB_INT, -1);
			}
			rv->isCtVal = rv->isLVal = 0;
			return 1;
		}
		else
			tkerr(crtTk, "Invalid unary expression after %s", codeName(tkopr->code));
	}
	else if (exprPostfix(rv))
		return 1;
	crtTk = startTk;
	return 0;
}

int exprPostfixPrim(RetVal *rv)
{
	printf("@exprPostfixPrim %s\n", codeName(crtTk->code));
	if (consume(LBRACKET))
	{
		RetVal rve;
		if (expr(&rve))
		{
			if (rv->type.nElements < 0)
				tkerr(crtTk, "Only an array can be indexed");
			Type typeInt = createType(TB_INT, -1);
			cast(&typeInt, &rve.type);
			rv->type = rv->type;
			rv->type.nElements = -1;
			rv->isLVal = 1;
			rv->isCtVal = 0;
			if (consume(RBRACKET))
			{
				if (exprPostfixPrim(rv))
				{
					return 1;
				}
			}
			else
				tkerr(crtTk, "Invalid expression or missing ]");
		}
		else
			tkerr(crtTk, "Invalid expression after [");
	}
	else
	{
		if (consume(DOT))
		{
			if (consume(ID))
			{
				Token *tkName = consumedTk;
				Symbol *sStruct = rv->type.s;
				Symbol *sMember = findSymbol(&sStruct->members, tkName->text);
				if (!sMember)
					tkerr(crtTk, "Struct %s does not have a member %s", sStruct->name, tkName->text);
				rv->type = sMember->type;
				rv->isLVal = 1;
				rv->isCtVal = 0;
				if (exprPostfixPrim(rv))
				{
					return 1;
				}
			}
			else
				tkerr(crtTk, "Missing field name after .");
		}
	}
	return 1;
}

int exprPostfix(RetVal *rv)
{
	Token *startTk = crtTk;
	printf("@exprPostfix %s\n", codeName(crtTk->code));
	if (exprPrimary(rv))
	{
		if (exprPostfixPrim(rv))
		{
			return 1;
		}
	}
	crtTk = startTk;
	return 0;
}

int exprPrimary(RetVal *rv)
{
	Token *startTk = crtTk;
	printf("@exprPrimary %s\n", codeName(crtTk->code));
	Token *tkName;
	if (consume(ID))
	{
		tkName = consumedTk;
		Symbol *s = findSymbol(&symbols, tkName->text);
		if (!s)
			tkerr(crtTk, "Undefined symbol %s", tkName->text);
		rv->type = s->type;
		rv->isCtVal = 0;
		rv->isLVal = 1;
		if (consume(LPAR))
		{
			Symbol **crtDefArg = s->args.begin;
			if (s->cls != CLS_FUNC && s->cls != CLS_EXTFUNC)
				tkerr(crtTk, "Call of the non-function %s", tkName->text);
			RetVal arg;
			if (expr(&arg))
			{
				if (crtDefArg == s->args.end)
					tkerr(crtTk, "Too many arguments in call");
				cast(&(*crtDefArg)->type, &arg.type);
				crtDefArg++;
			}
			while (1)
			{
				if (consume(COMMA))
				{
					if (expr(&arg))
					{
						if (crtDefArg == s->args.end)
							tkerr(crtTk, "Too many arguments in call");
						cast(&(*crtDefArg)->type, &arg.type);
						crtDefArg++;
					}
					else
					{
						if (s->cls == CLS_FUNC || s->cls == CLS_EXTFUNC)
							tkerr(crtTk, "Missing call for function %s", tkName->text);
						tkerr(crtTk, "Invalid expression after ,");
					}
				}
				else
					break;
			}
			if (consume(RPAR))
			{
				if (crtDefArg != s->args.end)
					tkerr(crtTk, "Too few arguments in call");
				rv->type = s->type;
				rv->isCtVal = rv->isLVal = 0;
			}
			else
				tkerr(crtTk, "Invalid expression or missing a )");
		}
		return 1;
	}
	else if (consume(CT_INT)){
		Token *tki;
		tki=consumedTk;
		rv->type=createType(TB_INT,-1);
		rv->ctVal.i=tki->i;
        rv->isCtVal=1;rv->isLVal=0;
		return 1;
	}
	else if (consume(CT_REAL)){
		Token *tkr;
		tkr=consumedTk;
		rv->type=createType(TB_DOUBLE,-1);
		rv->ctVal.d=tkr->r;
        rv->isCtVal=1;rv->isLVal=0;
		return 1;
	}
	else if (consume(CT_CHAR)){
		Token *tkc;
		tkc=consumedTk;
		rv->type=createType(TB_CHAR,-1);
		rv->ctVal.i=tkc->i;
        rv->isCtVal=1;rv->isLVal=0;
		return 1;
	}
	else if (consume(CT_STRING)){
		Token *tks;
		tks=consumedTk;
		rv->type=createType(TB_CHAR,0);
		rv->ctVal.str=tks->text;
        rv->isCtVal=1;rv->isLVal=0;
		return 1;
	}
	else if (consume(LPAR))
	{
		if (expr(rv))
		{
			if (consume(RPAR))
			{
				return 1;
			}
			else
				tkerr(crtTk, "Invalid expression or missing a )");
		}
	}
	crtTk = startTk;
	return 0;
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		err("Usage:./prog fisier");
	}
	read_file(argv[1]);
	crtTk = getTokens();
	addExtFuncs();
	if (unit())
	{
		printf("Syntax is correct\n");
	}

	terminare();
	return 0;
}
