#ifndef SNDCORE_H
#define SNDCORE_H
#include "types.h"

/*
* Opens a sound at path and reads it in with read_sound()
* Returns NULL if file could not be opened.
*/
snd_t* open_sound(char* path);

/*
* Returns a new snd_t* based on in
*/
snd_t* read_sound(FILE* in, char* name);

/*
* Frees memory related to snd
*/
void close_sound(snd_t* snd);

/*
* Calls the correct writer based on type
*/
void write_sound(FILE* out, snd_t* snd);

/*
* Calls the correct parser based on the file type
*/
void read(snd_t** snd, int* mute);

/*
* Determines the type of the sound based
* on the first few bytes in in.
*/
void determine_type(FILE* in, sndtype* type);

/*
* Adds samples to snd2 until it
* is the same length as snd1
*/
void normalize_num_samples(snd_t* snd1, snd_t* snd2);

/*
* Scales the sample data from snd2
* to match the bitres of snd1
*/
void normalize_bitres(snd_t* snd1, snd_t* snd2);

/*
* Adds channels to snd2 until it has
* the same number of channels as snd1
*/
void normalize_num_channels(snd_t* snd1, snd_t* snd2);

/*
* Converts snd from one file type to the
* other, limiting values where needed.
*/
void convert(snd_t* snd);

/*
* Mallocs a new node of num_channels channels,
* checks the malloc, initializes channels to
* zero, then returns the new node.
*/
snd_dat_t* new_node(int num_channels);

/*
* Adds a node to the end of
* the sound data linked-list
*/
void add(snd_t* snd, snd_dat_t* node);

/*
* Finds the length of a sound data linked-list
*/
u_int length(snd_dat_t* list);

/*
* Appends postfix on to list, returning the size of postfix
*/
u_int append(snd_t* snd, snd_dat_t* postfix);

/*
* Prints each item in a sound data linked-list
* such that each node is on its own line.
*/
void print_list(FILE* out, snd_dat_t* list, int num_channels);
#endif
