#ifndef ABC_H
#define ABC_H

#include "types.h"


void read_header_abc229(snd_t* snd);
snd_dat_t* parse_instrument(FILE* in, int num_channels, int bpm, int sr, int bits);
void parse_notes(FILE* in, snd_t* final_inst, int bpm);

#endif
