#include "entities.h"
#include "sprites.h"
#include "map_events.h"

/*inicia o jogador*/
tPlayer *get_player(ALLEGRO_BITMAP *spriteSheet)
{
  tPlayer *player = (tPlayer *)malloc(sizeof(tPlayer));
  if (!player)
  {
    perror("Malloc error on player\n");
    exit(1);
  }
  player->anim_stop = 0;
  player->anim_l = 0;
  player->anim_r = 0;
  player->isUp = 0;
  player->isDown = 0;
  player->isLeft = 0;
  player->isRight = 0;
  player->isStopped = 0;
  player->diamonds = 0;
  player->score = 0;
  player->godMode = 0;
  player->isAlive = 1;
  player->lifes = 3;
  player->posX = 0;
  player->posY = 0;
  for (int i = 0; i < 8; i++)
    player->code[i] = '-';
  player->code[4] = '\0';
  get_player_sprite(spriteSheet, player);
  return player;
}

/*verifica se tem diamantes*/
int has_diamonds_x(tPlayer *player, tSound *soun, tMap *map, int x, int y, int lado)
{
  if (map->map_matrix[y][x + lado] == DIAMOND)
  {
    remove_diamond(map->objects, y, x + lado);
    map->map_matrix[y][x + lado] = PLAYER;
    map->map_matrix[y][x] = EMPTY;
    play_sound(soun->diamond);
    player->score += 10;
    player->diamonds++;
    return 1;
  }
  return 0;
}

/*verifica se tem diamantes*/
int has_diamonds_y(tPlayer *player, tSound *soun, tMap *map, int x, int y, int lado)
{
  if (map->map_matrix[y + lado][x] == DIAMOND)
  {
    remove_diamond(map->objects, y + lado, x);
    map->map_matrix[y + lado][x] = PLAYER;
    map->map_matrix[y][x] = EMPTY;
    play_sound(soun->diamond);
    player->score += 10;
    player->diamonds++;
    return 1;
  }
  return 0;
}

/*verifica se tem terra*/
int has_terrain_x(tPlayer *player, tSound *soun, tMap *map, int x, int y, int lado)
{
  if (map->map_matrix[y][x + lado] == TERRAIN || map->map_matrix[y][x + lado] == EMPTY)
  {
    if (map->map_matrix[y][x + lado] == TERRAIN)
      play_sound(soun->terrain);
    map->map_matrix[y][x + lado] = PLAYER;
    map->map_matrix[y][x] = EMPTY;
    return 1;
  }
  return 0;
}

/*verifica se tem terra*/
int has_terrain_y(tPlayer *player, tSound *soun, tMap *map, int x, int y, int lado)
{
  if (map->map_matrix[y + lado][x] == TERRAIN || map->map_matrix[y + lado][x] == EMPTY)
  {
    if (map->map_matrix[y + lado][x] == TERRAIN)
      play_sound(soun->terrain);
    map->map_matrix[y + lado][x] = PLAYER;
    map->map_matrix[y][x] = EMPTY;
    return 1;
  }
  return 0;
}

/*empurra a pedra para a esquerda*/
int push_rock_left(tPlayer *player, tSound *soun, tMap *map, long frames, int x, int y)
{
  if (map->map_matrix[y][x - 1] == ROCK && map->map_matrix[y][x - 2] == EMPTY && frames % 10 == 0)
  {
    for (int i = 0; i < map->objects->qte_rock; i++)
    {
      if (map->objects->rock[i].x == y && map->objects->rock[i].y == x - 1)
        map->objects->rock[i].y--;
    }
    map->map_matrix[y][x - 1] = PLAYER;
    map->map_matrix[y][x - 2] = ROCK;
    map->map_matrix[y][x] = EMPTY;
    return 1;
  }
  return 0;
}

/*empurra a pedra para a direita*/
int push_rock_right(tPlayer *player, tSound *soun, tMap *map, long frames, int x, int y)
{
  if (map->map_matrix[y][x + 1] == ROCK && map->map_matrix[y][x + 2] == EMPTY && frames % 10 == 0)
  {
    for (int i = 0; i < map->objects->qte_rock; i++)
    {
      if (map->objects->rock[i].x == y && map->objects->rock[i].y == x + 1)
        map->objects->rock[i].y++;
    }
    map->map_matrix[y][x + 1] = PLAYER;
    map->map_matrix[y][x + 2] = ROCK;
    map->map_matrix[y][x] = EMPTY;
    return 1;
  }
  return 0;
}

/*movimento valido*/
int is_valid(tPlayer *player, tSound *sound, tMap *map, int dir, long frames)
{
  int x, y;
  x = player->posX / SIZE_OBJS;
  y = (player->posY - MARGIN_TOP) / SIZE_OBJS;

  switch (dir)
  {
  case 0:
    if (has_diamonds_x(player, sound, map, x, y, -1))
      return 1;
    else if (has_terrain_x(player, sound, map, x, y, -1))
      return 1;
    if (push_rock_left(player, sound, map, frames, x, y))
      return 1;
    break;
  case 1:
    if (has_diamonds_x(player, sound, map, x, y, 1))
      return 1;
    else if (has_terrain_x(player, sound, map, x, y, 1))
      return 1;
    if (push_rock_right(player, sound, map, frames, x, y))
      return 1;
    break;
  case 2:
    if (has_diamonds_y(player, sound, map, x, y, -1))
      return 1;
    else if (has_terrain_y(player, sound, map, x, y, -1))
      return 1;
    break;
  case 3:
    if (has_diamonds_y(player, sound, map, x, y, 1))
      return 1;
    else if (has_terrain_y(player, sound, map, x, y, 1))
      return 1;
    break;
  }
  return 0;
}

void update_player_left(tPlayer *player, int move)
{
  player->anim_l++;
  player->isLeft = 0;
  if (move)
  {
    player->posX -= VEL;
  }
}

void update_player_right(tPlayer *player, int move)
{
  player->anim_r++;
  player->isRight = 0;
  if (move)
  {
    player->posX += VEL;
  }
}

void update_player_up(tPlayer *player, int move)
{
  player->anim_r++;
  player->isUp = 0;
  if (move)
  {
    player->posY -= VEL;
  }
}

void update_player_down(tPlayer *player, int move)
{
  player->anim_r++;
  player->isDown = 0;
  if (move)
  {
    player->posY += VEL;
  }
}

void update_player(tPlayer *player, int dir, int move)
{
  switch (dir)
  {
  case 0:
    update_player_left(player, move);
    break;
  case 1:
    update_player_right(player, move);
    break;
  case 2:
    update_player_up(player, move);
    break;
  case 3:
    update_player_down(player, move);
    break;
  }
  if (player->anim_l == 7)
    player->anim_l = 0;
  if (player->anim_r == 7)
    player->anim_r = 0;
}

void draw_player(tPlayer *player, tSound *sound, tMap *map, long frames)
{
  int move = 1;

  if (!player->isAlive)
  {
    // Revive jogador após tempo
    if (frames % TEMPO_RESET == 0)
      player->isAlive = 1;
    else
      return;
  }

  // Verifica terreno de acordo com a direção(esq, dir, etc), atualiza posição do player e o desenha na tela
  if (player->isLeft)
  {
    move = is_valid(player, sound, map, 0, frames);
    update_player(player, 0, move);
    al_draw_scaled_bitmap(player->left[player->anim_l], 0, 0, 15, 17, player->posX, player->posY, SIZE_OBJS, SIZE_OBJS, 0);
  }
  else if (player->isRight)
  {
    move = is_valid(player, sound, map, 1, frames);
    update_player(player, 1, move);
    al_draw_scaled_bitmap(player->right[player->anim_r], 0, 0, 15, 16, player->posX, player->posY, SIZE_OBJS, SIZE_OBJS, 0);
  }
  else if (player->isUp)
  {
    move = is_valid(player, sound, map, 2, frames);
    update_player(player, 2, move);
    al_draw_scaled_bitmap(player->right[player->anim_r], 0, 0, 15, 16, player->posX, player->posY, SIZE_OBJS, SIZE_OBJS, 0);
  }
  else if (player->isDown)
  {
    move = is_valid(player, sound, map, 3, frames);
    update_player(player, 3, move);
    al_draw_scaled_bitmap(player->right[player->anim_r], 0, 0, 15, 16, player->posX, player->posY, SIZE_OBJS, SIZE_OBJS, 0);
  }
  else
    al_draw_scaled_bitmap(player->stop[player->anim_stop], 0, 0, 15, 16, player->posX, player->posY, SIZE_OBJS, SIZE_OBJS, 0);
  // De 10 em 10 frames a animação muda
  if (player->isStopped && frames % 3 == 0)
    player->anim_stop++;
  if (player->anim_stop == 7)
    player->anim_stop = 0;
}

void restart_player(tPlayer *player, int init_score)
{
  player->isAlive = 0;
  player->lifes--;
  player->diamonds = 0;
  player->score = init_score;
  player->posX = player->spawnX;
  player->posY = player->spawnY;
}

void get_items_position(tMap *map)
{
  int qte_rock = 0, qte_diamond = 0, i, j;

  for (i = 0; i < map->height; i++)
  {
    for (j = 0; j < map->width; j++)
    {
      if (map->map_matrix[i][j] == ROCK)
      {
        map->objects->rock[qte_rock].x = i;
        map->objects->rock[qte_rock].y = j;
        map->objects->rock[qte_rock].isFalling = 0;
        qte_rock++;
      }
      if (map->map_matrix[i][j] == DIAMOND)
      {
        map->objects->diamond[qte_diamond].x = i;
        map->objects->diamond[qte_diamond].y = j;
        map->objects->diamond[qte_diamond].isFalling = 0;
        qte_diamond++;
      }
    }
  }
}

tMap *get_map(char *file_name, tPlayer *player, ALLEGRO_BITMAP *spriteSheet)
{
  FILE *arq;
  tMap *map = (tMap *)malloc(sizeof(tMap));
  if (!map)
  {
    perror("Malloc error on map\n");
    exit(1);
  }
  tObjects *objects = get_objects(spriteSheet);
  int **map_matrix, i, j, lin, col;
  arq = fopen(file_name, "r");
  if (!arq)
  {
    perror("Couldn't open file\n");
    exit(1);
  }
  fscanf(arq, "%d %d", &col, &lin);
  map->height = lin;
  map->width = col;
  map_matrix = (int **)malloc(lin * sizeof(int *));
  if (!map)
  {
    perror("Malloc error on map_matrix\n");
    exit(1);
  }
  for (i = 0; i < lin; i++)
  {
    map_matrix[i] = (int *)malloc(col * sizeof(int));
    if (!map_matrix[i])
    {
      perror("Malloc error on map_matrix[i]\n");
      exit(1);
    }
  }

  int qte_rock = 0, qte_diamonds = 0;
  for (i = 0; i < lin; i++)
    for (j = 0; j < col; j++)
    {
      fscanf(arq, "%d", &map_matrix[i][j]);
      if (map_matrix[i][j] == ESCAPE)
      {
        map->exitX = i;
        map->exitY = j;
        map_matrix[i][j] = METAL;
      }
      if (map_matrix[i][j] == PLAYER)
      {
        player->spawnX = (i+1) * SIZE_OBJS;
        player->spawnY = ((j)* SIZE_OBJS);
        player->posX = player->spawnX;
        player->posY = player->spawnY;
        map_matrix[i][j] = EMPTY;
      }
      if (map_matrix[i][j] == ROCK)
        qte_rock++;
      if (map_matrix[i][j] == DIAMOND)
        qte_diamonds++;
    }
  objects->qte_rock = qte_rock;
  objects->qte_diamond = qte_diamonds;
  objects->rock = (tItem *)malloc(qte_rock * sizeof(tItem));
  if (!objects->rock)
  {
    perror("Malloc error on objects->rock\n");
    exit(1);
  }
  objects->diamond = (tItem *)malloc(qte_diamonds * sizeof(tItem));
  if (!objects->diamonds)
  {
    perror("Malloc error on objects->diamonds\n");
    exit(1);
  }
  fclose(arq);
  map->map_matrix = map_matrix;
  map->objects = objects;
  get_items_position(map);
  return map;
}

void draw_map(tMap *map, long frames)
{
  int i, j, i_aux, j_aux;
  for (i = 0; i < 22; i++)
  {
    for (j = 0; j < 40; j++)
    {
      i_aux = i * SIZE_OBJS;
      j_aux = j * SIZE_OBJS;
      switch (map->map_matrix[i][j])
      {
      case METAL:
        al_draw_scaled_bitmap(map->objects->metal, 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
        break;
      case TERRAIN:
        al_draw_scaled_bitmap(map->objects->terrain, 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
        break;
      case WALL:
        al_draw_scaled_bitmap(map->objects->wall, 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
        break;
      case ROCK:
        al_draw_scaled_bitmap(map->objects->rock_sprite, 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
        break;
      case EMPTY:
        al_draw_scaled_bitmap(map->objects->empty_sprite, 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
        break;
      case ESCAPE:
        al_draw_scaled_bitmap(map->objects->escape, 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
        break;
      case DIAMOND:
        // Reseta frames do diamante se necessário e faz animação
        if (map->objects->ciclos_diamante == 7)
          map->objects->ciclos_diamante = 0;
        if (frames % 15 == 0)
          map->objects->ciclos_diamante++;
        al_draw_scaled_bitmap(map->objects->diamonds[map->objects->ciclos_diamante], 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
        break;
      case EXPLOSION:
        al_draw_scaled_bitmap(map->objects->explosion[1], 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
        if (frames % 3 == 0)
          map->map_matrix[i][j] = EXPLOSION2;
        break;
      case EXPLOSION2:
        al_draw_scaled_bitmap(map->objects->explosion[2], 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
        if (frames % 3 == 0)
          map->map_matrix[i][j] = EXPLOSION3;
        break;
      case EXPLOSION3:
        al_draw_scaled_bitmap(map->objects->explosion[3], 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
        if (frames % 3 == 0)
          map->map_matrix[i][j] = EMPTY;
        break;
      }
    }
  }
}

tObjects *get_objects(ALLEGRO_BITMAP *spriteSheet)
{
  tObjects *objects = malloc(sizeof(tObjects));
  if (!objects)
  {
    perror("Malloc error on objects\n");
    exit(1);
  }
  get_objects_sprite(spriteSheet, objects);
  objects->ciclos_diamante = 0;
  return objects;
}

void free_map(tMap *map)
{
  int i;
  for (i = 0; i < map->height; i++)
    free(map->map_matrix[i]);
  free(map->map_matrix);
  free(map->objects->rock);
  free(map->objects->diamond);
  free(map->objects);
}
