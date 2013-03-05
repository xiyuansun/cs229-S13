#include <stdio.h>
#include "./core/sndcore.h"

int main(int argc, char* argv[])
{
    snd_t* info = open_sound("./test.cs229");
    printf("RATE:\t\t%d\n", info->rate);
    printf("SAMPLES:\t%d\n", info->num_samples);
    printf("LENGTH:\t\t%d\n", info->len);
    printf("BITRES:\t\t%d\n", info->bitdepth);
    printf("CHANNELS:\t%d\n", info->num_channels);
    printf("NAME:\t\t%s\n", info->name);
    
    return 0;
}
