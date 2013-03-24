#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "sndabc.h"
#include "util.h"

void read_header_abc229(snd_t* snd)
{
    char word[11];
    snd_dat_t* inst[16];
    int bpm = 240;
    char c;
    char read_num = 0;
    int param = 0;
    int i = 0;
    int ninst = 0;

    while((c = fgetc(snd->file)) && !feof(snd->file))
    {
        if(c == '%')
        {
            while(c != '\n') c = fgetc(snd->file);
            continue;
        }

        if(is_alpha(c) && i < 10)
        {
            word[i] = c;
            ++i;
        }
        else if(isspace(c) && i > 0)
        {
            word[i] = 0;
            to_upper(word);
            
            while(isspace(c) && !feof(snd->file)) c = fgetc(snd->file);
            
            while('0' <= c && c <= '9')
            {
                read_num = 1;
                param *= 10;
                param += (c - '0');
                c = fgetc(snd->file);
            }

            if(!read_num)
            {
                fprintf(stderr, "Error parsing %s. Expected integer after %s. Exiting\n", snd->name, word);
                exit(1);
            }

            if(strncmp(word, "TEMPO", 5) == 0)
            {
                bpm = param;
            }
            else if(strncmp(word, "INSTRUMENT", 10) == 0)
            {
                if(param != ninst)
                {
                    fprintf(stderr, "Error parsing %s. Expected instrument %d, got %d. Exiting.\n", snd->name, ninst, param);
                    exit(1);
                }
                
                if(ninst >= 16)
                {
                    fprintf(stderr, "Error parsing %s. Too many instruments. Exiting.\n", snd->name);
                    exit(1);
                }

                inst[ninst] = parse_instrument(snd->file, ninst, bpm, snd->rate, snd->bitdepth);
                ++ninst;
            }
            else
            {
                fprintf(stderr, "Error parsing %s. Unexpected keyword %s. Exiting.\n", snd->name, word);
                exit(1);
            }

            i = 0;
            read_num = 0;
            param = 0;
        }
        else if(!isspace(c) || i != 0)
        {
            printf("%d %c %s\n", i, c, word);
            fprintf(stderr, "Error parsing %s. Malformed file. Exiting.\n", snd->name);
            exit(1);
        }
    }

    printf("bpm: %d\n", bpm);
    printf("ninsts: %d\n", ninst);
}

snd_dat_t* parse_instrument(FILE* in, int inst, int bpm, int sr, int bits)
{
    snd_t* final_inst = malloc(sizeof(snd_t));
    final_inst->num_channels = 1;
    final_inst->data = NULL;
    final_inst->last = NULL;
    final_inst->type = WAVE;
    final_inst->rate = sr;
    final_inst->bitdepth = bits;
    final_inst->num_samples = 0;

    char word[10];
    char c;
    int read_num = 0;
    int i = 0;
    double v, a, d, s, r, pf;

    while((c = fgetc(in)) && !feof(in))
    {
        if(c == '%')
        {
            while(c != '\n') c= fgetc(in);
            continue;
        }
        
        if(is_alpha(c) && i < 9)
        {
            word[i] = c;
            ++i;
        }
        else if(isspace(c) && i > 0)
        {
            word[i] = 0;
            to_upper(word);

            if(strncmp(word, "VOLUME", 6) == 0)
            {
                fscanf(in, "%lf", &v);
                printf("V: %lf\n", v);
            }
            else if(strncmp(word, "ATTACK", 6) == 0)
            {
                fscanf(in, "%lf", &a);
                printf("A: %lf\n", a);
            }
            else if(strncmp(word, "DECAY", 5) == 0)
            {
                fscanf(in, "%lf", &d);
                printf("D: %lf\n", d);
            }
            else if(strncmp(word, "SUSTAIN", 7) == 0)
            {
                fscanf(in, "%lf", &s);
                printf("S: %lf\n", s);
            }
            else if(strncmp(word, "RELEASE", 7) == 0)
            {
                fscanf(in, "%lf", &r);
                printf("R: %lf\n", r);
            }
            else if(strncmp(word, "PULSEFRAC", 9) == 0)
            {
                fscanf(in, "%lf", &pf);
                printf("PF: %lf\n", pf);
            }
            else if(strncmp(word, "WAVEFORM", 8) == 0)
            {
                fscanf(in, "%s", word);
                printf("WAVEFORM: %s\n", word);
            }
            else if(strncmp(word, "SCORE", 5) == 0)
            {
                while(c != '[') c = fgetc(in);
                parse_notes(in, final_inst, bpm);
                printf("\n");
                break;
            }
            else
            {
                fprintf(stderr, "Error parsing instrument %d. Unexpected keyword %s. Exiting\n", inst, word);
                exit(1);
            }

            i = 0;
        }
        else if(!isspace(c) || i != 0)
        {
            fprintf(stderr, "Error parsing instrument %d. Malformed instrument. Exiting.\n", inst);
            exit(1);
        }
    }
}

void parse_notes(FILE* in, snd_t* final_inst, int bpm)
{
    char c;
    char reading_note = 0;
    char note;
    char sharp = 0;
    int octive_change = 0;
    double bps = bpm / 60. //YOU WERE HERE
    double count = 1.0;
    int n = 0;
    int d = 1;
    

    while((c = fgetc(in)) && c != ']')
    {
        if((isspace(c) || c == '|') && !reading_note)
        {
            continue;
        }
        else if(is_alpha(c) && !reading_note)
        {
            reading_note = 1;
            note = c;
            if(('A' <= c && c <= 'Z'))
            {
                --octive_change;
                note += ('a'-'A');
            }
        }
        else if(c == '#' && reading_note)
        {
            if(note == 'b' || note == 'e')
            {
                fprintf(stderr, "Error parsing notes. %c cannot be sharp. Exiting.\n", note);
                exit(1);
            }

            sharp = 1;
        }
        else if(c == ',' && reading_note)
        {
            --octive_change;
        }
        else if(c == '\'' && reading_note)
        {
            ++octive_change;
        }
        else if('0' <= c && c <= '9' && reading_note)
        {
            int param = 0;
            while('0' <= c && c <= '9')
            {
                param *= 10;
                param += (c - '0');
                c = fgetc(in);
            }

            ungetc(c, in);

            if(param > 16)
            {
                fprintf(stderr, "Error parsing notes. %d is out of range. Exiting.\n", param);
                exit(1);
            }

            if(!n)
            {
                n = param;
            }
            else if(param != 1 || param != 2 || param != 4 || param != 8 || param != 16)
            {
                fprintf(stderr, "Error parsing notes. %d is not a valid value. Exiting.\n", param);
            }
            else
            {
                d = param;
            }
        }
        else if(c == '/' && reading_note)
        {
            continue;
        }
        else if(isspace(c) && reading_note)
        {
            printf("NOTE: %c\n", note);
            printf("SHARP: %d\n", sharp);
            printf("OCTIVE: %d\n", octive_change);
            printf("N: %d\n", n);
            printf("D: %d\n", d);
            printf("\n");
            reading_note = 0;
            d = 1;
            n = 0;
            sharp = 0;
            octive_change = 0;
        }
        else
        {
            printf("%c %c %d %d %d %d\n", c, note, sharp, octive_change, n, d);
            fprintf(stderr, "Error parsing notes. Malformed structure. Exiting.\n");
            exit(1);
        }
    }
}
