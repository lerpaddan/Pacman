
/* entity.c */

#include <stddef.h>
#include "entity.h"
#include "vector.h"
#include "icon.h"
#include "level.h"
#include "peripherals.h"



/* Constructor
 */
void entity_cons(Entity *entity)
{
	entity->icon.data   = NULL;
	vector_set(&entity->icon.size, 0, 0);
	vector_set(&entity->pos, 0, 0);
	vector_set(&entity->vec, 0, 0);
}




/* Move an entity if possible with its movement vector,
 * and update the movement vector if no movement was possible
 */
void entity_move(Entity *entity)
{
	int x = entity->vec.x;
	int y = entity->vec.y;
	int const px = entity->pos.x + x;
	int const py = entity->pos.y + y;
	
	if(px < 0 || px >= LEVEL_COLUMNS)
		x = 0;
	if(py < 0 || py >= LEVEL_ROWS)
		y = 0;
	
	vector_set(&entity->vec, x, y);
	vector_add(&entity->pos, &entity->vec);
}




/* Check if two entities intersect each other.
 * Their respective icon sizes will be used as hitbox
 * An overlap by 1 pixel is allowed, (otherwise it gets too hard)
 */
int entity_intersects(Entity const *x, Entity const *y)
{
      Vector px, py;
      int    wx, wy;
      int    hx, hy;
      
      wx = x->icon.size.x - 2;
      wy = y->icon.size.x - 2;
      hx = x->icon.size.y - 2;
      hy = y->icon.size.y - 2;
      vector_set(&px, x->pos.x - (wx / 2) + 1, x->pos.y - (hx / 2) + 1);
      vector_set(&py, y->pos.x - (wy / 2) + 1, y->pos.y - (hy / 2) + 1);
      
      if(px.x > py.x + wy || px.x + wx < py.x)
	      return 0;
      if(px.y > py.y + hy || px.y + hx < py.y)
	      return 0;
      return 1;
}




void entity_draw(Entity const *entity)
{
	int const dx = entity->pos.x - (entity->icon.size.x / 2) + LEVEL_OFFSET_X;
	int const dy = entity->pos.y - (entity->icon.size.y / 2) + LEVEL_OFFSET_Y;
	
	icon_draw(&entity->icon, &(Vector){dx, dy});
}
