#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core/gencore.h"
#include "core/sndcore.h"
#include "core/util.h"

const char* err_prefix = "sndgen: Error:";

void print_usage(int status);

int main(int argc, char* argv[])
{
    sndtype out_type = CS229;
    int i = 1;
    char* cur_arg;
    snd_t* info = 0;

    FILE* out = stdout;
    int bits = 8;
    int sr = 4000;

    double f = 2.0;
    double t = 5.0;
    double v = 1.0;

    double a = 1.0;
    double d = 1.0;
    double s = .5;
    double r = 1.0;

    char sine = 1;
    char triangle = 0;
    char sawtooth = 0;
    char pulse = 0;
    double pf = .5;

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
            bits = get_int(argv[i + 1], err_prefix);

            if(bits != 8 || bits != 16 || bits != 32)
            {
                fprintf(stderr, "%s The bits parameter is not 8, 16, or 32. Exiting.\n", err_prefix);
                exit(1);
            }

            i += 2;
        }
        else if(strcmp(cur_arg, "--sr") == 0)
        {
            check_bounds(argc, i, "--sr", err_prefix);
            sr = get_int(argv[i + 1], err_prefix);
            i += 2;
        }
        else if(strcmp(cur_arg, "-f") == 0)
        {
            check_bounds(argc, i, "-f", err_prefix);
            f = get_real(argv[i + 1], err_prefix);
            i += 2;
        }
        else if(strcmp(cur_arg, "-t") == 0)
        {
            check_bounds(argc, i, "-t", err_prefix);
            t = get_real(argv[i + 1], err_prefix);
            i += 2;
        }
        else if(strcmp(cur_arg, "-v") == 0)
        {
            check_bounds(argc, i, "-v", err_prefix);
            v = get_real(argv[i + 1], err_prefix);
            
            if(v < 0 || v > 1)
            {
                fprintf(stderr, "%s The the v parameter is not in range of [0, 1]. Exiting\n", err_prefix);
                exit(1);
            }
            
            i += 2;
        }
        else if(strcmp(cur_arg, "-a") == 0)
        {
            check_bounds(argc, i, "-a", err_prefix);
            a = get_real(argv[i + 1], err_prefix);
            i += 2;
        }
        else if(strcmp(cur_arg, "-d") == 0)
        {
            check_bounds(argc, i, "-d", err_prefix);
            d = get_real(argv[i + 1], err_prefix);
            i += 2;
        }
        else if(strcmp(cur_arg, "-s") == 0)
        {
            check_bounds(argc, i, "-s", err_prefix);
            s = get_real(argv[i + 1], err_prefix);
            
            if(s < 0 || s > 1)
            {
                fprintf(stderr, "%s The the s parameter is not in range of [0, 1]. Exiting\n", err_prefix);
                exit(1);
            }

            i += 2;
        }
        else if(strcmp(cur_arg, "-r") == 0)
        {
            check_bounds(argc, i, "-r", err_prefix);
            r = get_real(argv[i + 1], err_prefix);
            i += 2;
        }
        else if(strcmp(cur_arg, "--sine") == 0)
        {
            sine = 1;
            triangle = sawtooth = pulse = 0;
            ++i;
        }
        else if(strcmp(cur_arg, "--triangle") == 0)
        {
            triangle = 1;
            sine = sawtooth = pulse = 0;
            ++i;
        }
        else if(strcmp(cur_arg, "--sawtooth") == 0)
        {
            sawtooth = 1;
            sine = triangle = pulse = 0;
            ++i;
        }
        else if(strcmp(cur_arg, "--pulse") == 0)
        {
            pulse = 1;
            sine = triangle = sawtooth = 0;
            ++i;
        }
        else if(strcmp(cur_arg, "--pf") == 0)
        {
            check_bounds(argc, i, "--pf", err_prefix);
            pf = get_real(argv[i + 1], err_prefix);
            
            if(s < 0 || s > 1)
            {
                fprintf(stderr, "%s The the pf parameter is not in range of [0, 1]. Exiting\n", err_prefix);
                exit(1);
            }
            
            i += 2;
        }
        else
        {
            fprintf(stderr, "%s Unrecongnized argument '%s'. Exiting.\n", err_prefix, argv[i]);
        }
    }
    
    adsr_t en = {a, d, s, r, v};
    if(sine)
    {
        info = gen_sin(bits, sr, f, t);
    }
    else if(triangle)
    {
        fflush(stdout);
        info = gen_tri(bits, sr, f, t);
    }
    else if(sawtooth)
    {
        info = gen_saw(bits, sr, f, t);
    }
    else
    {
        info = gen_pwm(bits, sr, f, t, pf);
    }

    apply_adsr(info, en);

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
* Prints out usage details for sndgen
*/
void print_usage(int status)
{
    puts("Usage: sndgen [OPTIONS]\n");
    puts("Produces a sound of a specified frequency and waveform using a simple ADSR envelope.\n");
    puts("STANDARD OPTIONS:");
    puts("\t-h\t\t\tDisplays this help message.");
    puts("\t-o [FILE]\t\tSpecifies the output file name. If omitted, the file is written to standard output.");
    puts("\t-w\t\t\tOutput in .wav format instead of cs229 format.\n");
    
    puts("OTHER OPTIONS:");
    puts("\tOPTION\t\tDEFAULT\t\tDESCRIPTION");
    puts("\t--bits [N]\t8\t\tUse a bit depth of N. Must be 8, 16, or 32.");
    puts("\t--sr [N]\t4000\t\tUse a sample rate of N. Must be an integer.");
    puts("\t-f [R]\t\t2.0\t\tUse a frequency of R Hz.");
    puts("\t-t [R]\t\t5.0\t\tSet the total duration of the sound, in seconds.");
    puts("\t-v [P]\t\t1.0\t\tSet the peak volume in the range of [0, 1].\n");

    puts("\t-a [R]\t\t1.0\t\tSet the attack time.");
    puts("\t-d [R]\t\t1.0\t\tSet the decay time.");
    puts("\t-s [P]\t\t0.5\t\tSet the sustain volume in the range of [0, 1].");
    puts("\t-r [R]\t\t1.0\t\tSet the release time.\n");

    puts("\t--sine\t\tYes\t\tGenerate a sine wave.");
    puts("\t--triangle\tNo\t\tGenerate a triangle wave.");
    puts("\t--sawtooth\tNo\t\tGenerate a sawtooth wave.");
    puts("\t--pulse\t\tNo\t\tGenerate a pulse wave.");
    puts("\t--pf [P]\t0.5\t\tFraction of the time the pulse is 'up' in range of [0, 1]");
    exit(status);
}
