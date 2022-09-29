#include <stdio.h>
#include <stdlib.h>
#include "entities.h"
#include "sprites.h"
#include "map_events.h"

void remove_diamond(tObjects *objects, int x, int y)
{
	for (int i = 0; i < objects->qte_diamond; i++)
	{
		if (objects->diamond[i].x == x && objects->diamond[i].y == y)
		{
			objects->diamond[i].x = 0;
			objects->diamond[i].y = 0;
			objects->diamond[i].isFalling = 0;
		}
	}
}

void is_rock_falling(tMap *map, tSound *sound, long frames)
{
	/*Não deixa o desabamento acontecer rapidamente*/
	if (frames % 3 != 0)
		return;
	int posX, posY;
	for (int i = 0; i < map->objects->qte_rock; i++)
	{
		posX = map->objects->rock[i].x;
		posY = map->objects->rock[i].y;
		rock_roll_over(map, posX, posY, i);
		/*verifica se a pedra já chegou no chão*/
		if (map->objects->rock[i].isFalling == 1)
		{
			if (map->map_matrix[posX + 1][posY] != EMPTY && map->map_matrix[posX + 1][posY] != PLAYER && map->map_matrix[posX + 1][posY])
			{
				play_sound(sound->fall);
				map->objects->rock[i].isFalling = 0;
			}
		}
		/*faz o desabamento da pedra*/
		if (map->map_matrix[posX + 1][posY] == EMPTY && (posX + 1 < 21))
		{
			map->objects->rock[i].isFalling = 1;
			map->objects->rock[i].x++;
			map->map_matrix[posX + 1][posY] = ROCK;
			map->map_matrix[posX][posY] = EMPTY;
		}
	}
}

void is_diamond_falling(tMap *map, tSound *sound, long frames)
{
	if (frames % 3 != 0)
		return;
	int posX, posY;
	for (int i = 0; i < map->objects->qte_diamond; i++)
	{
		posX = map->objects->diamond[i].x;
		posY = map->objects->diamond[i].y;
		diamond_roll_over(map, posX, posY, i);
		if (map->objects->diamond[i].isFalling == 1)
		{
			if (map->map_matrix[posX + 1][posY] != EMPTY && map->map_matrix[posX + 1][posY] != PLAYER && map->map_matrix[posX + 1][posY] != EXPLOSION)
			{
				play_sound(sound->fall);
				map->objects->diamond[i].isFalling = 0;
			}
		}
		if (map->map_matrix[posX + 1][posY] == EMPTY && (posX + 1 < 21))
		{
			map->objects->diamond[i].isFalling = 1;
			map->objects->diamond[i].x++;
			map->map_matrix[posX + 1][posY] = DIAMOND;
			map->map_matrix[posX][posY] = EMPTY;
		}
	}
}

/*faz o desmoronamento para os lados*/
void rock_roll_over(tMap *map, int posX, int posY, int i)
{
	if ((map->map_matrix[posX + 1][posY] == ROCK || map->map_matrix[posX + 1][posY] == DIAMOND) &&
		(map->map_matrix[posX - 1][posY] != ROCK || map->map_matrix[posX - 1][posY] != DIAMOND))
	{
		if (map->map_matrix[posX][posY + 1] == EMPTY && map->map_matrix[posX + 1][posY + 1] == EMPTY)
		{
			map->objects->rock[i].y++;
			map->map_matrix[posX][posY + 1] = ROCK;
			map->map_matrix[posX][posY] = EMPTY;
		}
		else if (map->map_matrix[posX][posY - 1] == EMPTY && map->map_matrix[posX + 1][posY - 1] == EMPTY)
		{
			map->objects->rock[i].y--;
			map->map_matrix[posX][posY - 1] = ROCK;
			map->map_matrix[posX][posY] = EMPTY;
		}
	}
}

void diamond_roll_over(tMap *map, int posX, int posY, int i)
{
	if ((map->map_matrix[posX + 1][posY] == ROCK || map->map_matrix[posX + 1][posY] == DIAMOND) &&
		(map->map_matrix[posX - 1][posY] != ROCK || map->map_matrix[posX - 1][posY] != DIAMOND))
	{
		if (map->map_matrix[posX][posY + 1] == EMPTY && map->map_matrix[posX + 1][posY + 1] == EMPTY)
		{
			map->objects->diamond[i].y++;
			map->map_matrix[posX][posY + 1] = DIAMOND;
			map->map_matrix[posX][posY] = EMPTY;
		}
		else if (map->map_matrix[posX][posY - 1] == EMPTY && map->map_matrix[posX + 1][posY - 1] == EMPTY)
		{
			map->objects->diamond[i].y--;
			map->map_matrix[posX][posY - 1] = DIAMOND;
			map->map_matrix[posX][posY] = EMPTY;
		}
	}
}

/*verifica se o jogador morreu*/
int is_game_over(tMap *map, tSound *sound, long frames, int tempo)
{
	if (frames % 10 != 0)
		return 0;

	int posX, posY;
	/*verifica se alguma pedra acertou o jogador*/
	for (int i = 0; i < map->objects->qte_rock; i++)
	{
		posX = map->objects->rock[i].x;
		posY = map->objects->rock[i].y;
		if (map->objects->rock[i].isFalling == 1)
		{
			if (map->map_matrix[posX + 1][posY] == PLAYER)
			{
				play_sound(sound->fall);
				remove_explosion_rocks(map->objects, posX + 1, posY);
				explode(map, posX, posY);
				map->objects->rock[i].y = 0;
				map->objects->rock[i].x = 0;
				map->objects->rock[i].isFalling = 0;
				return 1;
			}
		}
	}
	return 0;
}

/*verifica saidas*/

int is_exit_y(tPlayer* player, tMap *map, int x, int y, int lado, int direcao){
  if(map->map_matrix[y + lado][x] == ESCAPE){
  	map->map_matrix[y + lado][x] = PLAYER;
  	map->map_matrix[y][x] = EMPTY;
  	update_player(player, direcao, 1);
  	return 1;
  }
  return 0;
}

int is_exit_x(tPlayer* player, tMap *map, int x, int y, int lado, int direcao){
  if(map->map_matrix[y][x + lado] == ESCAPE){
  	map->map_matrix[y][x + lado] = PLAYER;
  	map->map_matrix[y][x] = EMPTY;
  	update_player(player, direcao, 1);
  	return 1;
  }
  return 0;
}

/*verifica se o jogador está na saída*/
int is_exit_level(tMap *map, tPlayer *player){
  int x, y;
  x = player->posX / SIZE_OBJS;
  y = (player->posY - MARGIN_TOP) /SIZE_OBJS;
  if(map->exitX == x &&  map->exitY == y)
  	return 1;
  else if(player->isRight){
  	if(is_exit_x(player, map, x, y, 1, 1))
  	  return 1;
  }
  else if(player->isUp){
  	if(is_exit_y(player, map, x, y, -1, 2))
  	  return 1;
  }
  else if(player->isDown){
  	if(is_exit_y(player, map, x, y, 1, 3))
  	  return 1;
  }
  return 0;
}

/*faz a explosão dos itens ao redor*/
void explode(tMap *map, int posX, int posY)
{
	map->map_matrix[posX + 1][posY] = EXPLOSION;
	map->map_matrix[posX][posY] = EXPLOSION;
	map->map_matrix[posX][posY + 1] = EXPLOSION;
	map->map_matrix[posX][posY - 1] = EXPLOSION;
	map->map_matrix[posX + 1][posY + 1] = EXPLOSION;
	map->map_matrix[posX + 1][posY - 1] = EXPLOSION;
	map->map_matrix[posX + 2][posY - 1] = EXPLOSION;
	map->map_matrix[posX + 2][posY] = EXPLOSION;
	map->map_matrix[posX + 2][posY + 1] = EXPLOSION;
}

/*destroi todas as pedras na explosão*/
void remove_explosion_rocks(tObjects *objects, int x, int y)
{
	remove_rock(objects, x - 1, y - 1);
	remove_rock(objects, x - 1, y);
	remove_rock(objects, x - 1, y + 1);
	remove_rock(objects, x, y - 1);
	remove_rock(objects, x, y + 1);
	remove_rock(objects, x + 1, y - 1);
	remove_rock(objects, x + 1, y);
	remove_rock(objects, x + 1, y + 1);
}

/*destroi todas os diamantes na explosão*/
void remove_explosion_diamonds(tObjects *objects, int x, int y)
{
	remove_diamond(objects, x - 1, y - 1);
	remove_diamond(objects, x - 1, y);
	remove_diamond(objects, x - 1, y + 1);
	remove_diamond(objects, x, y - 1);
	remove_diamond(objects, x, y + 1);
	remove_diamond(objects, x + 1, y - 1);
	remove_diamond(objects, x + 1, y);
	remove_diamond(objects, x + 1, y + 1);
}

/*remove a pedra*/
void remove_rock(tObjects *objects, int x, int y)
{
	for (int i = 0; i < objects->qte_rock; i++)
	{
		if (objects->rock[i].x == x && objects->rock[i].y == y)
		{
			objects->rock[i].x = 0;
			objects->rock[i].y = 0;
			objects->rock[i].isFalling = 0;
		}
	}
}

/*faz a troca de mapas*/
void handle_switch_map(tMap *map, tPlayer *player, tSound *sound, int *curr_map, char **wich_map, int *init_score, int *must_exit, bool *skip_level, bool *skip_level_down, ALLEGRO_BITMAP *spriteSheet, int *clock){
    /*o jogador pode mudar de nível ao pressionar as teclas PGUP, PGDOWN ou se está na saída */
	int exit = is_exit_level(map, player);
	if ((*skip_level == true || *skip_level_down == true) || exit)
        {
			if(*skip_level || exit){
				(*curr_map)++;
				player->lifes++;
			}
			else if(*skip_level_down){
				(*curr_map)--;
			}
            *skip_level = false;
			*skip_level_down = false;
			*clock = 200;
            play_sound(sound->win);
            switch (*curr_map)
            {
            case 1:
				*init_score = player->score;
                *must_exit = 0;
                *wich_map = MAP_1;
                restart_player(player, *init_score);
                map = get_map(*wich_map, player, spriteSheet);
                break;
            case 2:
                *init_score = player->score;
                *must_exit = 0;
                *wich_map = MAP_2;
                restart_player(player, *init_score);
                map = get_map(*wich_map, player, spriteSheet);
                break;
            case 3:
                *init_score = player->score;
                *must_exit = 0;
                *wich_map = MAP_3;
                restart_player(player, *init_score);
                map = get_map(*wich_map, player, spriteSheet);
                break;
            case 4:
                *init_score = player->score;
                *must_exit = 0;
                *wich_map = MAP_4;
                restart_player(player, *init_score);
                map = get_map(*wich_map, player, spriteSheet);
                break;
            case 5:
                *init_score = player->score;
                *must_exit = 0;
                *wich_map = MAP_5;
                restart_player(player, *init_score);
                map = get_map(*wich_map, player, spriteSheet);
                break;
            case 6:
                *init_score = player->score;
                *must_exit = 0;
                *wich_map = MAP_6;
                restart_player(player, *init_score);
                map = get_map(*wich_map, player, spriteSheet);
                break;
            case 7:
                *init_score = player->score;
                *must_exit = 0;
                *wich_map = MAP_7;
                restart_player(player, *init_score);
                map = get_map(*wich_map, player, spriteSheet);
                break;
            case 8:
                *init_score = player->score;
                *must_exit = 0;
                *wich_map = MAP_8;
                restart_player(player, *init_score);
                map = get_map(*wich_map, player, spriteSheet);
                break;
            case 9:
                *init_score = player->score;
                *must_exit = 0;
                *wich_map = MAP_9;
                restart_player(player, *init_score);
                map = get_map(*wich_map, player, spriteSheet);
                break;
            case 10:
                *init_score = player->score;
                *must_exit = 0;
                *wich_map = MAP_10;
                restart_player(player, *init_score);
                map = get_map(*wich_map, player, spriteSheet);
                break;
            default:
                break;
            }
        }
}