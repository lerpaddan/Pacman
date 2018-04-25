
/* interface.c */

#include <string.h>
#include <stddef.h>
#include "interface.h"
#include "button.h"
#include "peripherals.h"
#include "game.h"
#include "icon.h"
#include "functions.h"

#define BUTTONS 4 /* 4 pages/lines available, and as such; 4 buttons */



static Button buttons[BUTTONS]; 
static int selected = 0;




/* Draw all buttons with an outline around the currently selected one
 */
void interface_draw(void)
{
	/* Draw buttons */
	for(int i = 0; i < BUTTONS; ++i)
	{
		button_draw(&buttons[i], i);
	}
	
	/* Draw outline around selected button */
	int const page = selected * 8;
	
	for(int y = page; y < page + 8; ++y)
	{
		for(int x = 0; x < DSP_COLUMNS; ++x)
		{
			if((x == 0 || x == DSP_COLUMNS - 1) || (y == page || y == page + 7))
				graphics_set(x, y, 1);
		}
	}
}




/* Press the currently selected button,
 * and reset the selected index
 */
void interface_button_press(void)
{
	button_press(&buttons[selected]);
	
	if(buttons[selected].function != NULL)
		selected = 0;
}


void interface_button_prev(void)
{
	if(--selected < 0)
		selected = BUTTONS - 1;
}


void interface_button_next(void)
{
	if(++selected >= BUTTONS)
		selected = 0;
}




void interface_menu_load_paused(void)
{
	state = STATE_MENU_PAUSED;
	
	button_cons(&buttons[0], "PAUSED", NULL);
	
	char buffer[BUTTON_BUFFER_SIZE];
	strcpy(buffer, "LEVEL: ");
	itodsconv(&buffer[7], game_level());
	button_cons(&buttons[1], buffer, NULL);
	
	strcpy(buffer, "SCORE: ");
	itodsconv(&buffer[7], game_score());
	button_cons(&buttons[2], buffer, NULL);
	
	strcpy(buffer, "DIFFICULTY: ");
	strcat(&buffer[12], difficulty_to_string());
	button_cons(&buttons[3], buffer, NULL);
}


void interface_menu_load_main(void)
{
	state = STATE_MENU_MAIN;
	
	button_cons(&buttons[0], "PACMAN",      NULL);
	button_cons(&buttons[1], "START GAME",  interface_menu_load_difficulty);
	button_cons(&buttons[2], "HIGHSCORES",  interface_menu_load_highscores);
	button_cons(&buttons[3], "HOW TO PLAY", interface_menu_load_help);
}


void interface_menu_load_highscores(void)
{
	state = STATE_MENU_HIGHSCORES;
	
	char buffer[BUTTON_BUFFER_SIZE];
	buffer[1] = '.';
	buffer[2] = ' ';
		
	int i;
	for(i = 0; i < GAME_HIGHSCORES; ++i)
	{
		buffer[0] = '1' + i;
		itodsconv(&buffer[3], highscores[i]);
		button_cons(&buttons[i], buffer, NULL);
	}
	button_cons(&buttons[3], "BACK", interface_menu_load_main);
}


void interface_menu_load_help(void)
{
	state = STATE_MENU_HELP;
	
	button_cons(&buttons[0], "BTNX: MOVE           ",  NULL);
	button_cons(&buttons[1], "SW4 : PAUSE          ",  NULL);
	button_cons(&buttons[2], "SW3 : INVERT DISPLAY " , NULL);
	button_cons(&buttons[3], "BACK", interface_menu_load_main);
}


void interface_menu_load_difficulty(void)
{
	state = STATE_MENU_DIFFICULTY;
	
	button_cons(&buttons[0], "CHOOSE A DIFFICULTY", NULL);
	button_cons(&buttons[1], "EASY",   game_start_easy);
	button_cons(&buttons[2], "MEDIUM", game_start_medium);
	button_cons(&buttons[3], "HARD",   game_start_hard);
}


void interface_menu_load_dead(void)
{
	state = STATE_MENU_DEAD;
	
	button_cons(&buttons[0], "YOU DIED", NULL);
	
	char buffer[BUTTON_BUFFER_SIZE];
	strcpy(buffer, "LIVES REMAINING: ");
	itodsconv(&buffer[17], game_lives());
	button_cons(&buttons[1], buffer, NULL);
	
	button_cons(&buttons[2], "", NULL);
	button_cons(&buttons[3], "RESPAWN", game_continue);
}


void interface_menu_load_gameover(void)
{
	state = STATE_MENU_GAMEOVER;
	
	button_cons(&buttons[0], "GAME OVER", NULL);
	
	char buffer[BUTTON_BUFFER_SIZE];
	strcpy(buffer, "SCORE: ");
	itodsconv(&buffer[7], game_score());
	button_cons(&buttons[1], buffer, NULL);
	
	button_cons(&buttons[2], "RETRY",     interface_menu_load_difficulty);
	button_cons(&buttons[3], "MAIN MENU", interface_menu_load_main);
	
}


void interface_menu_load_advance(void)
{
	state = STATE_MENU_ADVANCE;
	
	button_cons(&buttons[0], "CONGRATULATIONS", NULL);
	
	char buffer[BUTTON_BUFFER_SIZE];
	strcpy(buffer, "FINISHED LEVEL ");
	itodsconv(&buffer[15], game_level() - 1);
	button_cons(&buttons[1], buffer, NULL);
	
	button_cons(&buttons[2], "", NULL);
	button_cons(&buttons[3], "ADVANCE TO NEXT LEVEL", game_continue);
}


void interface_menu_load_won(void)
{
	state = STATE_MENU_WON;
	
	button_cons(&buttons[0], "YOU WON", NULL);
	
	char buffer[BUTTON_BUFFER_SIZE];
	
	strcpy(buffer, "FINAL SCORE: ");
	itodsconv(&buffer[13], game_score());
	button_cons(&buttons[1], buffer, NULL);
	
	strcpy(buffer, "DIFFICULTY: ");
	strcpy(&buffer[12], difficulty_to_string());
	button_cons(&buttons[2], buffer, NULL);
	
	button_cons(&buttons[3], "RETURN TO MAIN MENU", interface_menu_load_main);
}
