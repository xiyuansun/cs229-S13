#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "./core/sndcore.h"

void print_usage(int status);

int main(int argc, char* argv[])
{
    int i;
    char c;
    snd_t* info;

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
    
    /*
    Get from stdin
    */
    if(optind == argc)
    {
        info = read_sound(stdin, "Standard Input");
    }

    for(i = optind; i < argc; ++i)
    {
        info = open_sound(argv[i]);
        FILE* out = fopen("written.cs229", "wb");
        if(!info || !out) continue;
        write_sound(out, info);
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
    puts("If no files are passed as arguments, sndcat reads from standard input.\n");
    puts("OPTIONS:");
    puts("\t-h\t\tDisplays this help message.");
    puts("\t-o [FILE]\tSpecifies the output file name. If omitted, the file is written to standard output.");
    puts("\t-w\t\tOutput in .wav format instead of .cs229 format");
    exit(status);
}
