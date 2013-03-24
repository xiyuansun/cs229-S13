#include <stdio.h>
#include <stdlib.h>
#include "core/sndabc.h"

int main(int argc, char* argv[])
{
    snd_t* info = malloc(sizeof(snd_t));
    info->name = "Instruments";
    info->file = fopen("test.abc229", "rb");
    
    read_header_abc229(info);

    return 0;
}
