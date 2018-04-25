
/* input.c */

#include <pic32mx.h>
#include "peripherals.h"
#include "game.h"
#include "entity.h"
#include "interface.h"
#include "functions.h"

/* There needs to be a delay in the menus, otherwise it's hard to select buttons */
#define MENU_DELAY 1000000


/* The controls are as follows:
 *      SW3: Toggle inverted display
 * When in menu:
 *      BTN2: Press button
 *      BTN1: Up
 *      BTN3: Down
 * When in game:
 *      SW4: Pause
 *      BTN4: Left
 *      BTN3: Down
 *      BTN2: Up
 *      BTN1: Right
 */



enum INPUT
{
	BTN1 = 0x01,
	BTN2 = 0x02,
	BTN3 = 0x04,
	BTN4 = 0x08,
	SW1  = 0x10,
	SW2  = 0x20,
	SW3  = 0x40,
	SW4  = 0x80
};


/* As input_poll() will be called continously, data will need to be 
 * collected continously and then reset when acted upon
 */
static volatile int8_t data = 0;




void input_init(void)
{
	/* PORTD takes SW4-SW1 in bits <11:8> and BTN4-BTN2 in bits <7:5> */
	TRISDSET = 0x7f << 5;
	/* PORTF takes BTN1 in bit <1:1> */
	TRISFSET = 0x01 << 1;
}




/* Returns board input as a byte with the 
 * input represented bitwise as
 * SW4 SW3 SW2 SW1 | BTN4 BTN3 BTN2 BTN1
 */
uint8_t input_poll(void)
{
	data |= (PORTD >> 4) & 0xFE; /* SW4-SW1, BTN4-BTN3 */
	data |= (PORTF >> 1) & 0x01; /* BTN1 */

	return data;
}


void input_update(void)
{
	/* State needs to be saved in case the player pauses whilst under */
	/* some kind of boost */
	static int prev_state;
	
	/* As data can be volatile, save it in a local variable for reading */
	int8_t const val = data;
	
	/* Set inversion flag */
	invert = val & SW3;
	
	/* Game instructions */
	if(state & STATE_PLAYING)
	{
		if(val & SW4)
		{
			prev_state = state;
			interface_menu_load_paused();
		}
		
		if(val & BTN1)
			player.vec.x = 1;
		if(val & BTN2)
			player.vec.y = -1;
		if(val & BTN3)
			player.vec.y = 1;
		if(val & BTN4)
			player.vec.x = -1;
	}
	/* Menu instructions */
	else if(state & STATE_MENU)
	{
		quicksleep(MENU_DELAY);
		
		if(val & BTN3)
			interface_button_next();
		else if(val & BTN2)
			interface_button_press();
		else if(val & BTN1)
			interface_button_prev();
		if(state == STATE_MENU_PAUSED && ! (val & SW4))
			state = prev_state;
			
		
	}
	
	/* Reset data for next polling */
	data = 0;
}
