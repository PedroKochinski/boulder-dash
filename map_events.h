#ifndef __MAP_EVENTS__
#define __MAP_EVENTS__
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "entities.h"

void remove_diamond(tObjects *objects, int x, int y);
void is_rock_falling(tMap *map, tSound *sound, long frames);
void is_diamond_falling(tMap *map, tSound *sound, long frames);
void rock_roll_over(tMap *map, int pos_x, int pos_y, int i);
void diamond_roll_over(tMap *map, int pos_x, int pos_y, int i);
int is_game_over(tMap *map, tSound *sound, long frames, int tempo);
void explode(tMap *map, int pos_x, int pos_y);
void remove_explosion_rocks(tObjects *objects, int x, int y);
void remove_explosion_diamonds(tObjects *objects, int x, int y);
void remove_rock(tObjects *objects, int x, int y);
int is_exit_level(tMap *map, tPlayer *player);
void handle_switch_map(tMap *map, tPlayer *player, tSound *sound, int *curr_map, char **wich_map, int *init_score, int *must_exit, bool *skip_level, bool *skip_level_down, ALLEGRO_BITMAP *spriteSheet, int *clock);
#endif