#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "./core/sndcore.h"
#include "./core/util.h"

void print_usage(int status);
void layer(snd_t* snd1, snd_t* snd2);
void unlayer(snd_t* snd, int channel);
void normalize(snd_t* snd1, snd_t* snd2);

/*
* 1) Parses arguments
* 2) Adds channels as sounds are read in
* 3) Picks channel to write out if necessary
* 4) Writes out sound
*/
int main(int argc, char* argv[])
{
    sndtype out_type = CS229;
    int i;
    char c;
    char* outfile;
    char* endptr;
    int channel;
    char c_optn_used = 0;
    snd_t* info = 0;
    snd_t* current;
    FILE* out;

    while((c = getopt(argc, argv, "ho:wc:")) != -1)
    {
        switch(c)
        {
            case 'h':
                print_usage(0);
                break;
            case 'o':
                outfile = optarg;
                break;
            case 'w':
                out_type = WAVE;
                break;
            case 'c':
                channel = (int) strtol(optarg, &endptr, 0);
                c_optn_used = 1;
                break;
            case '?':
                fprintf(stderr, "sndchan: Error: Try 'sndchan -h' for more information.\n");
                exit(1);
                break;
            default:
                print_usage(1);
        }
    }
    
    if(c_optn_used && *endptr != '\0')
    {
        fprintf(stderr, "sndchan: Error: Argument for -c is not a number\n");
        exit(1);
    }

    /*
    Get from stdin
    */
    if(optind == argc)
    {
        info = read_sound(stdin, "Standard Input");
    }

    for(i = optind; i < argc; ++i)
    {
        if(!info)
        {
            info = open_sound(argv[i]);
            if(!info)
            {
                fprintf(stderr, "sndchan: Error: %s could not be opened. Exiting.\n", argv[i]);
                exit(1);
            }

            continue;
        }

        current = open_sound(argv[i]);
        if(!current)
        {
            fprintf(stderr, "sndchan: Error: %s could not be opened. Exiting.\n", argv[i]);
            exit(1);
        }
        layer(info, current);
        close_sound(current);
        
    }

    if(c_optn_used)
    {
        if(1 <= channel && channel <= info->num_channels)
        {
            unlayer(info, channel-1);
        }
        else
        {
            fprintf(stderr, "sndchan: Error: Argument for -c is out of range. Try 'sndchan -h' for more information.\n");
            exit(1);
        }
    }

    if(out_type != info->type)
    {
        convert(info);
    }
    
    if(!outfile)
    {
        out = stdout;
    }
    else
    {
        out = fopen(outfile, "wb");
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
* Prints out usage details for sndinfo
*/
void print_usage(int status)
{
    puts("Usage: sndchan [OPTION]... [FILE]...\n");
    puts("Reads all sound files passed as arguments and makes a single sound file where each channel corresponds to one input channel. Channel order is preserved.\n");
    puts("If no files are passed as arguments, sndchan reads from standard input. All files must have the same sample rate.\n");
    puts("OPTIONS:");
    puts("\t-c [N]\t\tOnly outputs channel N, where 1 <= N <= total number of channels.");
    puts("\t-h\t\tDisplays this help message.");
    puts("\t-o [FILE]\tSpecifies the output file name. If omitted, the file is written to standard output.");
    puts("\t-w\t\tOutput in .wav format instead of .cs229 format");
    exit(status);
}

/*
* Normalizes and layers snd2 on to snd1
*/
void layer(snd_t* snd1, snd_t* snd2)
{
    if(snd1->rate != snd2->rate)
    {
        fprintf(stderr, "sndchan: Incompatible File Error: Sample rates are not the same. Exiting.\n");
        exit(1);
    }

    if(!snd1 || !snd2)
    {
        return;
    }

    normalize(snd1, snd2);
    int total_channels = snd1->num_channels + snd2->num_channels;
    int i = 0;
    int j = 0;
    int* new_block;

    snd_dat_t* node_snd1 = snd1->data;
    snd_dat_t* node_snd2 = snd2->data;

    while(node_snd1 && node_snd2)
    {
        j = 0;
        new_block = realloc(node_snd1->channel_data, sizeof(int) * total_channels);
        check_malloc(new_block);
        
        for(i = snd1->num_channels; i < total_channels; ++i)
        {
            new_block[i] = node_snd2->channel_data[j];
            ++j;
        }

        node_snd1->channel_data = new_block;

        node_snd1 = node_snd1->next;
        node_snd2 = node_snd2->next;
    }

    snd1->num_channels = total_channels;
}

/*
* Removes all but channel channel from the sound
*/
void unlayer(snd_t* snd, int channel)
{
    snd_dat_t* node = snd->data;
    int ch_data;
    int* new_block;

    while(node)
    {
        ch_data = node->channel_data[channel];

        new_block = realloc(node->channel_data, sizeof(int));
        check_malloc(new_block);

        new_block[0] = ch_data;
        node = node->next;
    }

    snd->num_channels = 1;
}

/*
* Normalizes the bit resolution 
* and the length of snd1 and snd2
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
