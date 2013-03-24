#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "sndcore.h"
#include "sndcs.h"
#include "sndwav.h"
#include "util.h"

/*
* Opens a sound at path and reads it in with read_sound()
* Returns NULL if file could not be opened.
*/
snd_t* open_sound(char* path)
{
    FILE* in = fopen(path, "rb");

    if(!in)
    {
        fprintf(stderr, "Error: %s: File could not be opened.\n", path);
        return NULL;
    }

    snd_t* ret = read_sound(in, get_filename(path));
    
    if(!ret)
    {
        fprintf(stderr, "Error: %s: Malformed sound file.\n", path);
        return NULL;
    }
    fclose(in);

    return ret;
}

/*
* 1) Allocates memory for a snd_t struct
* 2) Fills with default values
* 3) Reads and parses the file
* 4) Returns the snd_t struct
*/
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
    ret->data = NULL;
    ret->last = NULL;
    
    read(&ret);

    if(ret)
    {

        if( ((int) ret->num_samples) <= 0 )
        {
            ret->num_samples = length(ret->data);
        }

        ret->len = (int) (ret->num_samples) * 1.0 / (ret->rate);

        return ret;
    }
    else
    {
        return NULL;
    }
}

/*
* 1) Frees the sound data
* 2) Frees the snd_t struct
*/
void close_sound(snd_t* snd)
{  
    snd_dat_t* cur_node = snd->data;
    snd_dat_t* next_node;

    while(cur_node)
    {
        next_node = cur_node->next;
        cur_node->next = NULL;
        free(cur_node->channel_data);
        free(cur_node);
        cur_node = next_node;
    }
    snd->data = NULL;
    free(snd);
}

/*
* Calls the correct writer based on type
*/
void write_sound(FILE* out, snd_t* sound)
{
    if(sound->type == CS229)
    {
        write_cs229(out, sound);
    }
    else
    {
        write_wav(out, sound);
    }
}

/*
* Calls the correct parser based on the file type
*/
void read(snd_t** snd) 
{
    determine_type((*snd)->file, &((*snd)->type));
    
    if(CS229 == (*snd)->type)
    {
        read_header_cs229(*snd);
        read_info_cs229(*snd);
    }
    else
    {
        int res = read_header_wav(*snd);
        res |= read_info_wav(*snd);
        if(res)
        {
            close_sound(*snd);
            *snd = NULL;
        }
    }
}

/*
* Determines the type of the sound based
* on the first few bytes in in.
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

/*
* Adds samples to snd2 until it
* is the same length as snd1
*/
void normalize_num_samples(snd_t* snd1, snd_t* snd2)
{
    int i, diff;
    i = 0;
    diff = snd1->num_samples - snd2->num_samples;

    if(diff <= 0) return;

    snd_dat_t* node;

    while(i < diff)
    {
        node = new_node(snd2->num_channels);
        add(snd2, node);
        ++i;
    }
    snd2->num_samples += diff;
}

/*
* Scales the sample data from snd2
* to match the bitres of snd1
*/
void normalize_bitres(snd_t* snd1, snd_t* snd2)
{
    int scale_factor = (int) pow(2, snd1->bitdepth - snd2->bitdepth);

    snd_dat_t* samples = snd2->data;

    int i;

    while(samples)
    {
        for(i = 0; i < snd2->num_channels; ++i)
        {
            samples->channel_data[i] *= scale_factor;
        }
        
        samples = samples->next;
    }

    snd2->bitdepth = snd1->bitdepth;
}

/*
* Adds channels to snd2 until it has
* the same number of channels as snd1
*/
void normalize_num_channels(snd_t* snd1, snd_t* snd2)
{
    int diff = snd1->num_channels - snd2->num_channels;
    if(diff <= 0) return;

    snd_dat_t* samples = snd2->data;

    while(samples)
    {
        int* new_block = (int*) realloc(samples->channel_data, snd1->num_channels);
        check_malloc(new_block);

        samples->channel_data = new_block;
        
        int i;
        for(i = snd2->num_channels; i < snd1->num_channels; ++i)
        {
            samples->channel_data[i] = 0;
        }

        samples = samples->next;
    }

    snd2->num_channels = snd1->num_channels;
}

/*
* Converts snd from one file type to the
* other, limiting values where needed.
*/
void convert(snd_t* snd)
{
    if(snd->type == CS229)
    {
        snd->type = WAVE;
        return;
    }
    
    int i;
    snd->type = CS229;
    snd_dat_t* node = snd->data;
    long max = (long) pow(2, snd->bitdepth - 1) - 1;
    long min = -1 * max;

    while(node)
    {
        for(i = 0; i < snd->num_channels; ++i)
        {
            node->channel_data[i] = (int) LIMIT(((long) node->channel_data[i]), max, min);
        }
        node = node->next;
    }
}

/********************************************/
/*            LINKED LIST STUFF             */
/********************************************/

snd_dat_t* new_node(int num_channels)
{
    snd_dat_t* node = malloc(sizeof(snd_dat_t));
    check_malloc(node);

    node->channel_data = malloc(num_channels * sizeof(int));
    check_malloc(node->channel_data);
    
    int i = 0;
    for(; i < num_channels; ++i)
    {
        node->channel_data[i] = 0;
    }
    
    node->next = NULL;

    return node;
}

/*
* Adds a new node to the end of the
* sound data linked-list
*/
void add(snd_t* snd, snd_dat_t* node)
{
    if(!(snd->last))
    {
        snd->data = node;
        snd->last = node;
        return;
    }
    
    snd_dat_t* cur_node = snd->last;

    while(cur_node->next)
    {
        cur_node = cur_node->next;
    }
    cur_node->next = node;
    snd->last = node;
}

/*
* Finds the length of a sound data linked-list
*/
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

/*
* Appends postfix on to list, returning the size of postfix
*/
u_int append(snd_t* snd, snd_dat_t* postfix)
{
    add(snd, postfix);
    u_int ret = length(postfix);

    while(postfix->next)
    {
        postfix = postfix->next;
    }

    snd->last = postfix;

    return ret;
}

/*
* Prints each item in a sound data linked-list
* such that each node is on its own line.
*/
void print_list(FILE* out, snd_dat_t* list, int num_channels)
{
    int i;
    while(list)
    {
        for(i = 0; i < num_channels-1; ++i)
        {
            fprintf(out, "%d ", list->channel_data[i]);
        }
        fprintf(out, "%d\n", list->channel_data[i]);
        list = list->next;
    }
}
