#ifndef __SPRITES__
#define __SPRITES__

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "entities.h"
// void inicia_sprites_objetos(ALLEGRO_BITMAP* sheet, objetos* obj);
void get_player_sprite(ALLEGRO_BITMAP *spriteSheet, tPlayer *player);
void get_objects_sprite(ALLEGRO_BITMAP* spriteSheet, tObjects* object);
void free_objects_sprite(tMap *map);
void free_player_sprite(tPlayer *player);
#endif