#ifndef SNDCORE_H
#define SNDCORE_H
/*
Any generic operation having to do with sound can be found here
*/
#include "types.h"

/*
TODO Determine arguments
*/
snd_t* open_sound(char* path);
void close_sound(snd_t* snd);
void write_sound(snd_t* snd);

void read_header(snd_t* snd);
void read_header_cs229(snd_t* snd);
void read_header_wav(snd_t* snd);

void determine_type(FILE* in, sndtype* type);
#endif
