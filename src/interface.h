
/* interface.h */

#pragma once



void interface_draw(void);
void interface_button_prev(void);
void interface_button_next(void);
void interface_button_press(void);

/* This is a very ugly solution, but it allows menu buttons to
 * interact more efficiently using function pointers
 */
void interface_menu_load_main(void);
void interface_menu_load_highscores(void);
void interface_menu_load_help(void);
void interface_menu_load_difficulty(void);
void interface_menu_load_paused(void);
void interface_menu_load_dead(void);
void interface_menu_load_gameover(void);
void interface_menu_load_advance(void);
void interface_menu_load_won(void);
