#include "allegro_functions.h"
#include "sprites.h"
#include "sound.h"
#include "score.h"
#include "map_events.h"

ALLEGRO_TIMER *timer = NULL;
ALLEGRO_TIMER *frameTimer = NULL;
ALLEGRO_EVENT_QUEUE *queue = NULL;
ALLEGRO_DISPLAY *disp = NULL;
ALLEGRO_FONT *font = NULL;
ALLEGRO_BITMAP *spriteSheet;
ALLEGRO_EVENT event;

/*tratamento de erro das funcões da lib allegro*/
void must_init(bool test, const char *description)
{
    if (test)
        return;
    printf("couldn't initialize %s\n", description);
    exit(1);
}

/*inicializa tudo que o jogo precisa para funcionar*/
void initialize_allegro(tPlayer **pPlayer, tMap **pMap, tSound **pSound, tScore **pScore)
{
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");
    must_init(al_install_mouse(), "mouse");
    must_init(al_init_primitives_addon(), "primitives");

    timer = al_create_timer(1.0 / 60.0);
    must_init(timer, "timer");

    frameTimer = al_create_timer(1.0 / 15.0);
    must_init(frameTimer, "frame timer");

    queue = al_create_event_queue();
    must_init(queue, "queue");

    disp = al_create_display(WIDTH, HEIGHT);
    must_init(disp, "display");

    must_init(al_init_font_addon(), "font");
    font = al_create_builtin_font();
    must_init(font, "font");

    must_init(al_init_image_addon(), "image addon");
    spriteSheet = al_load_bitmap("./resources/sprites/spritesheet.png");
    must_init(spriteSheet, "spriteSheet");

    must_init(al_install_audio(), "audio");
    must_init(al_init_acodec_addon(), "audio codecs");
    must_init(al_reserve_samples(16), "reserve samples");

    /*inicializa jogador, mapa pontos e o som do jogo*/
    *pPlayer = get_player(spriteSheet);
    *pMap = get_map(MAP_1, *pPlayer, spriteSheet);
    *pScore = get_score();
    *pSound = sound_setup();
    al_set_audio_stream_playmode((*pSound)->bg_music, ALLEGRO_PLAYMODE_LOOP);
    /*musica de fundo*/
    al_attach_audio_stream_to_mixer((*pSound)->bg_music, al_get_default_mixer());

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_timer_event_source(frameTimer));
}

/*mostra as informações na tela*/
void draw_hud(tPlayer *player, tMap *map, int clock)
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_textf(font, al_map_rgb(255, 255, 255), 250, 10, 0, "%05d", player->score);
    al_draw_bitmap(map->objects->diamonds[0], 0, 8, 0);
    al_draw_textf(font, al_map_rgb(255, 255, 255), 20, 10, 0, "%d/%d", player->diamonds, map->objects->qte_diamond);
    al_draw_textf(font, al_map_rgb(255, 255, 255), 200, 10, 0, "%d", clock);
    al_draw_textf(font, al_map_rgb(255, 255, 255), 100, 10, 0, "Lifes: %d", player->lifes);
    al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 2 - 10, 10, 0, "Help: H/F1");
    if (player->godMode)
        al_draw_textf(font, al_map_rgb(255, 255, 255), 800, 10, 0, "god mode: ON");
    else
        al_draw_textf(font, al_map_rgb(255, 255, 255), 800, 10, 0, "god mode: OFF");
}

/*mostra a tela de fim de jogo*/
void draw_end_game(tPlayer *player, tScore *score)
{
    al_draw_filled_rectangle(3 * SIZE_OBJS, 2 * SIZE_OBJS, WIDTH - 3 * SIZE_OBJS, HEIGHT - 1 * SIZE_OBJS, al_map_rgba_f(0, 0, 0, 0.9));
    al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 4 + 7 * SIZE_OBJS, 20 + 2 * SIZE_OBJS, 0, "GAME OVER");
    al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 4 + 7 * SIZE_OBJS, 100 + 2 * SIZE_OBJS, 0, "SCORE: %d", player->score);
    al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 4 + 7 * SIZE_OBJS, 160 + 2 * SIZE_OBJS, 0, "Leader Board");
    for (int i = 0; i < 5; i++)
        al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 4 + 7 * SIZE_OBJS, 220 + (i * 40) + 2 * SIZE_OBJS, 0, "%d: %d score", i + 1, score->score[i]);
    al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 4 + 5 * SIZE_OBJS, 500 + 2 * SIZE_OBJS, 0, "ESC to exit");
    al_flip_display();
}

int end_game(tPlayer *player, tScore *score, tMap *map, unsigned char *key)
{
    bool done = false;
    save_score(player->score, score);
    al_flush_event_queue(queue);
    while (1)
    {
        al_wait_for_event(queue, &event);
        if (al_is_event_queue_empty(queue) && event.timer.source == timer)
            draw_end_game(player, score);
        switch (event.type)
        {
        case ALLEGRO_EVENT_KEY_DOWN:
            key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
            break;
        case ALLEGRO_EVENT_KEY_UP:
            key[event.keyboard.keycode] &= KEY_RELEASED;
            break;
        }
        // Ao pressionar ESC termina o jogo
        if (key[ALLEGRO_KEY_ESCAPE])
        {
            key[ALLEGRO_KEY_ESCAPE] = 0;
            free_objects_sprite(map);
            free_map(map);
            free_player_sprite(player);
            free(player);
            free(score);
            shutdown_allegro();
            done = true;
        }
        if (done)
            break;
    }
    return 0;
}

/*mostra as instruções na tela para o jogador*/
void draw_instructions(unsigned char *key)
{
    bool done = false;
    al_flush_event_queue(queue);
    while (1)
    {
        al_wait_for_event(queue, &event);
        if (al_is_event_queue_empty(queue) && event.timer.source == timer)
        {
            al_draw_filled_rectangle(3 * SIZE_OBJS, 2 * SIZE_OBJS, WIDTH - 3 * SIZE_OBJS, HEIGHT - 1 * SIZE_OBJS, al_map_rgba_f(0, 0, 0, 0.9));
            al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 4 + 7 * SIZE_OBJS, 20 + 2 * SIZE_OBJS, 0, "I N S T R U C O E S");
            al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 4, 80 + 2 * SIZE_OBJS, 0, "Colete TODOS os diamantes para avançar de nível");
            al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 4, 100 + 2 * SIZE_OBJS, 0, "Cuidado com as pedras e com o tempo, ambos podem");
            al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 4, 120 + 2 * SIZE_OBJS, 0, "fazê-lo perder.");
            al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 4, 160 + 2 * SIZE_OBJS, 0, "Voce pode se mover pelo mapa utilizando as setas do teclado");
            al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 4, 180 + 2 * SIZE_OBJS, 0, "ou usando as teclas WASD.");
            al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 4, 200 + 2 * SIZE_OBJS, 0, "Para desistir da partida, aperte ESC.");
            al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 4, 220 + 2 * SIZE_OBJS, 0, "Ao utilizar o cheat code 'puft' seu personagem fica invencivel");
            al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 4, 240 + 2 * SIZE_OBJS, 0, "e consegue destruir o mapa ao seu redor, use-o com cautela.");
            al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 4, 280 + 2 * SIZE_OBJS, 0, "As teclas PGUP e PGDOWN mudam de nível");
            al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 4, 300 + 2 * SIZE_OBJS, 0, "Você ganha uma vida sempre que consegue avançar de nível");
            al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 4, 340 + 2 * SIZE_OBJS, 0, "O projeto foi desenvolvido para a disciplina de Programação 2");
            al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 4, 360 + 2 * SIZE_OBJS, 0, "pelo aluno Pedro Henrique Kochinski");
            al_flip_display();
        }
        switch (event.type)
        {
        case ALLEGRO_EVENT_KEY_DOWN:
            key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
            break;
        case ALLEGRO_EVENT_KEY_UP:
            key[event.keyboard.keycode] &= KEY_RELEASED;
            break;
        }
        if (key[ALLEGRO_KEY_ESCAPE])
        {
            key[ALLEGRO_KEY_ESCAPE] = 0;
            done = true;
        }
        if (done)
            break;
    }
}

/*pega os eventos do keyboard*/
void get_keyboard_events(unsigned char *key, bool *done, bool *redraw, long *frames, tPlayer *player, tMap *map, tScore *score, bool *skip_level, bool *skip_level_down, int *clock)
{
    if (event.timer.source == timer)
    {
        if (key[ALLEGRO_KEY_UP] || key[ALLEGRO_KEY_W])
        {
            if ((player->posY - MARGIN_TOP - SIZE_OBJS) > 0)
            {
                player->isUp = 1;
                player->isStopped = 0;
            }
        }
        else if (key[ALLEGRO_KEY_DOWN] || key[ALLEGRO_KEY_S])
        {
            if (player->posY < HEIGHT - MARGIN_TOP - SIZE_OBJS)
            {
                player->isDown = 1;
                player->isStopped = 0;
            }
        }
        else if (key[ALLEGRO_KEY_LEFT] || key[ALLEGRO_KEY_A])
        {
            if (player->posX > SIZE_OBJS)
            {
                player->isLeft = 1;
                player->isStopped = 0;
            }
        }
        else if (key[ALLEGRO_KEY_RIGHT] || key[ALLEGRO_KEY_D])
        {
            if (player->posX < WIDTH - 2 * SIZE_OBJS)
            {
                player->isRight = 1;
                player->isStopped = 0;
            }
        }
        else if ((key[ALLEGRO_KEY_PGUP]))
            *skip_level = true;
        else if(key[ALLEGRO_KEY_PGDN])
            *skip_level_down = true;
        else if ((key[ALLEGRO_KEY_H] || key[ALLEGRO_KEY_F1]))
            draw_instructions(key);
        else if (key[ALLEGRO_KEY_P])
            player->code[0] = 'p';
        else if (key[ALLEGRO_KEY_U])
            player->code[1] = 'u';
        else if (key[ALLEGRO_KEY_F])
            player->code[2] = 'f';
        else if (key[ALLEGRO_KEY_T])
            player->code[3] = 't';
        else if (key[ALLEGRO_KEY_ESCAPE])
        {
            key[ALLEGRO_KEY_ESCAPE] = 0;
            end_game(player, score, map, key);
            *done = true;
        }
        for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
            key[i] &= KEY_SEEN;
        player->isStopped = 1;
        if (*frames % 60 == 1)
            (*clock)--;
        *redraw = true;
        *frames += 1;
    }
}

void open_exit(tMap *map, tPlayer *player, int must_exit)
{
    if (player->diamonds == map->objects->qte_diamond || must_exit)
        map->map_matrix[map->exitX][map->exitY] = ESCAPE;
}

void cheat_code_event(tMap *map, tPlayer *player)
{
    int y = (player->posX) / SIZE_OBJS;
    int x = (player->posY - 2 * MARGIN_TOP) / SIZE_OBJS;
    remove_explosion_rocks(map->objects, x, y);
    remove_explosion_diamonds(map->objects, x, y);
    explode(map, x, y);
}

int game_loop()
{
    int curr_map = 1;
    char *wich_map = MAP_1;
    tPlayer *player = NULL;
    tMap *map = NULL;
    tSound *sound = NULL;
    tScore *score = NULL;
    initialize_allegro(&player, &map, &sound, &score);

    bool done = false;
    bool redraw = true;
    bool skip_level = false;
    bool skip_level_down = false;
    unsigned char key[ALLEGRO_KEY_MAX];
    memset(key, 0, sizeof(key));
    long frames = 0;
    int died = 0;
    int must_exit = 0;
    char *cheat_code = "puft";
    int init_score = 0;
    al_start_timer(timer);
    al_start_timer(frameTimer);
    int clock = 200;
    while (1)
    {
        al_wait_for_event(queue, &event);

        switch (event.type)
        {
        case ALLEGRO_EVENT_TIMER:
            get_keyboard_events(key, &done, &redraw, &frames, player, map, score, &skip_level, &skip_level_down, &clock);
            open_exit(map, player, must_exit);
            if(frames % 15 == 0)
                handle_switch_map(map, player, sound, &curr_map, &wich_map, &init_score, &must_exit, &skip_level, &skip_level_down, spriteSheet, &clock);
            if (event.timer.source == frameTimer && al_is_event_queue_empty(queue) && redraw)
            {
                draw_hud(player, map, clock);
                draw_map(map, frames);
                draw_player(player, sound, map, frames);
                is_rock_falling(map, sound, frames);
                is_diamond_falling(map, sound, frames);
                al_flip_display();
                if (!strcmp(player->code, cheat_code))
                {
                    play_sound(sound->cheat_code);
                    player->score += 500;
                    player->godMode = 1;
                    must_exit = 1;
                    cheat_code_event(map, player);
                    for (int i = 0; i < 4; i++)
                        player->code[i] = '-';
                }
                redraw = false;
            }
            if (!player->godMode)
                died = is_game_over(map, sound, frames, 0);
            if (died)
                restart_player(player, init_score);
            if (!player->isAlive && frames % TEMPO_RESET == 0)
                map = get_map(wich_map, player, spriteSheet);
            
            break;
        case ALLEGRO_EVENT_KEY_DOWN:
            key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
            break;
        case ALLEGRO_EVENT_KEY_UP:
            key[event.keyboard.keycode] &= KEY_RELEASED;
            break;
        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
            if (player->godMode)
                cheat_code_event(map, player);
            break;
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            done = true;
            break;
        }
        if(curr_map > 10){
            done = true;
            break;
        }
        if (player->lifes == 0 || clock == 0){
            play_sound(sound->lose);
            break;
        }
        if(done)
            break;
    }
    end_game(player, score, map, key);
    return 0;
}

int shutdown_allegro()
{
    al_destroy_bitmap(spriteSheet);
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_timer(frameTimer);
    al_destroy_event_queue(queue);
    exit(1);
}
