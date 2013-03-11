all: sndinfo

clean:
	rm -f sndinfo *.o core/*.o

sndinfo: core/sndcore.o core/util.o sndinfo.o core/sndcs.o core/sndwav.o
	gcc -Wall -o sndinfo ./core/sndcore.o ./core/util.o ./core/sndwav.o ./core/sndcs.o sndinfo.o

core/sndcore.o: core/sndcore.c core/sndcore.h core/util.h core/types.h core/sndcs.h core/sndwav.h
	gcc -Wall -c ./core/sndcore.c -o ./core/sndcore.o

core/util.o: core/util.c core/util.h core/types.h 
	gcc -Wall -c ./core/util.c -o ./core/util.o

core/sndcs.o: core/sndcs.c core/sndcs.h core/sndcore.h core/types.h core/util.h
	gcc -Wall -c ./core/sndcs.c -o ./core/sndcs.o

core/sndwav.o: core/sndwav.c core/sndwav.h core/types.h
	gcc -Wall -c ./core/sndwav.c -o ./core/sndwav.o

sndinfo.o: sndinfo.c core/sndcore.c core/sndcore.h core/types.h
	gcc -Wall -c sndinfo.c
