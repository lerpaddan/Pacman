
/* vector.h */

#pragma once

#include <stdint.h>



/* Assuming the screen is not larger than 128 pixels in either height
 * or width, 8-bit integers will be enough
 */
typedef struct VECTOR
{
	int8_t x;
	int8_t y;
}Vector;


void vector_set(Vector *, int, int);
void vector_copy(Vector *, Vector const *);
void vector_add(Vector *, Vector const *);
void vector_scalar_multiply(Vector *, int);
int  vector_eq(Vector const *, Vector const *);
