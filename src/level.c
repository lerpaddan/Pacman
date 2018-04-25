
/* level.c */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "level.h"
#include "peripherals.h"
#include "entity.h"
#include "game.h"

#define VISIBLE_SQUARES (LEVEL_SQUARE_WALL | LEVEL_SQUARE_CONSUMABLE_COIN)
#define COIN_SPACING 2



extern int8_t const map_level_1[LEVEL_COLUMNS*LEVEL_ROWS];
extern int8_t const map_level_2[LEVEL_COLUMNS*LEVEL_ROWS];
extern int8_t const map_level_3[LEVEL_COLUMNS*LEVEL_ROWS];


typedef struct PORTAL
{
	Vector entrance;
	Vector exit;
}Portal;


typedef struct LEVEL
{
	int8_t const *map; /* Pointer to the map layout */
	int    coins;    /* Amount of coins */
	int    ghosts;   /* Amount of ghosts */
	Portal portal[LEVEL_MAX_PORTALS];
	Vector origin;   /* Player spawn */
	Vector spawn[LEVEL_MAX_SPAWNS];    /* Ghost spawn */
	Vector booster[LEVEL_MAX_BOOSTERS];
}Level;


static int square_exists(Vector const *);


/* All game logic manipulation will occurr on this map */
static int8_t map[LEVEL_COLUMNS][LEVEL_ROWS];
/* Default level data */
static Level default_level[DEFAULT_LEVELS];
/* currently loaded level information */
static Level loaded;




/* Load a level, spawn ghosts and populate it with boosters and coins
 */
void level_load(int level, int difficulty)
{       
	/* Perform initial argument sanity check */
	if(level > DEFAULT_LEVELS)
		level = DEFAULT_LEVELS;
	else if(level < 1)
		level = 1;
	
	/* load default level data into the loaded level */
	memcpy(&loaded, &default_level[level - 1], sizeof(struct LEVEL));
	
	/* Load level map data into the map */
	for(int x = 0; x < LEVEL_COLUMNS; ++x)
	{
		for(int y = 0; y < LEVEL_ROWS; ++y)
		{
			map[x][y] = loaded.map[y*LEVEL_COLUMNS + x];
		}
	}
	
	/* Spawn player */
	level_player_spawn(&player.pos);
	
	/* Set amount of and spawn ghosts */
	loaded.ghosts += difficulty - 1;
	for(int i = 0; i < loaded.ghosts; ++i)
	{
		entity_ghost_cons(&ghosts[i]);
		level_ghost_spawn(&ghosts[i].pos);
	}
	
	level_populate();
}




/* Populates the loaded level with coins spaced out evenly
 * with other coins and consumables
 */
void level_populate(void)
{
	for(int y = 0; y < LEVEL_ROWS; ++y)
	{
		for(int x = 0; x < LEVEL_COLUMNS; ++x)
		{
			if(map[x][y] != LEVEL_SQUARE_EMPTY || vector_eq(&(Vector){x, y}, &loaded.origin))
				goto next_iteration;
			
			/* Check the surrounding squares for consumables */
			for(int i = -COIN_SPACING; i <= COIN_SPACING; ++i)
			{
				for(int j = -COIN_SPACING; j <= COIN_SPACING; ++j)
				{
					if(level_square(&(Vector){x+i, y+j}) & (LEVEL_SQUARE_CONSUMABLE | LEVEL_SQUARE_PORTAL))
						goto next_iteration;
				}
			}
			/* If no other consumables found, place a coin */
			map[x][y] = LEVEL_SQUARE_CONSUMABLE_COIN;
			loaded.coins++;
			
			next_iteration: ;
		}
	}
}




/* Check if coordinates are within bounds
 */
static int square_exists(Vector const *coord)
{
	if(coord->x >= LEVEL_COLUMNS || coord->x < 0)
		return 0;
	if(coord->y >= LEVEL_ROWS || coord->y < 0)
		return 0;
	
	return 1;
}




/* Return the data on a given square
 */
int level_square(Vector const *coord)
{
	if(square_exists(coord))
		return map[coord->x][coord->y];
	else
		return LEVEL_SQUARE_EMPTY;
}




/* Clear a position of something (e.g a coin)
 */
void level_square_empty(Vector const *coord)
{       
	if(square_exists(coord))
		map[coord->x][coord->y] = LEVEL_SQUARE_EMPTY;
}




/* If a portal exists at the given coordinates, its exit position is written
 * to the 'out' parameter, otherwise no effect occurs
 */
void level_use_portal(Vector *out)
{
	for(int i = 0; i < LEVEL_MAX_PORTALS; ++i)
	{
		if(vector_eq(out, &loaded.portal[i].entrance))
		{
			vector_copy(out, &loaded.portal[i].exit);	 
			return;
		}
	}
}




/* Returns the amount of loadedly active ghosts
 */
int level_active_ghosts(void)
{
	return loaded.ghosts;
}




/* Decrements and returns the remaining coins 
 */
int level_take_coin(void)
{
	return --loaded.coins;
}




/* Writes the coordinates of the player/ghost spawn to the out parameter
 */
void level_player_spawn(Vector *out)
{
       vector_copy(out, &loaded.origin);
}

void level_ghost_spawn(Vector *out)
{
	/* This is a pretty ugly solution, but it cycles between spawn positions every call */
	static int previous = 0;
	
	if(previous >= LEVEL_MAX_SPAWNS)
		previous = 0;
	
	vector_copy(out, &loaded.spawn[previous++]);
}




/* Updates the graphics video buffer with the level information
 */
void level_draw(void)
{
	/* Draw squares and coins */
	for(int y = 0; y < LEVEL_ROWS; ++y)
	{
		for(int x = 0; x < LEVEL_COLUMNS; ++x)
		{
			graphics_set(x + LEVEL_OFFSET_X, y + LEVEL_OFFSET_Y, map[x][y] & VISIBLE_SQUARES);
		}
	}
	
	
	/* Draw boosters */
	Icon const speed = (Icon)
	{
		icon_booster_speed, 
		(Vector){ICON_BOOSTER_SIZE, ICON_BOOSTER_SIZE}
	};
	Icon const freeze = (Icon)
	{
		icon_booster_freeze,
		(Vector){ICON_BOOSTER_SIZE, ICON_BOOSTER_SIZE}
	};
	for(int i = 0; i < LEVEL_MAX_BOOSTERS; ++i)
	{
		Vector const position = (Vector)
		{
			loaded.booster[i].x + LEVEL_OFFSET_X - (ICON_BOOSTER_SIZE / 2),
			loaded.booster[i].y + LEVEL_OFFSET_Y - (ICON_BOOSTER_SIZE / 2)
		};
		
		if((level_square(&loaded.booster[i]) & LEVEL_SQUARE_CONSUMABLE_SPEED) == LEVEL_SQUARE_CONSUMABLE_SPEED)
			icon_draw(&speed, &position);
		else if((level_square(&loaded.booster[i]) & LEVEL_SQUARE_CONSUMABLE_FREEZE) == LEVEL_SQUARE_CONSUMABLE_FREEZE)
			icon_draw(&freeze, &position);
	}
}




/* Set up all the data for all default levels (UGLY)
 */
void level_init(void)
{
	/* Set up level 1 */
	default_level[0].map	            = map_level_1;
	default_level[0].coins	            = 0;
	default_level[0].ghosts	            = 1;
	default_level[0].origin	            = (Vector){31, 29};
	default_level[0].spawn[0]           = (Vector){26, 2};
	default_level[0].spawn[1]           = (Vector){31, 2};
	default_level[0].spawn[2]           = (Vector){36, 2};
	default_level[0].spawn[3]           = (Vector){31, 2};
	default_level[0].portal[0].entrance = (Vector){0, 16};
	default_level[0].portal[0].exit     = (Vector){61, 16};
	default_level[0].portal[1].entrance = (Vector){62, 16};
	default_level[0].portal[1].exit     = (Vector){1, 16};
	default_level[0].portal[2]          = default_level[0].portal[0];
	default_level[0].portal[3]          = default_level[0].portal[1];
	default_level[0].booster[0]         = (Vector){3, 2};
	default_level[0].booster[1]         = (Vector){59, 2};
	default_level[0].booster[2]         = (Vector){3, 29};
	default_level[0].booster[3]         = (Vector){59, 29};
	
	
	/* Level 2 */
	default_level[1].map                = map_level_2;
	default_level[1].coins	            = 0;
	default_level[1].ghosts	            = 2;
	default_level[1].origin	            = (Vector){31, 15};
	default_level[1].spawn[0]           = (Vector){8, 15};
	default_level[1].spawn[1]           = (Vector){54, 15};
	default_level[1].spawn[2]           = default_level[1].spawn[0];
	default_level[1].spawn[3]           = default_level[1].spawn[1];
	default_level[1].portal[0].entrance = (Vector){14, 0};
	default_level[1].portal[0].exit     = (Vector){14, 30};
	default_level[1].portal[1].entrance = (Vector){14, 31};
	default_level[1].portal[1].exit     = (Vector){14, 1};
	default_level[1].portal[2].entrance = (Vector){48, 0};
	default_level[1].portal[2].exit     = (Vector){48, 30};
	default_level[1].portal[3].entrance = (Vector){48, 31};
	default_level[1].portal[3].exit     = (Vector){48, 1};
	default_level[1].booster[0]         = (Vector){2, 3};
	default_level[1].booster[1]         = (Vector){2, 27};
	default_level[1].booster[2]         = (Vector){60, 3};
	default_level[1].booster[3]         = (Vector){60, 27};
	
	
	/* Level 3 */
	default_level[2].map                = map_level_3;
	default_level[2].coins	            = 0;
	default_level[2].ghosts	            = 4;
	default_level[2].origin	            = (Vector){31, 15};
	default_level[2].spawn[0]           = (Vector){3, 9};
	default_level[2].spawn[1]           = (Vector){3, 21};
	default_level[2].spawn[2]           = (Vector){59, 9};
	default_level[2].spawn[3]           = (Vector){59, 21};
	default_level[2].portal[0].entrance = (Vector){0, 3};
	default_level[2].portal[0].exit     = (Vector){62, 27};
	default_level[2].portal[1].entrance = (Vector){0, 27};
	default_level[2].portal[1].exit     = (Vector){62, 3};
	default_level[2].portal[2].entrance = (Vector){63, 3};
	default_level[2].portal[2].exit     = (Vector){1, 27};
	default_level[2].portal[3].entrance = (Vector){63, 27};
	default_level[2].portal[3].exit     = (Vector){1, 3};
	default_level[2].booster[0]         = (Vector){15, 15};
	default_level[2].booster[1]         = (Vector){47, 15};
	default_level[2].booster[2]         = (Vector){15, 15};
	default_level[2].booster[3]         = (Vector){47, 15};
}
