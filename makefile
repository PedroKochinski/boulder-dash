LDLIBS = -L/usr/local/lib -lallegro -lallegro_font -lallegro_primitives -lallegro_audio -lallegro_acodec -lallegro_image
INCLUDE = main.c allegro_functions.c allegro_functions.h sprites.c sprites.h sound.c sound.h entities.c entities.h score.c score.h map_events.c map_events.h
CFLAG = -Wall -g
OBJS = boulderdash.o

all: boulderdash

boulderdash: $(INCLUDE)
	gcc $(CFLAG) -o boulderdash $(INCLUDE) $(LDLIBS)

clean:
	-rm -f $(OBJS) *~

purge: clean
	-rm -f boulderdash