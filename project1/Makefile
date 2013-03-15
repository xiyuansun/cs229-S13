OBJS = ./objects/sndcore.o ./objects/sndcs.o ./objects/sndwav.o ./objects/util.o
DEBUG = -g
CFLAGS = -Wall -c
LFLAGS = -Wall
CC = gcc

all: ./build/sndinfo ./build/sndcat

clean:
	rm -rf build debug objects

tar:

build:
	mkdir build

debug:
	mkdir debug

objects:
	mkdir objects

./build/sndinfo: $(OBJS) ./objects/sndinfo.o | build 
	$(CC) $(LFLAGS) $(OBJS) ./objects/sndinfo.o -o ./build/sndinfo

./build/sndcat: $(OBJS) ./objects/sndcat.o | build 
	$(CC) $(LFLAGS) $(OBJS) ./objects/sndcat.o -o ./build/sndcat

./objects/sndinfo.o: sndinfo.c ./core/sndcore.h | objects 
	$(CC) $(CFLAGS) sndinfo.c -o ./objects/sndinfo.o

./objects/sndcat.o: sndcat.c ./core/sndcore.h | objects 
	$(CC) $(CFLAGS) sndcat.c -o ./objects/sndcat.o

./objects/sndcore.o: ./core/sndcore.c ./core/sndcore.h ./core/types.h ./core/sndcs.h ./core/sndwav.h ./core/util.h | objects
	$(CC) $(CFLAGS) core/sndcore.c -o ./objects/sndcore.o

./objects/sndcs.o: ./core/sndcs.c ./core/sndcs.h ./core/sndcore.h ./core/util.h ./core/types.h | objects
	$(CC) $(CFLAGS) core/sndcs.c -o ./objects/sndcs.o

./objects/sndwav.o: ./core/sndwav.c ./core/sndwav.h ./core/types.h | objects 
	$(CC) $(CFLAGS) core/sndwav.c -o ./objects/sndwav.o

./objects/util.o: ./core/util.c ./core/util.h ./core/types.h | objects 
	$(CC) $(CFLAGS) core/util.c -o ./objects/util.o
