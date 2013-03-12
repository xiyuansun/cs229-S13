all: sndinfo

clean:
	rm bin/* build/*; rmdir bin build

build:
	mkdir build

bin:
	mkdir bin

sndinfo: build bin build/sndinfo.o build/sndcore.o build/util.o build/sndcs.o build/sndwav.o
	gcc -Wall ./build/sndcore.o ./build/util.o ./build/sndwav.o ./build/sndcs.o ./build/sndinfo.o -o ./bin/sndinfo

build/sndcore.o: core/sndcore.c core/sndcore.h core/util.h core/types.h core/sndcs.h core/sndwav.h
	gcc -Wall -c ./core/sndcore.c -o ./build/sndcore.o

build/util.o: core/util.c core/util.h core/types.h 
	gcc -Wall -c ./core/util.c -o ./build/util.o

build/sndcs.o: core/sndcs.c core/sndcs.h core/sndcore.h core/types.h core/util.h
	gcc -Wall -c ./core/sndcs.c -o ./build/sndcs.o

build/sndwav.o: core/sndwav.c core/sndwav.h core/types.h
	gcc -Wall -c ./core/sndwav.c -o ./build/sndwav.o

build/sndinfo.o: sndinfo.c core/sndcore.c core/sndcore.h core/types.h
	gcc -Wall -c sndinfo.c -o ./build/sndinfo.o
