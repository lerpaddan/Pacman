
/* vector.c */

#include <stdint.h>
#include "vector.h"



void vector_set(Vector *v, int x, int y)
{
	v->x = x;
	v->y = y;
}

void vector_copy(Vector *v, Vector const *u)
{
	vector_set(v, u->x, u->y);
}

void vector_add(Vector *v, Vector const *u)
{
	v->x += u->x;
	v->y += u->y;
}

void vector_scalar_multiply(Vector *v, int c)
{
	v->x *= c;
	v->y *= c;
}

int vector_eq(Vector const *v, Vector const *u)
{
	return v->x == u->x && v->y == u->y;
}
