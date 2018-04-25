
/* game.h */

#pragma once

#include "entity.h"

#define GAME_MAX_GHOSTS    6
#define GAME_HIGHSCORES    3
#define GAME_DEFAULT_LIVES 8
#define GAME_BOOST_TICKS   32



enum DIFFICULTY
{
	DIFFICULTY_EASY   = 1,
	DIFFICULTY_MEDIUM = 2,
	DIFFICULTY_HARD   = 3,
};


/* Bitmaps are laid out so all menus share the same bit 
 * (e.g. (STATE_MENU_MAIN & STATE_MENU) would evaluate to non-zero)
 * and vice versa for STATE_PLAYING_BOOST_X & STATE_PLAYING
 */
enum STATE
{
     STATE_PLAYING              = 0x0001,
     STATE_MENU                 = 0x0002,
     STATE_MENU_MAIN            = 0x0006,
     STATE_MENU_HIGHSCORES      = 0x000A,
     STATE_MENU_DIFFICULTY      = 0x0012,
     STATE_MENU_PAUSED	        = 0x0022,
     STATE_MENU_HELP            = 0x0042,
     STATE_MENU_ADVANCE	        = 0x0082,
     STATE_MENU_DEAD            = 0x0102,
     STATE_MENU_GAMEOVER        = 0x0202,
     STATE_PLAYING_BOOST        = 0x0401,
     STATE_PLAYING_BOOST_FREEZE = 0x0801,
     STATE_PLAYING_BOOST_SPEED  = 0x1401,
     STATE_MENU_WON             = 0x2002
};




extern Entity player;
extern Entity ghosts[GAME_MAX_GHOSTS];
extern int    state;
extern int    highscores[3]; 

void game_start(int);
void game_exit(void);
void game_init(void);
void game_tick(void);
void game_draw(void);

void game_continue(void);
void game_start_easy(void);
void game_start_medium(void);
void game_start_hard(void);

char *difficulty_to_string(void);
int game_score(void);
int game_level(void);
int game_lives(void);