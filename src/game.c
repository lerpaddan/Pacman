
/* game.c */

#include <stdlib.h>
#include "game.h"
#include "icon.h"
#include "level.h"
#include "interface.h"
#include "functions.h"
#include "peripherals.h"
#include "button.h"




Entity ghosts[GAME_MAX_GHOSTS];
Entity player;
int    state;
int    highscores[GAME_HIGHSCORES];


static int score;
static int level;
static int lives;
static int boost;
static int difficulty;

static void respawn(void);
static void calculate_highscore(void);
static void die(void);
static void gameover(void);
static void advance(void);
static void pickup_consumable(void);
static void respawn_ghosts(void);




/* Initialize the default levels, and load the main menu
 */
void game_init(void)
{
	boost = 0;
	level_init();
	interface_menu_load_main();
	
	for(int i = 0; i < GAME_HIGHSCORES; ++i)
	{
		highscores[i] = 0;
	}
}




/* Tick once, move player, ghosts and adjust state
 */
void game_tick(void)
{
	/* First check and adjust boost status */
	if(boost)
		boost--;
	else
		state = STATE_PLAYING;
	
	
	/* Perform a tick for the player */
	for(int i = 0; i < ((state == STATE_PLAYING_BOOST_SPEED) ? 2 : 1); ++i)
	{
		entity_player_tick(&player);
	
		if(level_square(&player.pos) & LEVEL_SQUARE_CONSUMABLE)
			pickup_consumable();
		else if(level_square(&player.pos) & LEVEL_SQUARE_PORTAL)
			level_use_portal(&player.pos);
	}
	/* Reset speed after every tick */
	vector_set(&player.vec, 0, 0);
	
	
	/* If the freeze boost is not active, perform a tick for the ghosts */
	/* Also perform collision checks against player */
	if(state == STATE_PLAYING_BOOST_FREEZE)
		return;
	
	for(int i = 0; i < level_active_ghosts(); ++i)
	{
		entity_ghost_tick(&ghosts[i]);
	
		if(entity_intersects(&player, &ghosts[i]))
			die();
		else if(level_square(&ghosts[i].pos) & LEVEL_SQUARE_PORTAL)
			level_use_portal(&ghosts[i].pos);
	}
	
}




/* If the game is in the PLAYING state; draw all entities, the level aswell as
 * information such as level, difficulty, etc
 */
void game_draw(void)
{       
	level_draw();
	entity_draw(&player);
	for(int i = 0; i < level_active_ghosts(); ++i)
	{
		entity_draw(&ghosts[i]);
	}
	
	
	char buffer[BUTTON_BUFFER_SIZE];
	graphics_print(0, 0, "LEVEL");
	itodsconv(buffer, level);
	graphics_print(0, 1, buffer);
	
	graphics_print(0, 2, "SCORE");
	itodsconv(buffer, score);
	graphics_print(0, 3, buffer);
	
	graphics_print(LEVEL_OFFSET_X + LEVEL_COLUMNS, 0, "MODE");
	graphics_print(LEVEL_OFFSET_X + LEVEL_COLUMNS, 1, difficulty == DIFFICULTY_EASY ? "EASY" : difficulty == DIFFICULTY_MEDIUM ? "MEDIUM" : "HARD");
	
	graphics_print(LEVEL_OFFSET_X + LEVEL_COLUMNS, 2, "LIVES");
	itodsconv(buffer, lives);
	graphics_print(LEVEL_OFFSET_X + LEVEL_COLUMNS, 3, buffer);
}




/* Start a game at level 1, and set the difficulty 
 */
void game_start(int diff)
{
	state = STATE_PLAYING;
	difficulty = diff;

	entity_player_cons(&player);
	lives = GAME_DEFAULT_LIVES - (difficulty - 1) * 2;
	score = 0;
	
	level = 1;
	level_load(level, difficulty);
}




static void respawn(void)
{
	level_player_spawn(&player.pos);
	respawn_ghosts();
}




/* Update the highscores
 */
static void calculate_highscore(void)
{
	for(int i = 0; i < GAME_HIGHSCORES; ++i)
	{
		if(score >= highscores[i])
		{
			for(int j = GAME_HIGHSCORES - 1; j > i; --j)
			{
				highscores[j] = highscores[j-1];
			}
			
			highscores[i] = score;
			break;
		}
	}
}




static void die(void)
{       
	if(--lives == 0)
		gameover();
	else
		interface_menu_load_dead();
}




static void gameover(void)
{
	calculate_highscore();
	interface_menu_load_gameover();
}




/* Advance to the next level
 */
static void advance(void)
{
	if(++level > DEFAULT_LEVELS)
	{
		interface_menu_load_won();
		calculate_highscore();
	}
	else
	{
		interface_menu_load_advance();
		level_load(level, difficulty);
	}
}




/* If the player is on a consumable/booster; pick it up and
 * set the correct state
 */
static void pickup_consumable(void)
{
	int const square = level_square(&player.pos);
	
	if((square & LEVEL_SQUARE_CONSUMABLE_COIN) == LEVEL_SQUARE_CONSUMABLE_COIN)
	{
		score += difficulty;
		
		if(level_take_coin() == 0)
			advance();
	}
	if((square & LEVEL_SQUARE_CONSUMABLE_FREEZE) == LEVEL_SQUARE_CONSUMABLE_FREEZE)
	{
		boost = GAME_BOOST_TICKS;
		state = STATE_PLAYING_BOOST_FREEZE;
		respawn_ghosts();
	}
	if((square & LEVEL_SQUARE_CONSUMABLE_SPEED) == LEVEL_SQUARE_CONSUMABLE_SPEED)
	{
		boost = GAME_BOOST_TICKS;
		state = STATE_PLAYING_BOOST_SPEED;
	}
	
	level_square_empty(&player.pos);
}




static void respawn_ghosts(void)
{
	for(int i = 0; i < level_active_ghosts(); ++i)
	{
		level_ghost_spawn(&ghosts[i].pos);
	}
}




/* Ugly solution that allows menu buttons to interact with the game
 */
void game_start_easy(void)
{
	game_start(DIFFICULTY_EASY);
}

void game_start_medium(void)
{
	game_start(DIFFICULTY_MEDIUM);
}

void game_start_hard(void)
{
	game_start(DIFFICULTY_HARD);
}

/* If the player dies, this is how the game continues */
void game_continue(void)
{
	state = STATE_PLAYING;
	respawn();
}




char *difficulty_to_string(void)
{
	return difficulty == DIFFICULTY_EASY   ? "EASY" 
	     : difficulty == DIFFICULTY_MEDIUM ? "MEDIUM"
	     : "HARD"
	;
}

int game_score(void)
{
	return score;
}

int game_level(void)
{
	return level;
}

int game_lives(void)
{
	return lives;
}
