all: lol
allegro_utils.o: allegro_utils.c allegro_utils.h
		g++  allegro_utils.c allegro_utils.h -o allegro_utils.o
config.o: config.c config.h
		g++  config.c config.h -o config.o
main.o: main.c gamestates/menu.h gamestates/../main.h \
 gamestates/../allegro_utils.h gamestates/loading.h gamestates/about.h \
 gamestates/intro.h gamestates/map.h gamestates/level.h \
 gamestates/pause.h gamestates/disclaimer.h config.h
		g++  main.c gamestates/menu.h gamestates/../main.h \
 gamestates/loading.h gamestates/about.h \
 gamestates/map.h gamestates/level.h \
 gamestates/disclaimer.h config.h -o main.o
lol: allegro_utils.o .oonfig.o  main.o
		g++ allegro_utils.o .oonfig.o  main.o -o lolclean:
		rm *.o 
		rm lol