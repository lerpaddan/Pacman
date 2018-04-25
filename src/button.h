
/* button.h */

#pragma once

#define BUTTON_BUFFER_SIZE 22 /* Highest amount of characters that can fit on one line */



/* Buttons can also be used as menu titles or information,
 * by setting the function variable to NULL
 */
typedef struct BUTTON
{
	void (*function)(void);
	char  string[BUTTON_BUFFER_SIZE];
}Button;

void button_cons(Button *, char const *, void (*)(void));
void button_press(Button const *);
void button_draw(Button const *, int);
