#ifndef GENCORE_H
#define GENCORE_H

#include "types.h"

snd_t* gen(int bits, int sr, double f, double t, double pf, char* wave_type);
snd_t* gen_sil(int bits, int sr, double t);
snd_t* gen_sin(int bits, int sr, double f, double t);
snd_t* gen_tri(int bits, int sr, double f, double t);
snd_t* gen_saw(int bits, int sr, double f, double t);
snd_t* gen_pwm(int bits, int sr, double f, double t, double pf);
void apply_adsr(snd_t* snd, double a, double d, double s, double r, double v);

#endif
