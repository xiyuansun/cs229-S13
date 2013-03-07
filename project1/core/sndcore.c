#include "sndcore.h"
#include "util.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
* Opens a sound at path, reads in the headers,
* and stores it in a snd_t structure.
*/
snd_t* open_sound(char* path)
{
    u_int rate = -1;
    u_int num_samples = -1;
    u_int len = -1;
    u_char bitdepth = -1;
    u_char num_channels = -1;
    sndtype type = CS229;
    
    snd_t* ret = (snd_t*) malloc(sizeof(snd_t));
    
    ret->rate = rate;
    ret->num_samples = num_samples;
    ret->len = len;
    ret->bitdepth = bitdepth;
    ret->num_channels = num_channels;
    ret->type = type;     
    ret->name = get_filename(path);
    ret->file = fopen(path, "rb");
    
    read_header(ret);

    return ret;
}

/*
* Reads the header of the file based on file type
*/
void read_header(snd_t* snd) 
{
    determine_type(snd->file, &(snd->type));
    
    if(CS229 == snd->type)
    {
        printf("determine_type failed\n");
        read_header_cs229(snd);
    }
    else if (WAVE == snd->type)
    {
        printf("determine_type passed\n");
        read_header_wav(snd);
    }
}

/*
* Reads the header of a 229 file:
*   1) checks the first character of a line
*       a) if it's '#', skip the line
*       b) else, go to (2)
*   2) reads in the keyword
*   3) reads in the value, if value is expected
*   4) if the keyword wasn't "STARTDATA", go to (1)
*/
void read_header_cs229(snd_t* snd)
{
    char word[11];

    while(1)
    {
        word[0] = fgetc(snd->file);

        if(word[0] == '#' || word[0] == '\n')
        {
            while(word[0] != '\n') word[0] = fgetc(snd->file);
            continue;
        }
        if(word[0] == '\n') continue;

        int i = 1;
        char c;
        
        while((c = fgetc(snd->file)) && is_alpha(c) && i < 10)
        {
            word[i] = c;
            ++i;
        }

        word[i] = 0;
        to_upper(word);

        uint val = 0;
        while((c = fgetc(snd->file)) && ('0' > c || '9' < c));
        while('0' <= c && c <= '9')
        {
            val *= 10;
            val += c - '0';
            c = fgetc(snd->file);
        }
        
        while(c != '\n') c = fgetc(snd->file);

        if(strncmp(word, "SAMPLERATE", 10) == 0)
        {
            snd->rate = val;
        }
        else if(strncmp(word, "SAMPLES", 7) == 0)
        {
            snd->num_samples = val;
        }
        else if(strncmp(word, "CHANNELS", 8) == 0)
        {
            snd->num_channels = val;
        }
        else if(strncmp(word, "BITRES", 6) == 0)
        {
            snd->bitdepth = val;
        }
        else if(strncmp(word, "STARTDATA", 9) == 0)
        {
            break;
        }
    } 
}

void read_header_wav(snd_t* snd)
{
    printf("WAVE");
    return;
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
    printf("%s\n", type_info);
    if(0 == strcmp(type_info, "CS229")) *type = CS229;
    else if(0 == strcmp(type_info, "RIFF")) *type = WAVE;
}
