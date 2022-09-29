#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#ifndef __SOUND__
#define __SOUND__

typedef struct sound
{
    ALLEGRO_SAMPLE *terrain;
    ALLEGRO_SAMPLE *diamond;
    ALLEGRO_SAMPLE *fall;
    ALLEGRO_SAMPLE *win;
    ALLEGRO_SAMPLE *lose;
    ALLEGRO_SAMPLE *cheat_code;
    ALLEGRO_AUDIO_STREAM *bg_music;
} tSound;

tSound *sound_setup();
ALLEGRO_SAMPLE *get_sound(char *file_name);
void play_sound(ALLEGRO_SAMPLE *sound);
void destroy_sound(tSound *sound);

#endif