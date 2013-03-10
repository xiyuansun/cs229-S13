#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "./core/sndcore.h"

void print_usage(int status);

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
    
    if(optind == argc);

    for(i = optind; i < argc; ++i)
    {
        snd_t* info = open_sound(argv[i]);
        printf("RATE:\t\t%d\n", info->rate);
        printf("SAMPLES:\t%d\n", info->num_samples);
        printf("LENGTH:\t\t%d\n", info->len);
        printf("BITRES:\t\t%d\n", info->bitdepth);
        printf("CHANNELS:\t%d\n", info->num_channels);
        printf("NAME:\t\t%s\n", info->name);
    }
    
    return 0;
}

void print_usage(int status)
{
    puts("Usage: sndinfo [OPTION]... [FILE]...\n");
    puts("Reads all sound files passed as arguments and, for each one, displays the\nfollowing:");
    puts("\t-The file name");
    puts("\t-The file type (.cs229 or .wav)");
    puts("\t-The sample rate");
    puts("\t-The bit depth");
    puts("\t-The number of channels");
    puts("\t-The number of samples");
    puts("\t-The length of the sound (in seconds)");
    puts("If no files are passed as arguments, sndinfo reads from standard input.\n");
    puts("OPTIONS:");
    puts("\t-h\tDisplays this help message.\n");
    exit(status);
}
