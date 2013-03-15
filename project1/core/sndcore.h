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
snd_t* read_sound(FILE* in, char* name); 
void close_sound(snd_t* snd);
void write_sound(FILE* out, snd_t* snd);

void read(snd_t* snd);

void determine_type(FILE* in, sndtype* type);
void add(snd_dat_t** list, snd_dat_t** node);
u_int length(snd_dat_t* list);
void print_list(FILE* out, snd_dat_t* list, int num_channels);
#endif
