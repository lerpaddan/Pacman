
/* entity.h */

#pragma once

#include <stdint.h>
#include "vector.h"
#include "icon.h"



typedef struct ENTITY
{
	Icon   icon; /* Pointer to icon data */
	Vector pos;  /* Position vector */
	Vector vec;  /* Movement vector */
}Entity;


void entity_cons(Entity *);
void entity_move(Entity *);
void entity_draw(Entity const *);
int  entity_intersects(Entity const *, Entity const *);

void entity_player_cons(Entity *);
void entity_player_move(Entity *);
void entity_player_tick(Entity *);

void entity_ghost_cons(Entity *);
void entity_ghost_move(Entity *);
void entity_ghost_tick(Entity *);
