#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef __ALLEGRO_FUNCTIONS__
#define __ALLEGRO_FUNCTIONS__
#define KEY_SEEN 1
#define KEY_RELEASED 2
typedef enum
{
    DOWN,
    LEFT,
    RIGHT,
    UP
} tDirection;


void initialize_allegro();
void must_init(bool test, const char *description);
int game_loop();
int shutdown_allegro();
#endif