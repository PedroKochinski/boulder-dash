#include "score.h"
#include <stdio.h>

/*pega o arquivo de pontuacao*/
tScore *get_score()
{
    FILE *arq;
    tScore *score = (tScore*) malloc(sizeof(tScore));
    if (!score)
    {
        perror("Malloc error on score\n");
        exit(1);
    }
    score->tam = SCORE_SIZE;
    arq = fopen(SCORE_PATH, "r");
    if (!arq)
    {
        perror("Couldn't open file\n");
        exit(1);
    }
    for (int i = 0; i < SCORE_SIZE && !feof(arq); i++)
        fscanf(arq, "%d", &score->score[i]);

    fclose(arq);
    return score;
}

/*salva a pontuacao no arquivo*/
void save_score(int player_score, tScore *score)
{
    FILE *arq;
    arq = fopen(SCORE_PATH, "w");
    if (!arq)
    {
        perror("Couldn't open file\n");
        exit(1);
    }
    // Insere a pontuação do jogador no vetor de pontuação
    int i = score->tam - 1;
    while (player_score > score->score[i - 1] && i > 0)
    {
        i--;
        score->score[i + 1] = score->score[i];
    }
    int pos = i;
    score->score[pos] = player_score;

    fprintf(arq, "%d", score->score[0]);
    for (int i = 1; i < score->tam; i++)
        fprintf(arq, "\n%d", score->score[i]);

    fclose(arq);
}