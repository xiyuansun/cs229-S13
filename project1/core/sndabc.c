#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "sndabc.h"
#include "util.h"
#include "gencore.h"
#include "sndcore.h"

/*
* 1) Parses the abc229 file.
* 2) Mixes the instruments together into the snd file.
*/
void read_header_abc229(snd_t* snd, int* mute)
{
    char word[11];
    snd_t* inst[16];
    int bpm = 0;
    char c;
    char read_num = 0;
    int param = 0;
    int i = 0;
    int ninst = 0;
    snd->type = WAVE;
    snd->num_channels = 1;
    snd->num_samples = 0;

    /*
    * 1) If the current character of the line is a '%', read to
    *    the end of the line, go to 1
    * 2) Else if the current character is in the alphabet, add it to word
    * 3) Else if the current character is a space and a word was read
    *    a) Read past the whitespace, get the integer parameter for the keyword
    *    b) If the keyword is TEMPO, set bpm
    *    c) Else if the keyword is INSTRUMENT, call parse_instrument()
    */
    while((c = fgetc(snd->file)) && !feof(snd->file))
    {
        if(c == '%')
        {
            while(c != '\n' && !feof(snd->file)) c = fgetc(snd->file);
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
                if(!bpm)
                {
                    fprintf(stderr, "Error parsing %s. Tempo keyword not found, or defined as 0. Exiting.\n", snd->name);
                    exit(1);
                }

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
            fprintf(stderr, "Error parsing %s. Malformed file. Exiting.\n", snd->name);
            exit(1);
        }
    }
    
    if(ninst <= 0)
    {
        fprintf(stderr, "Error parsing %s. No instruments found. Exiting.\n", snd->name);
        exit(1);
    }
    
    /*
    * 1) Find the instrument with the longest sound.
    * 2) Normalize number of samples amongst instruments.
    * 3) Mix the instruments together, taking into account mutes.
    * 4) Store the final sound in snd.
    */

    i = 0;
    int max_ind = 0;
    int max_len = 0;
    int cur_len = 0;

    while(i < ninst)
    {
        cur_len = length(inst[i]->data);
        
        if(cur_len > max_len)
        {
            max_len = cur_len;
            max_ind = i;
        }

        ++i;
    }
    
    long val;
    int min = (int) -1 * pow(2, snd->bitdepth - 1);
    int max = -1 * (min + 1);
    
    snd_dat_t* nodes[16];
    i = 0;
    while(i < ninst)
    {
        if(i != max_ind)
        {
            normalize_num_samples(inst[max_ind], inst[i]);
        }

        nodes[i] = inst[i]->data;
        ++i;
    }
    
    while(nodes[0])
    {
        nodes[0]->channel_data[0] *= !mute[0];
        i = 1;
        while(i < ninst)
        {
            val = nodes[0]->channel_data[0] + nodes[i]->channel_data[0] * !mute[i];
            nodes[0]->channel_data[0] = LIMIT(val, max, min);
            nodes[i] = nodes[i]->next;
            ++i;
        }
        nodes[0] = nodes[0]->next;
    }

    i = 1;
    while(i < ninst)
    {
        close_sound(inst[i]);
        ++i;
    }
    
    snd->num_samples = inst[0]->num_samples;
    snd->name = inst[0]->name;
    snd->data = inst[0]->data;
    snd->last = inst[0]->last;
}

/*
* 1) Creates a new sound
* 2) Parses the keywords from the instrument
* 3) Calls parse_notes()
* 4) Returns the new sound.
*/
snd_t* parse_instrument(FILE* in, int inst, int bpm, int sr, int bits)
{
    snd_t* final_inst = malloc(sizeof(snd_t));
    final_inst->num_channels = 1;
    final_inst->data = NULL;
    final_inst->last = NULL;
    final_inst->type = WAVE;
    final_inst->rate = sr;
    final_inst->bitdepth = bits;
    final_inst->num_samples = 0;
    final_inst->file = in;

    char word[10];
    char wave_type[10];
    char c;
    int i = 0;
    double pf = 0;
    adsr_t en = {0, 0, 0, 0, 0};

    while((c = fgetc(in)) && !feof(in))
    {
        if(c == '%')
        {
            while(c != '\n' && !feof(in)) c= fgetc(in);
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
                fscanf(in, "%lf", &(en.v));
            }
            else if(strncmp(word, "ATTACK", 6) == 0)
            {
                fscanf(in, "%lf", &(en.a));
            }
            else if(strncmp(word, "DECAY", 5) == 0)
            {
                fscanf(in, "%lf", &(en.d));
            }
            else if(strncmp(word, "SUSTAIN", 7) == 0)
            {
                fscanf(in, "%lf", &(en.s));
            }
            else if(strncmp(word, "RELEASE", 7) == 0)
            {
                fscanf(in, "%lf", &(en.r));
            }
            else if(strncmp(word, "PULSEFRAC", 9) == 0)
            {
                fscanf(in, "%lf", &pf);
            }
            else if(strncmp(word, "WAVEFORM", 8) == 0)
            {
                fscanf(in, "%s", wave_type);
                to_upper(wave_type);
            }
            else if(strncmp(word, "SCORE", 5) == 0)
            {
                while(c != '[') c = fgetc(in);
                parse_notes(in, final_inst, bpm, wave_type, pf, en);
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

    return final_inst;
}

/*
* 1) Parse each note
* 2) Calculate the frequency of the note
* 3) Append then note to final_inst
*/
void parse_notes(FILE* in, snd_t* final_inst, int bpm, char* wave_type, double pf, adsr_t en)
{
    const char notes[] = {'c', 'c', 'd', 'd', 'e', 'f', 'f', 'g', 'g', 'a', 'a', 'b'};
    const int base_note_ind = 9;
    const int base_freq = 440;
    double freq;
    char c;
    char reading_note = 0;
    char note;
    char sharp = 0;
    int octive_change = 0;
    double bps = bpm / 60.0;
    int n = 0;
    int d = 1;
    snd_t* cur_note = NULL;
    
    /*
    * 1) If the current character is a space or |, and we are not reading a note
    *    a) Go to 1
    * 2) If the current character is alpha and we are not reading a note
    *    a) store the character in note, modify octive/note to lowercase.
    * 3) Else if current character is # and reading note
    *    a) set sharp flag
    * 4) Else if current character is ',', modify octive and reading note
    * 5) Else if current character is ', modify octive and reading note
    * 6) Else if current character is a number and reading note
    *    a) read until not number and store in appropriate variable
    * 7) Else if current character is whitespace or | and reading note
    *    a) generate the waveform, reset variables
    */
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
            
            if('g' < note && note < 'z')
            {
                fprintf(stderr, "Unrecognized note %c. Exiting.\n", note);
                exit(1);
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
        else if((isspace(c) || c == '|') && reading_note)
        {
            if(note != 'z')
            {
                int i = 0;
                while(notes[i] != note)
                {
                    ++i;
                }
                
                if(n == 0) n = 1;

                i += sharp;
            
                int note_offset = i - base_note_ind;
                freq = base_freq * pow(2, octive_change) * pow(2, note_offset/12.0);
                cur_note = gen(final_inst->bitdepth, final_inst->rate, freq, n / (bps * d), pf, wave_type);
                apply_adsr(cur_note, en);
            }
            else
            {
                freq = 0;
                cur_note = gen_sil(final_inst->bitdepth, final_inst->rate, n / (bps * d));
            }
            
            final_inst->num_samples += append(final_inst, cur_note->data);
            reading_note = 0;
            d = 1;
            n = 0;
            sharp = 0;
            octive_change = 0;
            free(cur_note);
            cur_note = NULL;
        }
        else
        {
            fprintf(stderr, "Error parsing notes. Malformed structure. Exiting.\n");
            exit(1);
        }
    }
}
