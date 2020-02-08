#include "pch.h"
#include "pmath.h"

static te_variable vars[] = 
{
	{"root", root, TE_FUNCTION2}
};

double pinterp(char* exprStr, int* error)
{
	te_expr* n = te_compile(exprStr, vars, 1, error);
	const double r = te_eval(n);
	te_free(n);
	return r;
}

double root(double num, double rt)
{
	return pow(num, 1 / rt);
}