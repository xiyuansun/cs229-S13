#ifndef GENCORE_H
#define GENCORE_H

#include "types.h"

/*
* Calls the appropriate wave generator, based on wave_type
*/
snd_t* gen(int bits, int sr, double f, double t, double pf, char* wave_type);

/*
* Generates t seconds of silence at sr sample rate
*/
snd_t* gen_sil(int bits, int sr, double t);

/*
* Generates a sine wave of frequency f and time t,
* with bit depth bits and sample rate sr.
*/
snd_t* gen_sin(int bits, int sr, double f, double t);

/*
* Generates a triangle wave of frequency f and
* time t, with bit depth bits and sample rate sr.
*/
snd_t* gen_tri(int bits, int sr, double f, double t);

/*
* Generates a sawtooth wave of frequency f and
* time t, with bit depth bits and sample rate sr.
*/
snd_t* gen_saw(int bits, int sr, double f, double t);

/*
* Generates a pulse wave of frequency f
* and time t, with bit depth bits, sample
* rate sr, and pluse fraction pf.
*/
snd_t* gen_pwm(int bits, int sr, double f, double t, double pf);

/*
* Applies the ADSR envelope en to the sound data in node.
*/
void apply_adsr(snd_dat_t* node, adsr_t* en);

#endif
