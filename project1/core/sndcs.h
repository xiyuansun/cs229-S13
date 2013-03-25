#ifndef CS229_H
#define CS229_H

#include "types.h"


/*
* Reads header data from cs229 file
*/
int read_header_cs229(snd_t* snd);

/*
* Reads sample data from cs229 file
*/
void read_info_cs229(snd_t* snd);

/*
* Writes out a cs229 file
*/
void write_cs229(FILE* out, snd_t* snd);
#endif
