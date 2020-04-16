
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