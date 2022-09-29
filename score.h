#ifndef __SCORE__
#define __SCORE__

#include "allegro_functions.h"
#define SCORE_SIZE 5
#define SCORE_PATH "resources/scores/score.txt"

typedef struct score{
  int score[SCORE_SIZE];
  int tam;
}tScore;

tScore* get_score();
void save_score(int player_score, tScore* score);

#endif