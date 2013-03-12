#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sndcore.h"
#include "sndcs.h"
#include "sndwav.h"
#include "util.h"

/*
* Opens a sound at path, reads in the headers,
* and stores it in a snd_t structure.
*/
snd_t* open_sound(char* path)
{
    FILE* in = fopen(path, "rb");

    if(!in)
    {
        fprintf(stderr, "sndinfo: %s: File could not be opened.\n", path);
        exit(1);
    }

    snd_t* ret = read_sound(in, get_filename(path));

    fclose(in);

    return ret;
}

snd_t* read_sound(FILE* in, char* name)
{
    u_int rate = -1;
    u_int num_samples = -1;
    u_int len = -1;
    u_char bitdepth = -1;
    u_char num_channels = -1;
    sndtype type = CS229;
    
    snd_t* ret = (snd_t*) malloc(sizeof(snd_t));
    
    check_malloc(ret);

    ret->rate = rate;
    ret->num_samples = num_samples;
    ret->len = len;
    ret->bitdepth = bitdepth;
    ret->num_channels = num_channels;
    ret->type = type;     
    ret->name = name;
    ret->file = in;
    
    read(ret);

    if( ((int) ret->num_samples) <= 0 )
    {
        ret->num_samples = length(ret->data);
    }

    ret->len = (int) (ret->num_samples) * 1.0 / (ret->rate);

    return ret;
}

void close_sound(snd_t* snd)
{
    snd_dat_t* cur_node = snd->data;
    snd_dat_t* next_node = cur_node->next;

    while(cur_node)
    {
        free(cur_node->channel_data);
        free(cur_node);
        cur_node = next_node;
        next_node = cur_node->next;
    }

    free(snd);
}

/*
* Reads the header of the file based on file type
*/
void read(snd_t* snd) 
{
    determine_type(snd->file, &(snd->type));
    
    if(CS229 == snd->type)
    {
        read_header_cs229(snd);
        read_info_cs229(snd);
    }
    else if (WAVE == snd->type)
    {
        read_header_wav(snd);
    }
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
    if(0 == strcmp(type_info, "CS229")) *type = CS229;
    else if(0 == strcmp(type_info, "RIFF")) *type = WAVE;
}

void add(snd_dat_t** list, snd_dat_t** node)
{
    if(!(*list))
    {
        *list = *node;
        return;
    }
    
    snd_dat_t* cur_node = *list;

    while(cur_node->next)
    {
        cur_node = cur_node->next;
    }
    cur_node->next = *node;
}

u_int length(snd_dat_t* list)
{
    int i = 0;
    while(list)
    {
        list = list->next;
        ++i;
    }
    return i;
}

void print_list(snd_dat_t* list, int num_channels)
{
    int i;
    while(list)
    {
        for(i = 0; i < num_channels; ++i)
        {
            printf("%d ", list->channel_data[i]);
        }
        printf("\n");
        list = list->next;
    }
}
