#ifndef ABC_H
#define ABC_H

#include "types.h"

/*
* Parses the header for abc229 files, then calls
* parse_instrument for each instrument in the file.
*/
void read_header_abc229(snd_t* snd, int* mute);

/*
* Parses each instrument, and calls parse_notes to parse the score
*/
snd_t* parse_instrument(FILE* in, int num_channels, int bpm, int sr, int bits);

/*
* Parses the notes section for the instrument.
*/
void parse_notes(FILE* in, snd_t* final_inst, int bpm, char* wave_type, double pf, adsr_t en);

#endif
