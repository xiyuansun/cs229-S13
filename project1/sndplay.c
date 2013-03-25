#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./core/gencore.h"
#include "./core/sndcore.h"
#include "./core/util.h"

const char* err_prefix = "sndplay: Error:";

void print_usage(int status);

int main(int argc, char* argv[])
{
    sndtype out_type = CS229;
    int i = 1;
    char* cur_arg;

    snd_t* info = malloc(sizeof(snd_t));
    check_malloc(info);
    info->file = stdin;
    info->bitdepth = 8;
    info->type = ABC229;
    info->rate = 4000;
    info->data = NULL;
    info->last = NULL;
    info->num_samples = 0;

    FILE* out = stdout;
    int mute[16];
    int tmp = 0;

    for(; tmp < 16; ++tmp)
    {
        mute[tmp] = 0;
    }

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
            check_bounds(argc, i, "-o", err_prefix);
            out = fopen(argv[i + 1], "wb");

            if(!out)
            {
                fprintf(stderr, "%s Could not open %s. Exiting.\n", err_prefix, argv[i + 1]);
                exit(1);
            }

            i += 2;
        }
        else if(strcmp(cur_arg, "--bits") == 0)
        {
            check_bounds(argc, i, "--bits", err_prefix);
            info->bitdepth = get_int(argv[i + 1], err_prefix);

            if(info->bitdepth != 8 || info->bitdepth != 16 || info->bitdepth != 32)
            {
                fprintf(stderr, "%s The bits parameter is not 8, 16, or 32. Exiting.\n", err_prefix);
                exit(1);
            }

            i += 2;
        }
        else if(strcmp(cur_arg, "--sr") == 0)
        {
            check_bounds(argc, i, "--sr", err_prefix);
            info->rate = get_int(argv[i + 1], err_prefix);
            i += 2;
        }
        else if(strcmp(cur_arg, "--mute") == 0)
        {
            check_bounds(argc, i, "--mute", err_prefix);
            tmp = get_int(argv[i + 1], err_prefix);
            
            if(tmp < 1 || tmp > 16)
            {
                fprintf(stderr, "%s Mute paramater is out of range. Exiting.\n", err_prefix);
                exit(1);
            }

            mute[tmp - 1] = 1;
            i += 2;
        }
        else if(info->file == stdin)
        {
            info->file = fopen(cur_arg, "rb");
            if(!(info->file))
            {
                fprintf(stderr, "%s Could not open %s for writing. Exiting.\n", err_prefix, cur_arg);
                exit(1);
            }

            ++i;
        }
        else
        {
            fprintf(stderr, "%s Unrecognized option %s. Exiting.\n", err_prefix, cur_arg);
            exit(1);
        }
    }
    
    read(&info, mute);

    if(!info)
    {
        fprintf(stderr, "%s Could not parse file. Exiting.\n", err_prefix);
        exit(1);
    }

    if(info->type != out_type)
    {
        convert(info);
    }

    write_sound(out, info);

    if(out != stdout)
    {
        fclose(out);
    }

    close_sound(info);

    return 0;
}

/*
* Prints out usage details for sndgen
*/
void print_usage(int status)
{
    puts("Usage: sndgen [OPTIONS]\n");
    puts("Produces a sound of a specified frequency and waveform using a simple ADSR envelope.\n");
    puts("STANDARD OPTIONS:");
    puts("\t-h\t\t\tDisplays this help message.");
    puts("\t-o [FILE]\t\tSpecifies the output file name. If omitted, the file is written to standard output.");
    puts("\t-w\t\t\tOutput in .wav format instead of cs229 format.");
    puts("\t--mute [N]\t\tMute instrument N, where 1 <= N <= 16.\n");
    
    puts("OTHER OPTIONS:");
    puts("\tOPTION\t\tDEFAULT\t\tDESCRIPTION");
    puts("\t--bits [N]\t8\t\tUse a bit depth of N. Must be 8, 16, or 32.");
    puts("\t--sr [N]\t4000\t\tUse a sample rate of N. Must be an integer.");
    exit(status);
}
