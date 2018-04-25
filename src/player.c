
/* player.c */

#include "entity.h"
#include "icon.h"
#include "level.h"
#include "vector.h"

#define INACCESSIBLE_SQUARES (LEVEL_SQUARE_WALL | LEVEL_SQUARE_INACCESSIBLE | LEVEL_SQUARE_GHOST)




void entity_player_cons(Entity *player)
{
	entity_cons(player);
	player->icon.data   = icon_player_west;
	player->icon.size.x = ICON_ENTITY_SIZE;
	player->icon.size.y = ICON_ENTITY_SIZE;
	
}



void entity_player_move(Entity *player)
{
	int x = player->vec.x;
	int y = player->vec.y;
	
	if(level_square(&(Vector){player->pos.x + x, player->pos.y}) & INACCESSIBLE_SQUARES)
		x = 0;
	if(level_square(&(Vector){player->pos.x, player->pos.y + y}) & INACCESSIBLE_SQUARES)
		y = 0;
	
	vector_set(&player->vec, x, y);
	entity_move(player);
}




void entity_player_tick(Entity *player)
{
	/* Update the icon */
	if(player->icon.data != icon_player_satisfied && (player->vec.x != 0 || player->vec.y != 0))
		player->icon.data = icon_player_satisfied;
	else if(player->vec.x > 0)
		player->icon.data = icon_player_east;
	else if(player->vec.x < 0)
		player->icon.data = icon_player_west;
	else if(player->vec.y > 0)
		player->icon.data = icon_player_south;
	else if(player->vec.y < 0)
		player->icon.data = icon_player_north;
	
	entity_player_move(player);
}
