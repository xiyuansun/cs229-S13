#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sndwav.h"
#include "util.h"
#include "sndcore.h"

int read_header_wav(snd_t* snd)
{
    u_int remaining_bytes = read_little_dat(snd->file, 4);
    char* format = malloc(sizeof(char) * 5);
    check_malloc(format);

    fread(format, 4, 1, snd->file);
    format[4] = 0;

    if(strcmp(to_upper(format), "WAVE"))
    {
        free(format);
        return 1;
    }
    
    /*get the format chunk*/
    while(!feof(snd->file))
    {
        fread(format, 4, 1, snd->file);
        format[4] = 0;
        
        remaining_bytes = read_little_dat(snd->file, 4);

        if(strcmp(format, "fmt "))
        {
            while(remaining_bytes)
            {
                fgetc(snd->file);
                --remaining_bytes;
            }
            continue;
        }
        
        int audio_format = read_little_dat(snd->file, 2);

        /*If not PCM, can't read*/
        if(audio_format != 1)
        {
            return 1;
        }
        
        snd->num_channels = read_little_dat(snd->file, 2);
        snd->rate = read_little_dat(snd->file, 4);
        
        /*ByteRate and BlockAlign, not useful*/
        read_little_dat(snd->file, 4);
        read_little_dat(snd->file, 2);
        
        snd->bitdepth = read_little_dat(snd->file, 2);
        remaining_bytes -= 16;

        while(remaining_bytes)
        {
            fgetc(snd->file);
            --remaining_bytes;
        }

        break;
    }

    free(format);

    if(snd->rate == (u_int) -1) return 1;
    return 0;
}

int read_info_wav(snd_t* snd)
{
    u_int remaining_bytes;

    char* format = malloc(sizeof(char) * 5);
    check_malloc(format);

    while(!feof(snd->file))
    {
        fread(format, 4, 1, snd->file);
        format[4] = 0;
        
        remaining_bytes = read_little_dat(snd->file, 4);

        if(strcmp(format, "data"))
        {
            while(remaining_bytes)
            {
                fgetc(snd->file);
                --remaining_bytes;
            }
            continue;
        }

        snd->num_samples = (u_int) (remaining_bytes * 8.0 / (snd->num_channels * snd->bitdepth));

        int offset = (snd->bitdepth == 8) ? -128 : 0;
        int read_bytes = snd->bitdepth / 8;
        int i = 0;
        snd_dat_t* node;

        while(remaining_bytes)
        {
            if(i == 0)
            {
                node = new_node(snd->num_channels);
            }

            node->channel_data[i] = read_little_dat(snd->file, read_bytes) + offset;
            remaining_bytes -= read_bytes;
            ++i;

            if(i >= (snd->num_channels))
            {
                add(&(snd->data), &node);
                i = 0;
            }
        }
        break;
    }

    free(format);

    if(length(snd->data) <= 0) return 1;
    return 0;
}

void write_wav(FILE* out, snd_t* snd)
{
    int bytedepth = snd->bitdepth / 8;
    int block_align = snd->num_channels * bytedepth;
    int byte_rate = block_align * snd->rate;
    int data_size = block_align * snd->num_samples;
    int fmt_size = 16;
    int size = data_size + fmt_size + 20;
    int offset = (snd->bitdepth == 8) ? 128 : 0;
    snd_dat_t* node = snd->data;
    int i;
    
    printf("%d\n%d\n%d\n", size, fmt_size, data_size);

    write_bytes(out, "RIFF", 4, 0);
    write_bytes(out, to_little_char_arr(size, 4), 4, 1);
    write_bytes(out, "WAVE", 4, 0);

    write_bytes(out, "fmt ", 4, 0);
    write_bytes(out, to_little_char_arr(fmt_size, 4), 4, 1);
    write_bytes(out, to_little_char_arr(1, 2), 2, 1);
    write_bytes(out, to_little_char_arr(snd->num_channels, 2), 2, 1);
    write_bytes(out, to_little_char_arr(snd->rate, 4), 4, 1);
    write_bytes(out, to_little_char_arr(byte_rate, 4), 4, 1);
    write_bytes(out, to_little_char_arr(block_align, 2), 2, 1);
    write_bytes(out, to_little_char_arr(snd->bitdepth, 2), 2, 1);

    write_bytes(out, "data", 4, 0);
    write_bytes(out, to_little_char_arr(data_size, 4), 4, 1);
    
    while(node)
    {
        for(i = 0; i < snd->num_channels; ++i)
        {
            write_bytes(out, to_little_char_arr(node->channel_data[i] + offset, bytedepth), bytedepth, 1);
        }
        
        node = node->next;
    }
}
