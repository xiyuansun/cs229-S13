#include "sndcore.h"
#include "util.h"
#include <stdio.h>
/*

*/
snd_t* open_sound(char* path)
{

    /*
    u_int rate;
    u_int num_samples;
    u_int len;
    u_char bitdepth;
    u_char num_channels;
    */
    enum SoundType type;
    char* name;

    FILE* in = fopen(path, "rb");

    type = determine_type(in);
    name = get_filename(path);

    snd_t* ret;
    /*ret = {rate, num_samples, len, bitdepth, num_channelse, type, name, in};
    //return ret;*/
    return NULL;
}

enum SoundType determine_type(FILE* in)
{
    char type_info[5] = {0, 0, 0, 0, 0};
    fgets(type_info, 5, in);
    printf("%s\n", type_info);
    return Cs;
}

int main()
{
    FILE* in = fopen("./snd.cs229", "rb");
    printf("%d\n", determine_type(in));
    return 0;
}
