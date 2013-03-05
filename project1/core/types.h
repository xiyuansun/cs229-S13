#ifndef TYPES_H
#define TYPES_H
#include <stdio.h>

typedef enum SoundType {CS229, WAVE} sndtype;

typedef unsigned int u_int;
typedef unsigned char u_char;

struct Sound
{
    u_int rate;
    u_int num_samples;
    u_int len;
    u_char bitdepth;
    u_char num_channels;
    sndtype type;
    char* name;
    FILE* file;
};

typedef struct Sound snd_t;
#endif
