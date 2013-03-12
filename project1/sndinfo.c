#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "./core/sndcore.h"

void print_usage(int status);
void print_info(snd_t* info);

/*
* Reads sound files passed to it from shell and prints out the info
*   1) Checks for any important switches and acts accordingly
*   2) Checks the number of other arguments
*       a) If none, read from stdin
*       b) If some, open them as files and read them
*   3) Print out info for any valid file
*/
int main(int argc, char* argv[])
{
    int i;
    char c;

    while((c = getopt(argc, argv, "h")) != -1)
    {
        switch(c)
        {
            case 'h':
                print_usage(0);
                break;
            case '?':
                fprintf(stderr, "Try 'sndinfo -h' for more information.\n");
                exit(1);
                break;
            default:
                print_usage(1);
        }
    }
    
    //Get from stdin
    if(optind == argc)
    {
        snd_t* info = read_sound(stdin, "Standard Input");
        print_info(info);
    }

    for(i = optind; i < argc; ++i)
    {
        snd_t* info = open_sound(argv[i]);

        if(!info) continue;

        print_info(info);
        printf("Here");
        fflush(stdin);
        close_sound(info);
        
        if(i != argc - 1)
        {
            printf("\n");
        }
    }
    
    return 0;
}


/*
* Prints out usage details for sndinfo
*/
void print_usage(int status)
{
    puts("Usage: sndinfo [OPTION]... [FILE]...\n");
    puts("Reads all sound files passed as arguments and, for each one, displays the");
    puts("following:");
    puts("\t-The file name");
    puts("\t-The file type (.cs229 or .wav)");
    puts("\t-The sample rate");
    puts("\t-The bit depth");
    puts("\t-The number of channels");
    puts("\t-The number of samples");
    puts("\t-The length of the sound (in seconds, rounded down)");
    puts("If no files are passed as arguments, sndinfo reads from standard input.\n");
    puts("OPTIONS:");
    puts("\t-h\tDisplays this help message.\n");
    exit(status);
}

/*
* Prints out sound information
*/
void print_info(snd_t* info)
{
    char* type = (info->type == WAVE) ? "wav" : "cs229";
    printf("Information for %s:\n", info->name);
    printf("\tType:\t\t%s\n", type);
    printf("\tRate:\t\t%d\n", info->rate);
    printf("\tBit Depth:\t%d\n", info->bitdepth);
    printf("\tChannels:\t%d\n", info->num_channels);
    printf("\tSamples:\t%d\n", info->num_samples);
    printf("\tLength:\t\t%d\n", info->len);
}
