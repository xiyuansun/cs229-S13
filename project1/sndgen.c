#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "./core/sndcore.h"

const char* err_prefix = "sndgen: Error:";

void print_usage(int status);
u_int get_int(char* arg);
double get_real(char* arg);
void check_bounds(int argc, int i, char* opt);
snd_t* gen_sin(int bits, int sr, double f, double t, double v);
snd_t* gen_tri(int bits, int sr, double f, double t, double v);
snd_t* gen_saw(int bits, int sr, double f, double t, double v);
snd_t* gen_pwm(int bits, int sr, double f, double t, double v, double pf);

int main(int argc, char* argv[])
{
    sndtype out_type = CS229;
    int i = 1;
    char* cur_arg;
    snd_t* info = 0;
    snd_t* current;

    FILE* out = stdout;
    int bits = 8;
    int sr = 500;

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
            check_bounds(argc, i, "-o");
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
            check_bounds(argc, i, "--bits");
            bits = get_int(argv[i + 1]);

            if(bits != 8 || bits != 16 || bits != 32)
            {
                fprintf(stderr, "%s The bits parameter is not 8, 16, or 32. Exiting.\n", err_prefix);
                exit(1);
            }

            i += 2;
        }
        else if(strcmp(cur_arg, "--sr") == 0)
        {
            check_bounds(argc, i, "--sr");
            sr = get_int(argv[i + 1]);
            i += 2;
        }
        else if(strcmp(cur_arg, "-f") == 0)
        {
            check_bounds(argc, i, "-f");
            f = get_real(argv[i + 1]);
            i += 2;
        }
        else if(strcmp(cur_arg, "-t") == 0)
        {
            check_bounds(argc, i, "-t");
            t = get_real(argv[i + 1]);
            i += 2;
        }
        else if(strcmp(cur_arg, "-v") == 0)
        {
            check_bounds(argc, i, "-v");
            v = get_real(argv[i + 1]);
            
            if(v < 0 || v > 1)
            {
                fprintf(stderr, "%s The the v parameter is not in range of [0, 1]. Exiting\n", err_prefix);
                exit(1);
            }
        }
        else if(strcmp(cur_arg, "-a"))
        {
            check_bounds(argc, i, "-a");
            a = get_real(argv[i + 1]);
            i += 2;
        }
        else if(strcmp(cur_arg, "-d") == 0)
        {
            check_bounds(argc, i, "-d");
            d = get_real(argv[i + 1]);
            i += 2;
        }
        else if(strcmp(cur_arg, "-s") == 0)
        {
            check_bounds(argc, i, "-s");
            s = get_real(argv[i + 1]);
            
            if(s < 0 || s > 1)
            {
                fprintf(stderr, "%s The the s parameter is not in range of [0, 1]. Exiting\n", err_prefix);
                exit(1);
            }
        }
        else if(strcmp(cur_arg, "-r") == 0)
        {
            check_bounds(argc, i, "-r");
            r = get_real(argv[i + 1]);
            i += 2;
        }
        else if(strcmp(cur_arg, "--sine") == 0)
        {
            sine = 1;
            triangle = sawtooth = pulse = 0;
        }
        else if(strcmp(cur_arg, "--triangle") == 0)
        {
            triangle = 1;
            sine = sawtooth = pulse = 0;
        }
        else if(strcmp(cur_arg, "--sawtooth") == 0)
        {
            sawtooth = 1;
            sine = triangle = pulse = 0;
        }
        else if(strcmp(cur_arg, "--pulse") == 0)
        {
            pulse = 1;
            sine = triangle = sawtooth = 0;
        }
        else if(strcmp(cur_arg, "--pf") == 0)
        {
            check_bounds(argc, i, "--pf");
            pf = get_real(argv[i + 1]);
            
            if(s < 0 || s > 1)
            {
                fprintf(stderr, "%s The the pf parameter is not in range of [0, 1]. Exiting\n", err_prefix);
                exit(1);
            }
        }
        else
        {
            fprintf(stderr, "%s Unrecongnized argument '%s'. Exiting.\n", err_prefix);
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
    puts("\t--sr [N]\t500\t\tUse a sample rate of N. Must be an integer.");
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

u_int get_int(char* arg)
{
    char* endptr;
    u_int ret = (int) strtol(arg, &endptr, 10);

    if(*endptr != '\0')
    {
        fprintf(stderr, "%s %s is not an integer. Exiting.\n", err_prefix, arg);
        exit(1);
    }

    return ret;
}

double get_real(char* arg)
{
    char* endptr;
    double ret = strtod(arg, &endptr);

    if(*endptr != '\0')
    {
        fprintf(stderr, "%s %s is not an integer. Exiting.\n", err_prefix, arg);
        exit(1);
    }

    return ret;
}

void check_bounds(int argc, int i, char* opt)
{
    if(i + 1 >= argc)
    {
        fprintf(stderr, "%s %s was specified without an argument. Exiting.\n", err_prefix, opt);
        exit(1);
    }
}
