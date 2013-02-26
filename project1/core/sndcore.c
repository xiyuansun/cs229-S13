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
    u_int rate = -1;
    u_int num_samples = -1;
    u_int len = -1;
    u_char bitdepth = -1;
    u_char num_channels = -1;
    sndtype type;
    char* name;
    FILE* in;
    snd_t* ret;

    ret = {rate, num_samples, len, bitdepth, num_channels, type, name, in};
    ret->name = get_filename(path);
    ret->file = fopen(path, "rb");

    read_header(ret);


    return ret;
}

void read_header(snd_t* snd) 
{
    determine_type(snd->file, &(snd->type));
    
    if(CS229 == snd->type)
        read_header_cs229(snd);
    else if (WAVE == snd->type)
        read_header_wav(snd);
}

void read_header_cs229(snd_t* snd)
{
    //Will (probably) need to change this
    char* line[1000];

    do
    {
        fgets(line, 1000, snd->file);
        to_upper(line);
        
        if(strncmp(line, "SAMPLERATE", 10) == 0)
        {
            snd->rate = parse_num(&line[10]);
        }
        else if(strncmp(line, "SAMPLES", 7) == 0)
        {
            snd->num_samples = parse_num(&line[7]);
        }
        else if(strncmp(line, "CHANNELS", 8) == 0)
        {
            snd->num_channels = parse_num(&line[8]);
        }
        else if(strncmp(line, "BITRES", 6) == 0)
        {
            snd->bitrate = parse_num(&line[6]);
        }

    } while(strncmp(line, "STARTDATA", 9) == 0)
}

/*
Determines the type of the sound based
on the first few bytes in in.
*/
void determine_type(FILE* in, sndtype* type)
{
    char type_info[6] = {0, 0, 0, 0, 0, 0};
    int i;

    for(i = 0; i < 4; ++i)
        type_info[i] = fgetc(in);

    to_upper(type_info);
    if(type_info[0] == 'C') type_info[4] = fgetc(in);

    if(0 == strncmp(type_info, "CS229")) *type =  CS229;
    else if(0 == strncmp(type_info, "RIFF")) *type = WAVE;
}
