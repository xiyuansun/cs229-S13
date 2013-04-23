PROGS = showgen.c
BOBJS = build/objects/aut.o build/objects/board.o build/objects/scanner.o build/objects/util.o
DOBJS = debug/objects/aut.o debug/objects/board.o debug/objects/scanner.o debug/objects/util.o
DEBUG = -g
CFLAGS = -Wall -O3 -c
LFLAGS = -Wall -O3
CC = g++

all: showgen

showgen: build/showgen
	@

bug: debug/showgen

clean:
	rm -rf build debug cfogerty-aut.tar.gz

tarball:
	tar -czvf cfogerty-aut.tar.gz $(PROGS) lib/* makefile README

build:
	mkdir build

debug:
	mkdir debug

build/objects: | build
	mkdir build/objects

debug/objects: | debug
	mkdir debug/objects

#Links showgen
build/showgen: $(BOBJS) build/objects/showgen.o | build
	$(CC) $(LFLAGS) $(BOBJS) build/objects/showgen.o -o build/showgen

#Links debug version of showgen
debug/showgen: $(DOBJS) debug/objects/showgen.o | debug
	$(CC) $(LFLAGS) $(DEBUG) $(DOBJS) debug/objects/showgen.o -o debug/showgen

#Compiles showgen.o
build/objects/showgen.o: showgen.cc lib/board.h lib/io/aut.h | build/objects
	$(CC) $(CFLAGS) showgen.cc -o build/objects/showgen.o

#Compiles debug version of showgen.o
debug/objects/showgen.o: showgen.cc lib/board.h lib/io/aut.h | debug/objects
	$(CC) $(CFLAGS) $(DEBUG) showgen.cc -o debug/objects/showgen.o

#Compiles aut.o
build/objects/aut.o: lib/io/aut.cc lib/io/aut.h lib/io/scanner.h lib/board.h lib/util.h | build/objects
	$(CC) $(CFLAGS) lib/io/aut.cc -o build/objects/aut.o

#Compiles debug version aut.o
debug/objects/aut.o: lib/io/aut.cc lib/io/aut.h lib/io/scanner.h lib/board.h lib/util.h | debug/objects
	$(CC) $(CFLAGS) $(DEBUG) lib/io/aut.cc -o debug/objects/aut.o

#Complies board.o
build/objects/board.o: lib/board.cc lib/board.h | build/objects
	$(CC) $(CFLAGS) lib/board.cc -o build/objects/board.o

#Complies debug version of board.o
debug/objects/board.o: lib/board.cc lib/board.h | debug/objects
	$(CC) $(CFLAGS) $(DEBUG) lib/board.cc -o debug/objects/board.o

#Compiles scanner.o
build/objects/scanner.o: lib/io/scanner.cc lib/io/scanner.h lib/constants.h | build/objects
	$(CC) $(CFLAGS) lib/io/scanner.cc -o build/objects/scanner.o

#Compiles debug version of scanner.o
debug/objects/scanner.o: lib/io/scanner.cc lib/io/scanner.h lib/constants.h | debug/objects
	$(CC) $(CFLAGS) $(DEBUG) lib/io/scanner.cc -o debug/objects/scanner.o

#Compiles util.o
build/objects/util.o: lib/util.cc lib/util.h lib/constants.h | build/objects
	$(CC) $(CFLAGS) lib/util.cc -o build/objects/util.o

#Compiles debug version of util.o
debug/objects/util.o: lib/util.cc lib/util.h lib/constants.h | debug/objects
	$(CC) $(CFLAGS) $(DEBUG) lib/util.cc -o debug/objects/util.o