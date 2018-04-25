
/* level.h */

#pragma once

#include <stdint.h>
#include "vector.h"

#define LEVEL_COLUMNS      64
#define LEVEL_ROWS	 32
#define LEVEL_OFFSET_X     32
#define LEVEL_OFFSET_Y     0
#define LEVEL_MAX_PORTALS  4
#define LEVEL_MAX_SPAWNS   4
#define LEVEL_MAX_BOOSTERS 4
#define DEFAULT_LEVELS     3



enum LEVEL_SQUARE
{
	LEVEL_SQUARE_EMPTY	     = 0x00,
	LEVEL_SQUARE_WALL	      = 0x01,
	LEVEL_SQUARE_INACCESSIBLE      = 0x02,
	LEVEL_SQUARE_PORTAL	    = 0x04,
	LEVEL_SQUARE_GHOST	     = 0x08,
	LEVEL_SQUARE_CONSUMABLE	= 0x10,
	LEVEL_SQUARE_CONSUMABLE_COIN   = 0x30,
	LEVEL_SQUARE_CONSUMABLE_SPEED  = 0x50,
	LEVEL_SQUARE_CONSUMABLE_FREEZE = 0x90
};



void level_init(void);
void level_load(int, int);
void level_populate(void);
void level_draw(void);

int  level_square(Vector const *);
void level_square_empty(Vector const *);
void level_use_portal(Vector *);
void level_player_spawn(Vector *);
void level_ghost_spawn(Vector *);
int  level_active_ghosts(void);
int  level_take_coin(void);
