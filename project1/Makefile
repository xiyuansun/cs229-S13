all: sndinfo

sndinfo: ./core/sndcore.o ./core/util.o sndinfo.o
	gcc -Wall -o sndinfo ./core/sndcore.o ./core/util.o sndinfo.o

./core/sndcore.o: core/sndcore.c core/sndcore.h core/util.h core/types.h
	gcc -Wall -c ./core/sndcore.c -o ./core/sndcore.o

./core/util.o: core/util.h
	gcc -Wall -c ./core/util.c -o ./core/util.o

sndinfo.o: sndinfo.c core/sndcore.c core/sndcore.h core/types.h
	gcc -Wall -c sndinfo.c
