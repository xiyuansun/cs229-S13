#ifndef SNDCORE_H
#define SNDCORE_H
/*
Any generic operation having to do with sound can be found here
*/
#include "types.h"

snd_t* open_sound(char* path);
snd_t* read_sound(FILE* in, char* name);
void close_sound(snd_t* snd);
void write_sound(FILE* out, snd_t* snd);
void read(snd_t** snd);
void determine_type(FILE* in, sndtype* type);

void normalize_num_samples(snd_t* snd1, snd_t* snd2);
void normalize_bitres(snd_t* snd1, snd_t* snd2);
void normalize_num_channels(snd_t* snd1, snd_t* snd2);
void convert(snd_t* snd);

snd_dat_t* new_node(int num_channels);
void add(snd_t* snd, snd_dat_t* node);
u_int length(snd_dat_t* list);
u_int append(snd_t* snd, snd_dat_t* postfix);
void print_list(FILE* out, snd_dat_t* list, int num_channels);
#endif
