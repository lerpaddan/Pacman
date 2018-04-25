
/* icon.h */

#pragma once

#include <stdint.h>
#include "vector.h"

#define FONT_WIDTH  6
#define FONT_HEIGHT 8
#define ICON_ENTITY_SIZE 5
#define ICON_BOOSTER_SIZE 4



typedef struct ICON
{
	int8_t const *data;
	Vector size;
}Icon;


extern int8_t const icon_player_north    [];
extern int8_t const icon_player_west     [];
extern int8_t const icon_player_south    [];
extern int8_t const icon_player_east     [];
extern int8_t const icon_player_satisfied[];

extern int8_t const icon_ghost1[];
extern int8_t const icon_ghost2[];

extern int8_t const icon_booster_speed [];
extern int8_t const icon_booster_freeze[];

extern int8_t const font[][FONT_WIDTH];

void icon_draw(Icon const *, Vector const *);
