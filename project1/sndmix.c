#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <math.h>
#include <string.h>
#include "./core/sndcore.h"
#include "./core/util.h"

void print_usage(int status);
void mix(snd_t* snd1, snd_t* snd2);
void amp(snd_t* snd, double mult);
double get_mult(char* arg);
void normalize(snd_t* snd1, snd_t* snd2);

int main(int argc, char* argv[])
{
    const char* err_prefix = "sndmix: Error:";
    sndtype out_type = CS229;
    int i = 1;
    char* cur_arg;
    snd_t* info = 0;
    snd_t* current;
    FILE* out = stdout;
    double cur_mult;
    
    while(i < argc)
    {
        cur_arg = argv[i];
        
        if(strcmp(cur_arg, "-h") == 0)
        {
            print_usage(0);
        }
        else if(strcmp(cur_arg, "-w") == 0)
        {
            out_type = WAVE;
            ++i;
            continue;
        }
        else if(strcmp(cur_arg, "-o") == 0)
        {
            if(i + 1 >= argc)
            {
                fprintf(stderr, "%s -o was specified without an argument. Exiting.\n", err_prefix);
                exit(1);
            }

            out = fopen(argv[i + 1], "wb");

            if(!out)
            {
                fprintf(stderr, "%s Could not open %s. Exiting.\n", err_prefix, argv[i + 1]);
                exit(1);
            }

            i += 2;
        }
        else
        {
            /*number time*/
            if(i + 1 >= argc)
            {
                fprintf(stderr, "%s Missing argument from [MULT FILE] pair. Exiting\n", err_prefix);
                exit(1);
            }
            
            cur_mult = get_mult(argv[i]);
            current = open_sound(argv[i + 1]);

            if(!current)
            {
                fprintf(stderr, "%s Could not open %s. Exiting.\n", err_prefix, argv[i + 1]);
                exit(1);
            }

            amp(current, cur_mult);

            if(!info)
            {
                info = current;
            }
            else
            {
                mix(info, current);
            }

            i += 2;
        }
    }
    
    if(out_type != info->type)
    {
        convert(info);
    }

    write_sound(out, info);
    close_sound(info);

    if(out != stdout)
    {
        fclose(out);
    }

    return 0;
}

/*
* Prints out usage details for sndmix
*/
void print_usage(int status)
{
    puts("Usage: sndmix [OPTION]... MULT1 FILE1 [MULT2 FILE2]... [MULTn FILEn]\n");
    puts("Reads in all sound files passed as arguments and mixes them. All sample data from FILEi is multiplied by MULTi. This data is then summed and written out. -10 <= MULTi <= 10\n");
    puts("OPTIONS:");
    puts("\t-h\t\tDisplays this help message.\n");
    puts("\t-o [FILE]\tSpecifies the output file name. If omitted, the file is written to standard output.");
    puts("\t-w\t\tOutput in .wav format instead of .cs229 format");
    exit(status);
}

/*
* Normalizes and mixes snd1 and snd2
*/
void mix(snd_t* snd1, snd_t* snd2)
{
    if(snd1->rate != snd2->rate)
    {
        fprintf(stderr, "sndcat: Incompatible File Error: Sample rates are not the same. Exiting.\n");
        exit(1);
    }

    if(!snd1 || !snd2)
    {
        return;
    }

    normalize(snd1, snd2);
    
    int i, max, min;
    long tmp_dat;
    snd_dat_t* snd1_node = snd1->data;
    snd_dat_t* snd2_node = snd2->data;
    
    if(snd1->type == WAVE)
    {
        min = -1*((int) pow(2, snd1->bitdepth-1));
        max = -1*(min + 1);
    }
    else
    {
        min = -1*((int) pow(2, snd1->bitdepth-1)) + 1;
        max = -1*min;
    }

    while(snd1_node && snd2_node)
    {
        for(i = 0; i < snd1->num_channels; ++i)
        {
            tmp_dat = snd1_node->channel_data[i] + snd2_node->channel_data[i];
            snd1_node->channel_data[i] = LIMIT(tmp_dat, max, min);
        }

        snd1_node = snd1_node->next;
        snd2_node = snd2_node->next;
    }
}

/*
* Normalizes the number of
* channels, bit resolution,
* and length of snd1 and snd2
*/
void normalize(snd_t* snd1, snd_t* snd2)
{
    if(snd1->bitdepth > snd2->bitdepth)
    {
        normalize_bitres(snd1, snd2);
    }
    else if(snd1->bitdepth < snd2->bitdepth)
    {
        normalize_bitres(snd2, snd1);
    }

    if(snd1->num_channels > snd2->num_channels)
    {
        normalize_num_channels(snd1, snd2);
    }
    else if(snd1->num_channels < snd2->num_channels)
    {
        normalize_num_channels(snd2, snd1);
    }
    
    if(snd1->num_samples > snd2->num_samples)
    {
        normalize_num_samples(snd1, snd2);
    }
    else if(snd1->num_samples < snd2->num_samples)
    {
        normalize_num_samples(snd2, snd1);
    }

    if(snd2->type == WAVE && snd1->type != WAVE)
    {
        convert(snd1);
    }
}

void amp(snd_t* snd, double mult)
{
    int i, max, min;
    max = (int) pow(2, snd->bitdepth-1) - 1;
    min = -1*max;
    snd_dat_t* node = snd->data;
    
    if(-10 > mult || 10 < mult)
    {
        fprintf(stderr, "sndmix: Error: Multiplier for %s is out of range. Exiting.", snd->name);
        exit(1);
    }

    while(node)
    {
        for(i = 0; i < snd->num_channels; ++i)
        {
            node->channel_data[i] = (int) LIMIT(((long) node->channel_data[i]) * mult, max, min);
        }

        node = node->next;
    }
}

double get_mult(char* arg)
{
    char* endptr;
    double cur_mult = strtod(arg, &endptr);
    if(*endptr != '\0')
    {
        fprintf(stderr, "sndmix: Error: %s is not an integer. Exiting.\n", arg);
        exit(1);
    }
    return cur_mult;
}
