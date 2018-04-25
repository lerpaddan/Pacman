
/* ghost.c */

#include <stdlib.h>
#include <pic32mx.h>
#include "entity.h"
#include "level.h"
#include "vector.h"
#include "icon.h"
#include "peripherals.h"

#define INACCESSIBLE_SQUARES (LEVEL_SQUARE_WALL | LEVEL_SQUARE_INACCESSIBLE)



static void pathfind(Entity *);




void entity_ghost_cons(Entity *ghost)
{
	entity_cons(ghost);
	ghost->icon.data   = (rand() & 1) ? icon_ghost1 : icon_ghost2;
	ghost->icon.size.x = ICON_ENTITY_SIZE;
	ghost->icon.size.y = ICON_ENTITY_SIZE;
}




/* Same function as entity_move(), except with added checks
 */
void entity_ghost_move(Entity *ghost)
{
	int x = ghost->vec.x;
	int y = ghost->vec.y;
	
	if(level_square(&(Vector){ghost->pos.x + x, ghost->pos.y}) & INACCESSIBLE_SQUARES)
		x = 0;
	if(level_square(&(Vector){ghost->pos.x, ghost->pos.y + y}) & INACCESSIBLE_SQUARES)
		y = 0;
	
	vector_set(&ghost->vec, x, y);
	entity_move(ghost);
}




void entity_ghost_tick(Entity *ghost)
{
	/* Look around for alternate paths */
	pathfind(ghost);
	/* Switch between the two icons every tick */
	ghost->icon.data = (ghost->icon.data == icon_ghost1) ? icon_ghost2 : icon_ghost1;
	
	entity_ghost_move(ghost);
}




/* Decide which paths to take and update the movement vector
 */
static void pathfind(Entity *ghost)
{
	/* If stationary, set a random direction */
	if(vector_eq(&ghost->vec, &(Vector){0, 0}))
	{
		int8_t *var = (rand() & 1) ? &ghost->vec.x : &ghost->vec.y;
		*var = (rand() & 1) ? -1 : 1;
		
		return;
	}
	
	/* If rand() returns a clean multiple of 4, attempt to find an alternate path */
	Vector v, u;
	if((rand() & 3))
		return;
	else if(ghost->vec.y == 0)
		vector_set(&v, 0, 1);
	else
		vector_set(&v, 1, 0);
	
	if(rand() & 1)
		vector_scalar_multiply(&v, -1);
	
	/* Check if there is clear space on right/left/above/below sides */
	/* and if there is, update the movement vector */
	vector_copy(&u, &ghost->pos);
	vector_add(&u, &v);
	if(level_square(&u) & INACCESSIBLE_SQUARES)
	{
		vector_scalar_multiply(&v, -1);
		vector_add(&u, &v);
		vector_add(&u, &v);
		
		if(level_square(&u) & INACCESSIBLE_SQUARES)
			return;
	}
	
	vector_copy(&ghost->vec, &v);
}
