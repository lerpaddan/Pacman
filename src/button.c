
/* button.c */

#include <string.h>
#include "button.h"
#include "peripherals.h"
#include "icon.h"



void button_cons(Button *button, char const *string, void (*function)(void))
{
	if(strlen(string) < BUTTON_BUFFER_SIZE)
		strcpy(button->string, string);
	else
		strcpy(button->string, "INVALID");
	button->function = function;
}


void button_press(Button const *button)
{
	if(button->function != NULL)
		button->function();
}


void button_draw(Button const *button, int line)
{
	int const sz     = strlen(button->string);
	int const offset = (DSP_COLUMNS / 2) - ((sz / 2) * FONT_WIDTH) - ((sz & 1) ? (FONT_WIDTH / 2) : 0);
	
	graphics_print(offset, line, button->string);
}