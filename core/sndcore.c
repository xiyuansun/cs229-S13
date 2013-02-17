#include "sndcore.h"

/*
Assumes the file pointer in is at beginning of file
Will return null if in is null
*/
snd_t* read_sound(char* path)
{

    u_int rate;
    u_int num_samples;
    u_int len;
    u_char bitdepth;
    u_char num_channels;
    sndtype_e type;
    char* name;
    
    type = determine_snd_type(in);
}
