#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "./core/sndcore.h"

void print_usage(int status);
void concatenate(snd_t* snd1, snd_t* snd2);
void normalize(snd_t* snd1, snd_t* snd2);

int main(int argc, char* argv[])
{
    int i;
    char c, wav = 0;
    char* outfile;
    snd_t* info = 0;
    snd_t* current;
    FILE* out;

    while((c = getopt(argc, argv, "ho:w")) != -1)
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
                wav = 1;
                break;
            case '?':
                fprintf(stderr, "sndcat: Error: Try 'sndinfo -h' for more information.\n");
                exit(1);
                break;
            default:
                print_usage(1);
        }
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
                fprintf(stderr, "sndcat: Error: %s could not be opened. Exiting.\n", argv[i]);
                exit(1);
            }

            continue;
        }

        current = open_sound(argv[i]);
        if(!current)
        {
            fprintf(stderr, "sndcat: Error: %s could not be opened. Exiting.\n", argv[i]);
            exit(1);
        }
        concatenate(info, current);
        free(current);
    }
    
    if(wav)
    {
        info->type = WAVE;
    }
    else
    {
        info->type = CS229;
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
    puts("Usage: sndcat [OPTION]... [FILE]...\n");
    puts("Reads all sound files passed as arguments, concatenates the files, and writes it out");
    puts("If no files are passed as arguments, sndcat reads from standard input. All files must have the same sample rate.\n");
    puts("OPTIONS:");
    puts("\t-h\t\tDisplays this help message.");
    puts("\t-o [FILE]\tSpecifies the output file name. If omitted, the file is written to standard output.");
    puts("\t-w\t\tOutput in .wav format instead of .cs229 format");
    exit(status);
}

/*
* Normalizes and appends snd2 on to snd1
*/
void concatenate(snd_t* snd1, snd_t* snd2)
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

    snd1->num_samples += append(&(snd1->data), &(snd2->data));
}

/*
* Normalizes the number of channels
* and bit resolution of snd1 and snd2
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
}
