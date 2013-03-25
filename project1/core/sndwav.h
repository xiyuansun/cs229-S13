#ifndef WAV_H
#define WAV_H

#include "types.h"

/*
* Reads data from fmt  chunk
*/
int read_header_wav(snd_t* snd);

/*
* Reads data from data chunk
*/
int read_info_wav(snd_t* snd);

/*
* Writes out wave file
*/
void write_wav(FILE* out, snd_t* snd);
#endif
