all: dist/bin/sndinfo dist/bin/sndcat

clean:
	rm -rf dist debug

dist:
	mkdir dist

dist/build:
	mkdir dist/build

dist/bin:
	mkdir dist/bin

dist/bin/sndinfo: dist dist/build dist/bin dist/build/sndinfo.o dist/build/sndcore.o dist/build/util.o dist/build/sndcs.o dist/build/sndwav.o
	gcc -Wall dist/build/sndcore.o dist/build/util.o dist/build/sndwav.o dist/build/sndcs.o dist/build/sndinfo.o -o dist/bin/sndinfo

dist/bin/sndcat: dist dist/build dist/bin dist/build/sndcat.o dist/build/sndcore.o dist/build/util.o dist/build/sndcs.o dist/build/sndwav.o
	gcc -Wall dist/build/sndcore.o dist/build/util.o dist/build/sndwav.o dist/build/sndcs.o dist/build/sndcat.o -o dist/bin/sndcat

dist/build/sndcore.o: core/sndcore.c core/sndcore.h core/util.h core/types.h core/sndcs.h core/sndwav.h
	gcc -Wall -c core/sndcore.c -o dist/build/sndcore.o

dist/build/util.o: core/util.c core/util.h core/types.h 
	gcc -Wall -c core/util.c -o dist/build/util.o

dist/build/sndcs.o: core/sndcs.c core/sndcs.h core/sndcore.h core/types.h core/util.h
	gcc -Wall -c core/sndcs.c -o dist/build/sndcs.o

dist/build/sndwav.o: core/sndwav.c core/sndwav.h core/types.h
	gcc -Wall -c core/sndwav.c -o dist/build/sndwav.o

dist/build/sndinfo.o: sndinfo.c core/sndcore.c core/sndcore.h core/types.h
	gcc -Wall -c sndinfo.c -o dist/build/sndinfo.o

dist/build/sndcat.o: sndcat.c core/sndcore.c core/sndcore.h core/types.h
	gcc -Wall -c sndcat.c -o dist/build/sndcat.o

#Commands to build debug version.
bug: debug/bin/sndinfo debug/bin/sndcat

debug:
	mkdir debug

debug/build:
	mkdir debug/build

debug/bin:
	mkdir debug/bin

debug/bin/sndinfo: debug debug/build debug/bin debug/build/sndinfo.o debug/build/sndcore.o debug/build/util.o debug/build/sndcs.o debug/build/sndwav.o
	gcc -g -Wall debug/build/sndcore.o debug/build/util.o debug/build/sndwav.o debug/build/sndcs.o debug/build/sndinfo.o -o debug/bin/sndinfo

debug/bin/sndcat: debug debug/build debug/bin debug/build/sndcat.o debug/build/sndcore.o debug/build/util.o debug/build/sndcs.o debug/build/sndwav.o
	gcc -g -Wall debug/build/sndcore.o debug/build/util.o debug/build/sndwav.o debug/build/sndcs.o debug/build/sndcat.o -o debug/bin/sndcat

debug/build/sndcore.o: core/sndcore.c core/sndcore.h core/util.h core/types.h core/sndcs.h core/sndwav.h
	gcc -g -Wall -c core/sndcore.c -o debug/build/sndcore.o

debug/build/util.o: core/util.c core/util.h core/types.h 
	gcc -g -Wall -c core/util.c -o debug/build/util.o

debug/build/sndcs.o: core/sndcs.c core/sndcs.h core/sndcore.h core/types.h core/util.h
	gcc -g -Wall -c core/sndcs.c -o debug/build/sndcs.o

debug/build/sndwav.o: core/sndwav.c core/sndwav.h core/types.h
	gcc -g -Wall -c core/sndwav.c -o debug/build/sndwav.o

debug/build/sndinfo.o: sndinfo.c core/sndcore.c core/sndcore.h core/types.h
	gcc -g -Wall -c sndinfo.c -o debug/build/sndinfo.o

debug/build/sndcat.o: sndcat.c core/sndcore.c core/sndcore.h core/types.h
	gcc -g -Wall -c sndcat.c -o debug/build/sndcat.o
