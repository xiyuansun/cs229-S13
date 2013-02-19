#include "sndcore.h"
#include "util.h"
#include <stdio.h>
#include <string.h>

/*
Opens a sound at path, reads in the headers,
and stores it in a snd_t structure.
*/
snd_t* open_sound(char* path)
{
    u_int rate;
    u_int num_samples;
    u_int len;
    u_char bitdepth;
    u_char num_channels;
    
    sndtype type;
    char* name;

    FILE* in = fopen(path, "rb");

    type = determine_type(in);
    name = get_filename(path);

    snd_t* ret;
    ret = {rate, num_samples, len, bitdepth, num_channelse, type, name, in};
    return ret;
}

/*
Determines the type of the sound based
on the next 4 bytes in in.
*/
sndtype determine_type(FILE* in)
{
    char type_info[5] = {0, 0, 0, 0, 0};
    fgets(type_info, 5, in);

    if(0 == strncmp(to_upper(type_info), "CS22")) return CS229;
    else if(0 == strncmp(to_upper(type_info), "RIFF")) return WAVE;
}
