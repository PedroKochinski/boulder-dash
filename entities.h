#ifndef __ENTITIES__
#define __ENTITIES__
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "sound.h"
#define MAP_1 "./resources/maps/mapa1.txt"
#define MAP_2 "./resources/maps/mapa2.txt"
#define MAP_3 "./resources/maps/mapa3.txt"
#define MAP_4 "./resources/maps/mapa4.txt"
#define MAP_5 "./resources/maps/mapa5.txt"
#define MAP_6 "./resources/maps/mapa6.txt"
#define MAP_7 "./resources/maps/mapa7.txt"
#define MAP_8 "./resources/maps/mapa8.txt"
#define MAP_9 "./resources/maps/mapa9.txt"
#define MAP_10 "./resources/maps/mapa10.txt"


#define VEL 30
#define METAL 0
#define TERRAIN 1
#define WALL 2
#define ROCK 3
#define DIAMOND 4
#define EMPTY 5
#define PLAYER 6
#define EXPLOSION 7
#define EXPLOSION2 8
#define EXPLOSION3 9
#define ESCAPE 10
#define TEMPO_RESET 80
#define SIZE_OBJS 30
#define MARGIN_TOP 30
#define WIDTH (40 * SIZE_OBJS)
#define HEIGHT ((22 * SIZE_OBJS) + MARGIN_TOP)

typedef struct player
{
  ALLEGRO_BITMAP *stop[7];
  ALLEGRO_BITMAP *left[7];
  ALLEGRO_BITMAP *right[7];
  int isUp, isDown, isLeft, isRight, isStopped;
  int anim_stop, anim_l, anim_r;
  int isAlive, lifes;
  int posX, posY;
  int spawnX, spawnY;
  int godMode;
  int diamonds;
  int score;
  char code[4];
} tPlayer;

typedef struct item
{
  int isFalling;
  int x, y;
} tItem;

typedef struct objects
{
  tItem *rock;
  tItem *diamond;
  ALLEGRO_BITMAP *terrain;
  ALLEGRO_BITMAP *wall;
  ALLEGRO_BITMAP *metal;
  ALLEGRO_BITMAP *rock_sprite;
  ALLEGRO_BITMAP *empty_sprite;
  ALLEGRO_BITMAP *escape;
  ALLEGRO_BITMAP *diamonds[8];
  ALLEGRO_BITMAP *explosion[4];
  int qte_rock, qte_diamond;
  int ciclos_diamante;
} tObjects;

typedef struct map
{
  int **map_matrix;
  int height;
  int width;
  int exitX;
  int exitY;
  tObjects *objects;
} tMap;

typedef struct enemy{
  ALLEGRO_BITMAP *enemy_sprite[7];
  int posX, posY;
} tEnemy;

tPlayer *get_player(ALLEGRO_BITMAP *spriteSheet);

void restart_player(tPlayer *player, int init_score);

void draw_player(tPlayer *player, tSound *sound, tMap *map, long frames);

tMap *get_map(char *file_name, tPlayer *player, ALLEGRO_BITMAP *spriteSheet);

tObjects *get_objects(ALLEGRO_BITMAP *spriteSheet);

void draw_map(tMap *map, long frames);
void update_player(tPlayer *player, int dir, int move);
void free_map(tMap *map);
#endif