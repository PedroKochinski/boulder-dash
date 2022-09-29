#include "sound.h"
#include "allegro_functions.h"

ALLEGRO_SAMPLE *get_sound(char *nome_arquivo)
{
    ALLEGRO_SAMPLE *sound = al_load_sample(nome_arquivo);
    must_init(sound, "sound");
    return sound;
}

tSound *sound_setup()
{
    tSound *sound = (tSound *) malloc(sizeof(tSound));

    sound->terrain = get_sound("resources/audios/terra.ogg");
    sound->diamond = get_sound("resources/audios/diamante.ogg");
    sound->fall = get_sound("resources/audios/fall.ogg");
    sound->win = get_sound("resources/audios/win.wav");
    sound->lose = get_sound("resources/audios/lose.wav");
    sound->cheat_code = get_sound("resources/audios/cheat.wav");
    sound->bg_music = al_load_audio_stream("resources/audios/bg_music.ogg", 2, 2048);
    must_init(sound->bg_music, "musica");
    return sound;
}

void play_sound(ALLEGRO_SAMPLE *sound)
{
    al_play_sample(sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
}

void destroy_sounds(tSound *sound)
{
    al_destroy_sample(sound->terrain);
    al_destroy_sample(sound->diamond);
    al_destroy_sample(sound->fall);
    al_destroy_sample(sound->win);
    al_destroy_sample(sound->lose);
    al_destroy_sample(sound->cheat_code);
    al_destroy_audio_stream(sound->bg_music);
}