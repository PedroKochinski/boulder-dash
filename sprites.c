#include "sprites.h"

void get_player_sprite(ALLEGRO_BITMAP *spriteSheet, tPlayer *player)
{
  player->stop[0] = al_create_sub_bitmap(spriteSheet, 0, 0, 15, 16);
  for (int i = 1; i <= 6; i++)
    player->stop[i] = al_create_sub_bitmap(spriteSheet, (16 * i), 0, 15, 16);

  player->left[0] = al_create_sub_bitmap(spriteSheet, 0, 16, 15, 16);
  for (int i = 1; i <= 6; i++)
    player->left[i] = al_create_sub_bitmap(spriteSheet, (16 * i), 16, 15, 16);

  player->right[0] = al_create_sub_bitmap(spriteSheet, 0, 32, 15, 16);
  for (int i = 1; i <= 6; i++)
    player->right[i] = al_create_sub_bitmap(spriteSheet, (16 * i), 32, 15, 16);
}

void get_objects_sprite(ALLEGRO_BITMAP *spriteSheet, tObjects *object)
{
  object->diamonds[0] = al_create_sub_bitmap(spriteSheet, 0, 64, 15, 16);
  object->diamonds[2] = al_create_sub_bitmap(spriteSheet, 0, 80, 15, 16);
  object->diamonds[4] = al_create_sub_bitmap(spriteSheet, 0, 96, 15, 16);
  object->diamonds[6] = al_create_sub_bitmap(spriteSheet, 0, 112, 15, 16);
  object->diamonds[1] = al_create_sub_bitmap(spriteSheet, 16, 64, 15, 16);
  object->diamonds[3] = al_create_sub_bitmap(spriteSheet, 16, 80, 15, 16);
  object->diamonds[5] = al_create_sub_bitmap(spriteSheet, 16, 96, 15, 16);
  object->diamonds[7] = al_create_sub_bitmap(spriteSheet, 16, 112, 15, 16);

  object->explosion[3] = al_create_sub_bitmap(spriteSheet, 96, 48, 15, 16);
  object->explosion[2] = al_create_sub_bitmap(spriteSheet, 112, 64, 15, 16);
  object->explosion[1] = al_create_sub_bitmap(spriteSheet, 128, 64, 15, 16);
  object->explosion[0] = al_create_sub_bitmap(spriteSheet, 128, 80, 15, 16);

  object->metal = al_create_sub_bitmap(spriteSheet, 0, 48, 15, 16);
  object->escape = al_create_sub_bitmap(spriteSheet, 16, 48, 15, 16);
  object->wall = al_create_sub_bitmap(spriteSheet, 32, 48, 15, 16);
  object->terrain = al_create_sub_bitmap(spriteSheet, 48, 48, 15, 16);
  object->rock_sprite = al_create_sub_bitmap(spriteSheet, 80, 48, 15, 16);
  object->empty_sprite = al_create_sub_bitmap(spriteSheet, 96, 48, 15, 16);
}

void free_objects_sprite(tMap *map)
{
    al_destroy_bitmap(map->objects->metal);
    al_destroy_bitmap(map->objects->escape);
    al_destroy_bitmap(map->objects->wall);
    al_destroy_bitmap(map->objects->terrain);
    al_destroy_bitmap(map->objects->rock_sprite);
    al_destroy_bitmap(map->objects->empty_sprite);

    for (int i = 0; i < 4; i++)
        al_destroy_bitmap(map->objects->explosion[i]);

    for (int i = 0; i < 8; i++)
        al_destroy_bitmap(map->objects->diamonds[i]);
}

void free_player_sprite(tPlayer *player)
{
    for (int i = 0; i < 7; i++)
        al_destroy_bitmap(player->stop[i]);

    for (int i = 0; i < 7; i++)
        al_destroy_bitmap(player->left[i]);

    for (int i = 0; i < 7; i++)
        al_destroy_bitmap(player->right[i]);
}