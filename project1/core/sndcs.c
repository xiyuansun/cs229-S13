#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "sndcs.h"
#include "sndcore.h"
#include "util.h"

/*
* Reads the header of a 229 file:
*   1) checks the first character of a line
*       a) if it's '#', skip the line
*       b) else, go to (2)
*   2) reads in the keyword
*   3) reads in the value, if value is expected
*   4) if the keyword wasn't "STARTDATA", go to (1)
*/
int read_header_cs229(snd_t* snd)
{
    char word[11];

    while(!feof(snd->file))
    {
        word[0] = fgetc(snd->file);

        if(word[0] == '#')
        {
            while(word[0] != '\n' && !feof(snd->file)) word[0] = fgetc(snd->file);
            continue;
        }
        else if(!is_alpha(word[0]))
        {
            while(!is_alpha(word[0] && !feof(snd->file)) && word[0] != '\n') word[0] = fgetc(snd->file); 
        }

        if(word[0] == '\n') continue;

        int i = 1;
        char c;
        
        while((c = fgetc(snd->file)) && !feof(snd->file) && is_alpha(c) && i < 10)
        {
            word[i] = c;
            ++i;
        }

        word[i] = 0;
        to_upper(word);

        if(strncmp(word, "STARTDATA", 9) == 0)
        {
            while(c != '\n' && !feof(snd->file)) c = fgetc(snd->file);
            break;
        }

        uint val = 0;
        while((c = fgetc(snd->file) && !feof(snd->file)) && ('0' > c || '9' < c));
        while('0' <= c && c <= '9' && !feof(snd->file))
        {
            val *= 10;
            val += c - '0';
            c = fgetc(snd->file);
        }
        
        while(c != '\n' && !feof(snd->file)) c = fgetc(snd->file);

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
        else
        {
            fprintf(stderr, "Error: Unrecognized keyword %s.\n", word);
            return 1;
        }
    }

    if(feof(snd->file))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
* Reads the sound data of a cs229 file:
*   1) Loops until EOF
*       a) if we're on a new sample, create a new node
*       b) add the data scanned in to the channel info on the current node
*       c) if we've filled the current node, add it to the list in snd
*/
void read_info_cs229(snd_t* snd)
{
    snd_dat_t* node;
    int i = 0;
    char read_sample = 0;
    int dat = 0;
    int mult = 1;
    char c;
    
    while((c = fgetc(snd->file)) && !feof(snd->file))
    {
        if(i == 0)
        {
            node = new_node(snd->num_channels);
        }

        if(read_sample && isspace(c))
        {
            node->channel_data[i] = mult * dat;
            mult = 1;
            dat = 0;
            read_sample = 0;
            ++i;
        }
        else if('0' <= c && c <= '9')
        {
            read_sample = 1;
            dat *= 10;
            dat += (c - '0');
        }
        else if(!read_sample && c == '-')
        {
            read_sample = 1;
            mult = -1;
        }
        else if(!isspace(c))
        {
            fprintf(stderr, "Error parsing %s. Exiting.\n", snd->name);
            exit(1);
        }

        if(i >= (snd->num_channels))
        {
            add(snd, node);
            i = 0;
        }
    }
}

void write_cs229(FILE* out, snd_t* snd)
{
    fputs("cs229\n", out);
    fprintf(out, "samples %u\n", snd->num_samples);
    fprintf(out, "channels %u\n", snd->num_channels);
    fprintf(out, "bitres %u\n", snd->bitdepth);
    fprintf(out, "samplerate %u\n", snd->rate);
    fputs("startdata\n", out);
    print_list(out, snd->data, snd->num_channels);
}
