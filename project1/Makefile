BOBJS = build/objects/sndcore.o build/objects/sndcs.o build/objects/sndwav.o build/objects/util.o
DOBJS = debug/objects/sndcore.o debug/objects/sndcs.o debug/objects/sndwav.o debug/objects/util.o
DEBUG = -g
CFLAGS = -Wall -c
LFLAGS = -Wall
CC = gcc

all: sndinfo sndcat sndchan

sndinfo: build/sndinfo
	@

sndcat: build/sndcat
	@

sndchan: build/sndchan
	@

bug: debug/sndinfo debug/sndcat debug/sndchan

clean:
	rm -rf build debug

tar:

build:
	mkdir build

debug:
	mkdir debug

build/objects: | build
	mkdir build/objects

debug/objects: | debug
	mkdir debug/objects

#Links sndinfo
build/sndinfo: $(BOBJS) build/objects/sndinfo.o | build 
	$(CC) $(LFLAGS) $(BOBJS) build/objects/sndinfo.o -o build/sndinfo -lm

#Links debug version of sndinfo
debug/sndinfo: $(DOBJS) debug/objects/sndinfo.o | debug
	$(CC) $(LFLAGS) $(DEBUG) $(DOBJS) debug/objects/sndinfo.o -o debug/sndinfo -lm

#Links sndcat
build/sndcat: $(BOBJS) build/objects/sndcat.o | build
	$(CC) $(LFLAGS) $(BOBJS) build/objects/sndcat.o -o build/sndcat -lm

#Links debug version of sndcat
debug/sndcat: $(DOBJS) debug/objects/sndcat.o | debug
	$(CC) $(LFLAGS) $(DEBUG) $(DOBJS) debug/objects/sndcat.o -o debug/sndcat -lm

#Links sndchan
build/sndchan: $(BOBJS) build/objects/sndchan.o | build
	$(CC) $(LFLAGS) $(BOBJS) build/objects/sndchan.o -o build/sndchan -lm

#Links debug version of sndcat
debug/sndchan: $(DOBJS) debug/objects/sndchan.o | debug
	$(CC) $(LFLAGS) $(DEBUG) $(DOBJS) debug/objects/sndchan.o -o debug/sndchan -lm

#Compiles sndinfo.o
build/objects/sndinfo.o: sndinfo.c core/sndcore.h | build/objects 
	$(CC) $(CFLAGS) sndinfo.c -o build/objects/sndinfo.o

#Compiles debug version of sndinfo.o
debug/objects/sndinfo.o: sndinfo.c core/sndcore.h | debug/objects 
	$(CC) $(CFLAGS) $(DEBUG) sndinfo.c -o debug/objects/sndinfo.o

#Compiles sndcat.o
build/objects/sndcat.o: sndcat.c core/sndcore.h | build/objects 
	$(CC) $(CFLAGS) sndcat.c -o build/objects/sndcat.o

#Compiles debug version of sndcat.o
debug/objects/sndcat.o: sndcat.c core/sndcore.h | debug/objects 
	$(CC) $(CFLAGS) $(DEBUG) sndcat.c -o debug/objects/sndcat.o

#Compiles sndchan.o
build/objects/sndchan.o: sndchan.c core/sndcore.h core/util.h | build/objects 
	$(CC) $(CFLAGS) sndchan.c -o build/objects/sndchan.o

#Compiles debug version of sndchan.o
debug/objects/sndchan.o: sndchan.c core/sndcore.h core/util.h | debug/objects 
	$(CC) $(CFLAGS) $(DEBUG) sndchan.c -o debug/objects/sndchan.o

#Compiles sndcore.o
build/objects/sndcore.o: core/sndcore.c core/sndcore.h core/types.h core/sndcs.h core/sndwav.h core/util.h | build/objects
	$(CC) $(CFLAGS) core/sndcore.c -o build/objects/sndcore.o

#Compiles debug version of sndcore.o
debug/objects/sndcore.o: core/sndcore.c core/sndcore.h core/types.h core/sndcs.h core/sndwav.h core/util.h | debug/objects
	$(CC) $(CFLAGS) $(DEBUG) core/sndcore.c -o debug/objects/sndcore.o

#Compiles sndcs.o
build/objects/sndcs.o: core/sndcs.c core/sndcs.h core/sndcore.h core/util.h core/types.h | build/objects
	$(CC) $(CFLAGS) core/sndcs.c -o build/objects/sndcs.o

#Compiles debug version of sndcs.o
debug/objects/sndcs.o: core/sndcs.c core/sndcs.h core/sndcore.h core/util.h core/types.h | debug/objects
	$(CC) $(CFLAGS) $(DEBUG) core/sndcs.c -o debug/objects/sndcs.o

#Compiles sndwav.o
build/objects/sndwav.o: core/sndwav.c core/sndwav.h core/types.h | build/objects 
	$(CC) $(CFLAGS) core/sndwav.c -o build/objects/sndwav.o

#Compiles debug version of sndwav.o
debug/objects/sndwav.o: core/sndwav.c core/sndwav.h core/types.h | debug/objects 
	$(CC) $(CFLAGS) $(DEBUG) core/sndwav.c -o debug/objects/sndwav.o

#Compiles util.o
build/objects/util.o: core/util.c core/util.h core/types.h | build/objects 
	$(CC) $(CFLAGS) core/util.c -o build/objects/util.o

#Compiles debug version of util.o
debug/objects/util.o: core/util.c core/util.h core/types.h | debug/objects 
	$(CC) $(CFLAGS) $(DEBUG) core/util.c -o debug/objects/util.o
