#include <stdio.h>
#include <stdlib.h>
#include "core/sndabc.h"

int main(int argc, char* argv[])
{
    snd_t* info = malloc(sizeof(snd_t));
    info->name = "Instruments";
    info->file = fopen("test.abc229", "rb");
    info->bitdepth = 8;
    info->rate = 4000;
    
    read_header_abc229(info);
    FILE* out = fopen("test.wav", "wb");
    write_sound(out, info);

    return 0;
}
