#ifndef TYPES_H
#define TYPES_H
#include <stdio.h>

/*
* Enum to represent the type of a sound.
*/
typedef enum SoundType {CS229, WAVE, ABC229} sndtype;

/*
* Defined because unsigned is a long word.
*/
typedef unsigned int u_int;
typedef unsigned char u_char;

/*
* Each one of these stores data for all channels in one sample
*/
typedef struct sound_data
{
    struct sound_data* next;
    int* channel_data;
} snd_dat_t;

/*
* Stores sound data in memory
*/
typedef struct sound
{
    snd_dat_t* data;
    snd_dat_t* last;
    u_int rate;
    u_int num_samples;
    double len;
    u_char bitdepth;
    u_char num_channels;
    sndtype type;
    char* name;
    FILE* file;
} snd_t;

/*
* For ease-of-use in passing ADSR paramaters.
*/
typedef struct adsr_envelope
{
    double a;
    double d;
    double s;
    double r;
    double v;
} adsr_t;

#endif
