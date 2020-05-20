
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

void initSymbols(Symbols *symbols)
{
	symbols->begin = NULL;
	symbols->end = NULL;
	symbols->after = NULL;
}

Symbol *addSymbol(Symbols *symbols, const char *name, int cls)
{
	Symbol *s;
	if (symbols->end == symbols->after)
	{
		// create more room
		int count = symbols->after - symbols->begin;
		int n = count * 2; // double the room
		if (n == 0)
			n = 1; // needed for the initial case
		symbols->begin = (Symbol **)realloc(symbols->begin, n * sizeof(Symbol *));
		if (symbols->begin == NULL)
			err("Not enough memory");
		symbols->end = symbols->begin + count;
		symbols->after = symbols->begin + n;
	}
	SAFEALLOC(s, Symbol)
	*symbols->end++ = s;
	s->name = name;
	s->cls = cls;
	s->depth = crtDepth;
	return s;
}

Symbol *findSymbol(Symbols *symbols, const char *name)
{
	int n = symbols->end - symbols->begin;
	for (int i = n - 1; i >= 0; i--)
	{
		if (strcmp(symbols->begin[i]->name, name) == 0)
		{
			printf("Found it\n");
			return symbols->begin[i];
		}
	}
	printf("Did not find it\n");
	return NULL;
}

Symbol *requireSymbol(Symbols *symbols, const char *name)
{
	int n = symbols->end - symbols->begin;
	for (int i = n - 1; i >= 0; i--)
	{
		if (strcmp(symbols->begin[i]->name, name) == 0)
		{
			printf("Found it\n");
			return symbols->begin[i];
		}
	}
	printf("Did not find it\n");
	err("Missing symbol");
	return NULL;
}

void deleteSymbolsAfter(Symbols *symbols, Symbol *start)
{
	int n = symbols->end - symbols->begin;
	for (int i = 0; i < n; i++)
	{
		if (strcmp(symbols->begin[i]->name, start->name) == 0)
		{
			i = i + 1;
			for (int j = i; j < n; j++)
			{
				free(symbols->begin[j]);
				symbols->begin[j] = NULL;
				symbols->end--;
			}
			return;
		}
	}
	return;
}

void printSymbols(Symbols *symbols)
{
	int n = symbols->end - symbols->begin;
	for (int i = 0; i < n; i++)
	{
		printf("%s\n", symbols->begin[i]->name);
	}
}

Type createType(int typeBase, int nElements)
{
	Type t;
	t.typeBase = typeBase;
	t.nElements = nElements;
	return t;
}

Symbol *addExtFunc(const char *name, Type type, void *addr)
{
	Symbol *s = addSymbol(&symbols, name, CLS_EXTFUNC);
	s->type = type;
	s->addr = addr;
	initSymbols(&s->args);
	return s;
}

Symbol *addFuncArg(Symbol *func, const char *name, Type type)
{
	Symbol *a = addSymbol(&func->args, name, CLS_VAR);
	a->type = type;
	return a;
}

void put_i()
{
	printf("#%d\n", popi());
}

void get_i()
{
	int i;
	putchar('>');
	scanf("%d", &i);
	pushi(i);
}

void put_c()
{
	printf("#%c\n", popc());
}

void get_c()
{
	char c;
	putchar('>');
	c=getchar();
	pushc(c);
}

void put_d()
{
	printf("#%g\n", popd());
}

void get_d()
{
	double d;
	putchar('>');
	scanf("%lg", &d);
	pushd(d);
}

void put_s()
{
	printf("#%s\n", (char*)popa());
}

void get_s()
{
	putchar('>');
	fgets(popa(), 100, stdin);
}

void addExtFuncs()
{ /*
	s = addExtFunc("put_s", createType(TB_VOID, -1));
	addFuncArg(s, "s", createType(TB_CHAR, 0));

	s = addExtFunc("get_s", createType(TB_VOID, -1));
	addFuncArg(s, "s", createType(TB_CHAR, 0));

	s = addExtFunc("get_i", createType(TB_INT, -1));

	s = addExtFunc("put_d", createType(TB_VOID, -1));
	addFuncArg(s, "d", createType(TB_DOUBLE, -1));

	s = addExtFunc("get_d", createType(TB_DOUBLE, -1));

	s = addExtFunc("put_c", createType(TB_VOID, -1));
	addFuncArg(s, "c", createType(TB_CHAR, -1));

	s = addExtFunc("get_c", createType(TB_CHAR, -1));

	s = addExtFunc("seconds", createType(TB_DOUBLE, -1));
	*/
	Symbol *s, *a;
	s = addExtFunc("put_i", createType(TB_VOID, -1), put_i);
	a = addSymbol(&s->args, "i", CLS_VAR);
	a->type = createType(TB_INT, -1);

	s = addExtFunc("get_i", createType(TB_INT, -1), get_i);

	s = addExtFunc("put_c", createType(TB_VOID, -1), put_c);
	a = addSymbol(&s->args, "c", CLS_VAR);
	a->type = createType(TB_CHAR, -1);

	s = addExtFunc("get_c", createType(TB_CHAR, -1), get_c);

	s = addExtFunc("put_d", createType(TB_VOID, -1), put_d);
	a = addSymbol(&s->args, "d", CLS_VAR);
	a->type = createType(TB_DOUBLE, -1);

	s = addExtFunc("get_d", createType(TB_DOUBLE, -1), get_d);

	s = addExtFunc("put_s", createType(TB_VOID, -1), put_s);
	a = addSymbol(&s->args, "s", CLS_VAR);
	a->type = createType(TB_CHAR, 0);

	s = addExtFunc("get_s", createType(TB_VOID, -1), get_s);
	a = addSymbol(&s->args, "s", CLS_VAR);
	a->type = createType(TB_CHAR, 0);
}

void addVar(Token *tkName, Type *t)
{
	Symbol *s;
	if (crtStruct)
	{
		if (findSymbol(&crtStruct->members, tkName->text))
			tkerr(crtTk, "Symbol redefinition: %s", tkName->text);
		s = addSymbol(&crtStruct->members, tkName->text, CLS_VAR);
	}
	else if (crtFunc)
	{
		s = findSymbol(&symbols, tkName->text);
		if (s && s->depth == crtDepth)
			tkerr(crtTk, "Symbol redefinition: %s", tkName->text);
		s = addSymbol(&symbols, tkName->text, CLS_VAR);
		s->mem = MEM_LOCAL;
	}
	else
	{
		if (findSymbol(&symbols, tkName->text))
			tkerr(crtTk, "Symbol redefinition: %s", tkName->text);
		s = addSymbol(&symbols, tkName->text, CLS_VAR);
		s->mem = MEM_GLOBAL;
	}
	s->type = *t;
	if (crtStruct || crtFunc)
	{
		s->offset = offset;
	}
	else
	{
		s->addr = allocGlobal(typeFullSize(&s->type));
	}
	offset += typeFullSize(&s->type);
}

void cast(Type *dst, Type *src)
{
	if (src->nElements > -1)
	{
		if (dst->nElements > -1)
		{
			if (src->typeBase != dst->typeBase)
				tkerr(crtTk, "An array cannot be converted to an array of another type");
		}
		else
		{
			tkerr(crtTk, "An array cannot be converted to a non array");
		}
	}
	else
	{
		if (dst->nElements > -1)
		{
			tkerr(crtTk, "A non-array cannot be converted to an array");
		}
	}
	switch (src->typeBase)
	{
	case TB_CHAR:
	case TB_INT:
	case TB_DOUBLE:
		switch (dst->typeBase)
		{
		case TB_CHAR:
		case TB_INT:
		case TB_DOUBLE:
			return;
		}
	case TB_STRUCT:
		if (dst->typeBase == TB_STRUCT)
		{
			if (src->s != dst->s)
				tkerr(crtTk, "A structure cannot be converted to another one");
			return;
		}
	}
	tkerr(crtTk, "Incompatible types");
}

Type getArithType(Type *s1, Type *s2)
{
	switch (s1->typeBase)
	{
	case TB_CHAR:
		switch (s2->typeBase)
		{
		case TB_CHAR:
			return *s1;
		case TB_INT:
			return *s2;
		case TB_DOUBLE:
			return *s2;
		default:
			tkerr(crtTk, "Incompatible types");
		}
		break;
	case TB_INT:
		switch (s2->typeBase)
		{
		case TB_CHAR:
			return *s1;
		case TB_INT:
			return *s1;
		case TB_DOUBLE:
			return *s2;
		default:
			tkerr(crtTk, "Incompatible types");
		}
		break;
	case TB_DOUBLE:
		switch (s2->typeBase)
		{
		case TB_CHAR:
			return *s1;
		case TB_INT:
			return *s1;
		case TB_DOUBLE:
			return *s1;
		default:
			tkerr(crtTk, "Incompatible types");
		}
		break;
	}
	tkerr(crtTk, "Incompatible types");
	return createType(TB_VOID, -1);
}