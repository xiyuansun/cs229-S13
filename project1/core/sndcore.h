/*
Any generic operation having to do with sound can be found here
*/
#include "types.h"

/*
TODO Determine arguments
*/
snd_t* open_sound(char* path);
void close_sound(snd_t* sound);
void write_sound();
enum SoundType determine_type(FILE* in);
