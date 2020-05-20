#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

// #include "alex.h"
// #include "adom.h"
// #include "mv.h"

#include "main.h"

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
int sizeArgs, offset;
Instr *crtLoopEnd;

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
	printf("@unit %s\n", codeName(crtTk->code));
	Instr *labelMain = addInstr(O_CALL);
	addInstr(O_HALT);
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
	labelMain->args[0].addr = requireSymbol(&symbols, "main")->addr;
	if (consume(END))
		return 1;
	else
		tkerr(crtTk, "Syntax error");
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
				offset = 0;
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
	// Token *startTk = crtTk;
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
	// crtTk = startTk;
	return 0;
}

int arrayDecl(Type *ret)
{
	// Token *startTk = crtTk;
	Instr *instrBeforeExpr;
	printf("@arrayDecl %s\n", codeName(crtTk->code));
	if (consume(LBRACKET))
	{
		RetVal rv;
		instrBeforeExpr = lastInstruction;
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
			deleteInstructionsAfter(instrBeforeExpr);
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
	// crtTk = startTk;
	return 0;
}

int typeName(Type *ret)
{
	// Token *startTk = crtTk;
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
	// crtTk = startTk;
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
	Symbol **ps;
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
			sizeArgs = offset = 0;
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
					crtFunc->addr = addInstr(O_ENTER);
					sizeArgs = offset;
					//update args offsets for correct FP indexing
					for (ps = symbols.begin; ps != symbols.end; ps++)
					{
						if ((*ps)->mem == MEM_ARG)
						{
							//2*sizeof(void*) == sizeof(retAddr)+sizeof(FP)
							(*ps)->offset -= sizeArgs + 2 * sizeof(void *);
						}
					}
					offset = 0;
					if (stmCompound())
					{
						deleteSymbolsAfter(&symbols, crtFunc);
						((Instr *)crtFunc->addr)->args[0].i = offset; // setup the ENTER argument
						if (crtFunc->type.typeBase == TB_VOID)
						{
							addInstrII(O_RET, sizeArgs, 0);
						}
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
	s->offset = offset;
	s = addSymbol(&crtFunc->args, tkName->text, CLS_VAR);
	s->mem = MEM_ARG;
	s->type = t;
	s->offset = offset;
	offset += typeArgSize(&s->type);
}

int funcArg()
{
	// Token *startTk = crtTk;
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
	// crtTk = startTk;
	return 0;
}

int ifStm()
{
	Instr *i1, *i2;
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
					i1 = createCondJmp(&rv);
					if (stm())
					{
						if (consume(ELSE))
						{
							i2 = addInstr(O_JMP);
							if (stm())
							{
								i1->args[0].addr = i2->next;
								i1 = i2;
							}
							else
								tkerr(crtTk, "Invalid expression in the body of else or missing its body");
						}
						i1->args[0].addr = addInstr(O_NOP);
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
	Instr *i1, *i2;
	if (consume(WHILE))
	{
		Instr *oldLoopEnd = crtLoopEnd;
		crtLoopEnd = createInstr(O_NOP);
		i1 = lastInstruction;
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
					i2 = createCondJmp(&rv);
					if (stm())
					{
						addInstrA(O_JMP, i1->next);
						appendInstr(crtLoopEnd);
						i2->args[0].addr = crtLoopEnd;
						crtLoopEnd = oldLoopEnd;
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
	Instr *i2, *i3, *i4, *ib3, *ibs, *is;
	if (consume(FOR))
	{
		Instr *oldLoopEnd = crtLoopEnd;
		crtLoopEnd = createInstr(O_NOP);
		if (consume(LPAR))
		{
			RetVal rv1;
			if (expr(&rv1))
			{
				if (typeArgSize(&rv1.type))
					addInstrI(O_DROP, typeArgSize(&rv1.type));
			}
			if (consume(SEMICOLON))
			{
				i2 = lastInstruction;
				RetVal rv2;
				if (expr(&rv2))
				{
					i4 = createCondJmp(&rv2);
					if (rv2.type.typeBase == TB_STRUCT)
					{
						tkerr(crtTk, "A structure cannot be logically tested");
					}
				}
				else
				{
					i4 = NULL;
				}
				if (consume(SEMICOLON))
				{
					ib3 = lastInstruction;
					RetVal rv3;
					if (expr(&rv3))
					{
						if (typeArgSize(&rv3.type))
							addInstrI(O_DROP, typeArgSize(&rv3.type));
					}
					if (consume(RPAR))
					{
						ibs = lastInstruction; /* ibs is before stm */
						if (stm())
						{
							if (ib3 != ibs)
							{
								i3 = ib3->next;
								is = ibs->next;
								ib3->next = is;
								is->last = ib3;
								lastInstruction->next = i3;
								i3->last = lastInstruction;
								ibs->next = NULL;
								lastInstruction = ibs;
							}
							addInstrA(O_JMP, i2->next);
							printf("\n%d\n",crtLoopEnd->opcode);
							appendInstr(crtLoopEnd);
							if (i4)
								i4->args[0].addr = crtLoopEnd;
							crtLoopEnd = oldLoopEnd;
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
			if (!crtLoopEnd)
				tkerr(crtTk, "break without for or while");
			addInstrA(O_JMP, crtLoopEnd);
			return 1;
		}
		else
			tkerr(crtTk, "Missing ; after break");
	}
	return 0;
}

int returnStm()
{
	Instr *i;
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
			i = getRVal(&rv);
			addCastInstr(i, &rv.type, &crtFunc->type);
		}
		if (consume(SEMICOLON))
		{
			if (crtFunc->type.typeBase == TB_VOID)
			{
				addInstrII(O_RET, sizeArgs, 0);
			}
			else
			{
				addInstrII(O_RET, sizeArgs, typeArgSize(&crtFunc->type));
			}
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
		if (isExpr)
		{
			if (typeArgSize(&rv.type))
				addInstrI(O_DROP, typeArgSize(&rv.type));
		}
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
	// Token *startTk = crtTk;
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
	// crtTk = startTk;
	return 0;
}

int expr(RetVal *rv)
{
	// Token *startTk = crtTk;
	printf("@expr %s\n", codeName(crtTk->code));
	if (exprAssign(rv))
		return 1;
	// crtTk = startTk;
	return 0;
}

int exprAssign(RetVal *rv)
{
	Token *startTk = crtTk;
	printf("@exprAssign %s\n", codeName(crtTk->code));
	Instr *oldLastInstr = lastInstruction, *i;
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
				i = getRVal(&rve);
				addCastInstr(i, &rve.type, &rv->type);
				addInstrII(O_INSERT,
						   sizeof(void *) + typeArgSize(&rv->type),
						   typeArgSize(&rv->type));
				addInstrI(O_STORE, typeArgSize(&rv->type));
				rv->isCtVal = rv->isLVal = 0;
				return 1;
			}
			else
				tkerr(crtTk, "Invalid expression after =");
		}

		crtTk = startTk;
		deleteInstructionsAfter(oldLastInstr);
	}

	if (exprOr(rv))
	{
		return 1;
	}
	// crtTk = startTk;
	return 0;
}

int exprOrPrim(RetVal *rv)
{
	printf("@exprOrPrim %s\n", codeName(crtTk->code));
	Instr *i1, *i2;
	Type t, t1, t2;
	if (consume(OR))
	{
		i1 = rv->type.nElements < 0 ? getRVal(rv) : lastInstruction;
		t1 = rv->type;
		RetVal rve;
		if (exprAnd(&rve))
		{
			if (rv->type.typeBase == TB_STRUCT || rve.type.typeBase == TB_STRUCT)
			{
				tkerr(crtTk, "A structure cannot be logically tested");
			}
			if (rv->type.nElements >= 0)
			{ // vectors
				addInstr(O_OR_A);
			}
			else
			{ // non-vectors
				i2 = getRVal(&rve);
				t2 = rve.type;
				t = getArithType(&t1, &t2);
				addCastInstr(i1, &t1, &t);
				addCastInstr(i2, &t2, &t);
				switch (t.typeBase)
				{
				case TB_INT:
					addInstr(O_OR_I);
					break;
				case TB_DOUBLE:
					addInstr(O_OR_D);
					break;
				case TB_CHAR:
					addInstr(O_OR_C);
					break;
				}
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
	Instr *i1, *i2;
	Type t, t1, t2;
	if (consume(AND))
	{
		RetVal rve;
		i1 = rv->type.nElements < 0 ? getRVal(rv) : lastInstruction;
		t1 = rv->type;
		if (exprEq(&rve))
		{
			if (rv->type.typeBase == TB_STRUCT || rve.type.typeBase == TB_STRUCT)
			{
				tkerr(crtTk, "A structure cannot be logically tested");
			}
			if (rv->type.nElements >= 0)
			{ // vectors
				addInstr(O_AND_A);
			}
			else
			{ // non-vectors
				i2 = getRVal(&rve);
				t2 = rve.type;
				t = getArithType(&t1, &t2);
				addCastInstr(i1, &t1, &t);
				addCastInstr(i2, &t2, &t);
				switch (t.typeBase)
				{
				case TB_INT:
					addInstr(O_AND_I);
					break;
				case TB_DOUBLE:
					addInstr(O_AND_D);
					break;
				case TB_CHAR:
					addInstr(O_AND_C);
					break;
				}
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
	Token *tkopr;
	Instr *i1, *i2;
	Type t, t1, t2;
	printf("@exprEqPrim %s\n", codeName(crtTk->code));
	if (consume(EQUAL) || consume(NOTEQ))
	{
		tkopr = consumedTk;
		RetVal rve;
		i1 = rv->type.nElements < 0 ? getRVal(rv) : lastInstruction;
		t1 = rv->type;
		if (exprRel(&rve))
		{
			if (rv->type.typeBase == TB_STRUCT || rve.type.typeBase == TB_STRUCT)
				tkerr(crtTk, "A structure cannot be compared");
			if (rv->type.nElements >= 0)
			{ // vectors
				addInstr(tkopr->code == EQUAL ? O_EQ_A : O_NOTEQ_A);
			}
			else
			{ // non-vectors
				i2 = getRVal(&rve);
				t2 = rve.type;
				t = getArithType(&t1, &t2);
				addCastInstr(i1, &t1, &t);
				addCastInstr(i2, &t2, &t);
				if (tkopr->code == EQUAL)
				{
					switch (t.typeBase)
					{
					case TB_INT:
						addInstr(O_EQ_I);
						break;
					case TB_DOUBLE:
						addInstr(O_EQ_D);
						break;
					case TB_CHAR:
						addInstr(O_EQ_C);
						break;
					}
				}
				else
				{
					switch (t.typeBase)
					{
					case TB_INT:
						addInstr(O_NOTEQ_I);
						break;
					case TB_DOUBLE:
						addInstr(O_NOTEQ_D);
						break;
					case TB_CHAR:
						addInstr(O_NOTEQ_C);
						break;
					}
				}
			}
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
	Token *tkopr;
	Instr *i1, *i2;
	Type t, t1, t2;
	printf("@exprRelPrim %s\n", codeName(crtTk->code));
	if (consume(LESS) || consume(LESSEQ) || consume(GREATER) || consume(GREATEREQ))
	{
		tkopr = consumedTk;
		RetVal rve;
		i1 = getRVal(rv);
		t1 = rv->type;
		if (exprAdd(&rve))
		{
			if (rv->type.nElements > -1 || rve.type.nElements > -1)
				tkerr(crtTk, "An array cannot be compared");
			if (rv->type.typeBase == TB_STRUCT || rve.type.typeBase == TB_STRUCT)
				tkerr(crtTk, "A structure cannot be compared");
			i2 = getRVal(&rve);
			t2 = rve.type;
			t = getArithType(&t1, &t2);
			addCastInstr(i1, &t1, &t);
			addCastInstr(i2, &t2, &t);
			switch (tkopr->code)
			{
			case LESS:
				switch (t.typeBase)
				{
				case TB_INT:
					addInstr(O_LESS_I);
					break;
				case TB_DOUBLE:
					addInstr(O_LESS_D);
					break;
				case TB_CHAR:
					addInstr(O_LESS_C);
					break;
				}
				break;
			case LESSEQ:
				switch (t.typeBase)
				{
				case TB_INT:
					addInstr(O_LESSEQ_I);
					break;
				case TB_DOUBLE:
					addInstr(O_LESSEQ_D);
					break;
				case TB_CHAR:
					addInstr(O_LESSEQ_C);
					break;
				}
				break;
			case GREATER:
				switch (t.typeBase)
				{
				case TB_INT:
					addInstr(O_GREATER_I);
					break;
				case TB_DOUBLE:
					addInstr(O_GREATER_D);
					break;
				case TB_CHAR:
					addInstr(O_GREATER_C);
					break;
				}
				break;
			case GREATEREQ:
				switch (t.typeBase)
				{
				case TB_INT:
					addInstr(O_GREATEREQ_I);
					break;
				case TB_DOUBLE:
					addInstr(O_GREATEREQ_D);
					break;
				case TB_CHAR:
					addInstr(O_GREATEREQ_C);
					break;
				}
				break;
			}
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
	Token *tkopr;
	Instr *i1, *i2;
	Type t1, t2;
	printf("@exprAddPrim %s\n", codeName(crtTk->code));
	if (consume(ADD) || consume(SUB))
	{
		tkopr = consumedTk;
		RetVal rve;
		i1 = getRVal(rv);
		t1 = rv->type;
		if (exprMul(&rve))
		{
			if (rv->type.nElements > -1 || rve.type.nElements > -1)
				tkerr(crtTk, "An array cannot be added or subtracted");
			if (rv->type.typeBase == TB_STRUCT || rve.type.typeBase == TB_STRUCT)
				tkerr(crtTk, "A structure cannot be added or subtracted");
			rv->type = getArithType(&rv->type, &rve.type);
			i2 = getRVal(&rve);
			t2 = rve.type;
			addCastInstr(i1, &t1, &rv->type);
			addCastInstr(i2, &t2, &rv->type);
			if (tkopr->code == ADD)
			{
				switch (rv->type.typeBase)
				{
				case TB_INT:
					addInstr(O_ADD_I);
					break;
				case TB_DOUBLE:
					addInstr(O_ADD_D);
					break;
				case TB_CHAR:
					addInstr(O_ADD_C);
					break;
				}
			}
			else
			{
				switch (rv->type.typeBase)
				{
				case TB_INT:
					addInstr(O_SUB_I);
					break;
				case TB_DOUBLE:
					addInstr(O_SUB_D);
					break;
				case TB_CHAR:
					addInstr(O_SUB_C);
					break;
				}
			}
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
	Token *tkopr;
	Instr *i1, *i2;
	Type t1, t2;
	printf("@exprMulPrim %s\n", codeName(crtTk->code));
	if (consume(MUL) || consume(DIV))
	{
		tkopr = consumedTk;
		RetVal rve;
		i1 = getRVal(rv);
		t1 = rv->type;
		if (exprCast(&rve))
		{
			if (rv->type.nElements > -1 || rve.type.nElements > -1)
				tkerr(crtTk, "An array cannot be multiplied or divided");
			if (rv->type.typeBase == TB_STRUCT || rve.type.typeBase == TB_STRUCT)
				tkerr(crtTk, "A structure cannot be multiplied or divided");
			rv->type = getArithType(&rv->type, &rve.type);
			i2 = getRVal(&rve);
			t2 = rve.type;
			addCastInstr(i1, &t1, &rv->type);
			addCastInstr(i2, &t2, &rv->type);
			if (tkopr->code == MUL)
			{
				switch (rv->type.typeBase)
				{
				case TB_INT:
					addInstr(O_MUL_I);
					break;
				case TB_DOUBLE:
					addInstr(O_MUL_D);
					break;
				case TB_CHAR:
					addInstr(O_MUL_C);
					break;
				}
			}
			else
			{
				switch (rv->type.typeBase)
				{
				case TB_INT:
					addInstr(O_DIV_I);
					break;
				case TB_DOUBLE:
					addInstr(O_DIV_D);
					break;
				case TB_CHAR:
					addInstr(O_DIV_C);
					break;
				}
			}
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
	Instr *oldLastInstr = lastInstruction;
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
					if (rv->type.nElements < 0 && rv->type.typeBase != TB_STRUCT)
					{
						switch (rve.type.typeBase)
						{
						case TB_CHAR:
							switch (t.typeBase)
							{
							case TB_INT:
								addInstr(O_CAST_C_I);
								break;
							case TB_DOUBLE:
								addInstr(O_CAST_C_D);
								break;
							}
							break;
						case TB_DOUBLE:
							switch (t.typeBase)
							{
							case TB_CHAR:
								addInstr(O_CAST_D_C);
								break;
							case TB_INT:
								addInstr(O_CAST_D_I);
								break;
							}
							break;
						case TB_INT:
							switch (t.typeBase)
							{
							case TB_CHAR:
								addInstr(O_CAST_I_C);
								break;
							case TB_DOUBLE:
								addInstr(O_CAST_I_D);
								break;
							}
							break;
						}
					}
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
		deleteInstructionsAfter(oldLastInstr);
	}
	if (exprUnary(rv))
		return 1;
	crtTk = startTk;
	return 0;
}

int exprUnary(RetVal *rv)
{
	Token *startTk = crtTk;
	Token *tkopr;
	printf("@exprUnary %s\n", codeName(crtTk->code));
	if (consume(SUB) || consume(NOT))
	{
		tkopr = consumedTk;
		if (exprUnary(rv))
		{
			if (tkopr->code == SUB)
			{
				if (rv->type.nElements >= 0)
					tkerr(crtTk, "Unary '-' cannot be applied to an array");
				if (rv->type.typeBase == TB_STRUCT)
					tkerr(crtTk, "Unary '-' cannot be applied to a struct");
				getRVal(rv);
				switch (rv->type.typeBase)
				{
				case TB_CHAR:
					addInstr(O_NEG_C);
					break;
				case TB_INT:
					addInstr(O_NEG_I);
					break;
				case TB_DOUBLE:
					addInstr(O_NEG_D);
					break;
				}
			}
			else
			{ // NOT
				if (rv->type.typeBase == TB_STRUCT)
					tkerr(crtTk, "'!' cannot be applied to a struct");
				if (rv->type.nElements < 0)
				{
					getRVal(rv);
					switch (rv->type.typeBase)
					{
					case TB_CHAR:
						addInstr(O_NOT_C);
						break;
					case TB_INT:
						addInstr(O_NOT_I);
						break;
					case TB_DOUBLE:
						addInstr(O_NOT_D);
						break;
					}
				}
				else
				{
					addInstr(O_NOT_A);
				}
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
			rv->type.nElements = -1;
			rv->isLVal = 1;
			rv->isCtVal = 0;
			if (consume(RBRACKET))
			{
				addCastInstr(lastInstruction, &rve.type, &typeInt);
				getRVal(&rve);
				if (typeBaseSize(&rv->type) != 1)
				{
					addInstrI(O_PUSHCT_I, typeBaseSize(&rv->type));
					addInstr(O_MUL_I);
				}
				addInstr(O_OFFSET);
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
				if (rv->type.typeBase == TB_STRUCT)
				{
					Symbol *sMember = findSymbol(&sStruct->members, tkName->text);
					if (!sMember)
						tkerr(crtTk, "Struct %s does not have a member %s", sStruct->name, tkName->text);
					rv->type = sMember->type;
					if (sMember->offset)
					{
						addInstrI(O_PUSHCT_I, sMember->offset);
						addInstr(O_OFFSET);
					}
				}
				else
				{
					tkerr(crtTk, "Request for member '%s' in something not a structure", tkName->text);
				}
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
	int isFunc = 0;
	Instr *i;
	if (consume(ID))
	{
		tkName = consumedTk;
		Symbol *s = findSymbol(&symbols, tkName->text);
		if (!s)
			tkerr(crtTk, "Undefined symbol %s", tkName->text);
		rv->type = s->type;
		rv->isCtVal = 0;
		rv->isLVal = 1;
		if (s->cls == CLS_FUNC || s->cls == CLS_EXTFUNC) // // //
		{
			rv->isLVal = 0;
			isFunc = 1;
		}
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
				if ((*crtDefArg)->type.nElements < 0)
				{ //only arrays are passed by addr
					i = getRVal(&arg);
				}
				else
				{
					i = lastInstruction;
				}
				addCastInstr(i, &arg.type, &(*crtDefArg)->type);
				crtDefArg++;
			}
			while (1)
			{
				if (consume(COMMA))
				{
					RetVal arg;
					if (expr(&arg))
					{
						if (crtDefArg == s->args.end)
							tkerr(crtTk, "Too many arguments in call");
						cast(&(*crtDefArg)->type, &arg.type);
						if ((*crtDefArg)->type.nElements < 0)
						{
							i = getRVal(&arg);
						}
						else
						{
							i = lastInstruction;
						}
						addCastInstr(i, &arg.type, &(*crtDefArg)->type);
						crtDefArg++;
					}
					else
					{
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
				i = addInstr(s->cls == CLS_FUNC ? O_CALL : O_CALLEXT);
				i->args[0].addr = s->addr;
			}
			else
			{
				if (s->cls == CLS_FUNC || s->cls == CLS_EXTFUNC)
					tkerr(crtTk, "Missing call for function %s", tkName->text);
				tkerr(crtTk, "Invalid expression or missing a )");
			}
		}
		else
		{
			if (isFunc == 1)
			{
				if (s->cls == CLS_FUNC || s->cls == CLS_EXTFUNC)
					tkerr(crtTk, "Missing call for function %s", tkName->text);
			}
			if (s->depth)
			{
				addInstrI(O_PUSHFPADDR, s->offset);
			}
			else
			{
				addInstrA(O_PUSHCT_A, s->addr);
			}
		}
		return 1;
	}
	else if (consume(CT_INT))
	{
		Token *tki;
		tki = consumedTk;
		rv->type = createType(TB_INT, -1);
		rv->ctVal.i = tki->i;
		rv->isCtVal = 1;
		rv->isLVal = 0;
		addInstrI(O_PUSHCT_I,tki->i);
		return 1;
	}
	else if (consume(CT_REAL))
	{
		Token *tkr;
		tkr = consumedTk;
		rv->type = createType(TB_DOUBLE, -1);
		rv->ctVal.d = tkr->r;
		rv->isCtVal = 1;
		rv->isLVal = 0;
		i=addInstr(O_PUSHCT_D);i->args[0].d=tkr->r;
		return 1;
	}
	else if (consume(CT_CHAR))
	{
		Token *tkc;
		tkc = consumedTk;
		rv->type = createType(TB_CHAR, -1);
		rv->ctVal.i = tkc->i;
		rv->isCtVal = 1;
		rv->isLVal = 0;
		addInstrI(O_PUSHCT_C,tkc->i);
		return 1;
	}
	else if (consume(CT_STRING))
	{
		Token *tks;
		tks = consumedTk;
		rv->type = createType(TB_CHAR, 0);
		rv->ctVal.str = tks->text;
		rv->isCtVal = 1;
		rv->isLVal = 0;
		addInstrA(O_PUSHCT_A,tks->text);
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
	showInstructions(instructions);
	run(instructions);
	terminare();
	return 0;
}
