#include <stdlib.h>
#include <math.h>
#include "gencore.h"
#include "util.h"
#include "sndcore.h"

snd_t* gen(int bits, int sr, double f, double t, double pf, char* wave_type)
{
    if(strcmp(wave_type, "SINE") == 0)
    {
        return gen_sin(bits, sr, f, t);
    }
    else if(strcmp(wave_type, "TRIANGLE") == 0)
    {
        return gen_tri(bits, sr, f, t);
    }
    else if(strcmp(wave_type, "SAWTOOTH") == 0)
    {
        return gen_saw(bits, sr, f, t);
    }
    else if(strcmp(wave_type, "PULSE") == 0)
    {
        return gen_pwm(bits, sr, f, t, pf);
    }
}

snd_t* gen_sil(int bits, int sr, double t)
{
    snd_t* ret = malloc(sizeof(snd_t));
    ret->bitdepth = (u_char) bits;
    ret->rate = sr;
    ret->num_channels = 1;
    ret->type = WAVE;
    ret->len = (int) t;
    ret->num_samples = 0;
    ret->name = "Generated Sound";
    ret->file = NULL;
    ret->data = NULL;
    ret->last = NULL;

    int total_samples = (int) sr * t;
    snd_dat_t* node;

    while(ret->num_samples < total_samples);
    {
        node = new_node(ret->num_channels);
        node->channel_data[0] = 0;
        add(ret, node);
        ++(ret->num_samples);
    }

    return ret;
}

snd_t* gen_sin(int bits, int sr, double f, double t)
{
    snd_t* ret = malloc(sizeof(snd_t));
    ret->bitdepth = (u_char) bits;
    ret->rate = sr;
    ret->num_channels = 1;
    ret->type = WAVE;
    ret->len = (int) t;
    ret->name = "Generated Sound";
    ret->file = NULL;
    ret->data = NULL;
    ret->last = NULL;
    ret->num_samples = 0;

    int min = (int) -1 * pow(2, bits - 1);
    int max = -1 * (min + 1);
    long mult = (long) pow(2, bits - 1);
    double incr = 1.0/sr;
    double cur_time = 0;
    int total_samples = (int) sr * t;
    
    snd_dat_t* node;
    int val;

    while(ret->num_samples < total_samples)
    {
        cur_time = (ret->num_samples) / (1.0 * sr);
        node = new_node(ret->num_channels);
        val = ((int) mult * sin(2 * M_PI * f * cur_time));
        node->channel_data[0] = LIMIT(val, max, min);
        add(ret, node);
        ++(ret->num_samples);
    }

    return ret;
}

snd_t* gen_tri(int bits, int sr, double f, double t)
{
    snd_t* ret = malloc(sizeof(snd_t));
    ret->bitdepth = (u_char) bits;
    ret->rate = sr;
    ret->num_channels = 1;
    ret->type = WAVE;
    ret->len = (int) t;
    ret->name = "Generated Sound";
    ret->file = NULL;
    ret->data = NULL;
    ret->last = NULL;
    ret->num_samples = 0;

    int min = (int) -1 * pow(2, bits - 1);
    int max = -1 * (min + 1);
    long mult = (long) pow(2, bits - 1);
    double incr = 1.0/sr;
    double period = 1.0/f;
    double cur_time = 0;
    int total_samples = (int) sr * t;

    snd_dat_t* node;
    int val;
    double half_period = period/2;

    while(ret->num_samples < total_samples)
    {
        cur_time = (ret->num_samples) / (1.0 * sr);
        node = new_node(ret->num_channels);
        int fl = (int) floor(cur_time/half_period + 1.0/2.0);

        val = (int) (mult * (2/half_period) * (cur_time - half_period * fl) * pow(-1, (double) fl));

        node->channel_data[0] = LIMIT(val, max, min);
        add(ret, node);
        ++(ret->num_samples);
        cur_time += incr;
    }
    return ret;
}

snd_t* gen_saw(int bits, int sr, double f, double t)
{
    snd_t* ret = malloc(sizeof(snd_t));
    ret->bitdepth = (u_char) bits;
    ret->rate = sr;
    ret->num_channels = 1;
    ret->type = WAVE;
    ret->len = (int) t;
    ret->name = "Generated Sound";
    ret->file = NULL;
    ret->data = NULL;
    ret->last = NULL;
    ret->num_samples = 0;

    int min = (int) -1 * pow(2, bits - 1);
    int max = -1 * (min + 1);
    long mult = (long) pow(2, bits - 1);
    double incr = 1.0/sr;
    double period = 1.0/f;
    double cur_time = 0;
    int total_samples = (int) sr * t;

    snd_dat_t* node;
    int val;

    while(ret->num_samples < total_samples)
    {
        cur_time = (ret->num_samples) / (1.0 * sr);
        node = new_node(ret->num_channels);

        int fl = (int) floor(cur_time/period + 1.0/2.0);

        val = (int) (mult * (2/period) * (cur_time - period * fl));

        node->channel_data[0] = LIMIT(val, max, min);
        add(ret, node);
        ++(ret->num_samples);
        cur_time += incr;
    }
    return ret;
}

snd_t* gen_pwm(int bits, int sr, double f, double t, double pf)
{
    snd_t* ret = malloc(sizeof(snd_t));
    ret->bitdepth = (u_char) bits;
    ret->rate = sr;
    ret->num_channels = 1;
    ret->type = WAVE;
    ret->len = (int) t;
    ret->name = "Generated Sound";
    ret->file = NULL;
    ret->data = NULL;
    ret->last = NULL;
    ret->num_samples = 0;

    int min = (int) -1 * pow(2, bits - 1);
    int max = -1 * (min + 1);
    double incr = 1.0/sr;
    double period = 1.0/f;
    double cur_time = 0;
    int total_samples = (int) sr * t;

    snd_dat_t* node;
    int val;

    while(ret->num_samples < total_samples)
    {
        cur_time = (ret->num_samples) / (1.0 * sr);
        node = new_node(ret->num_channels);

        int fl = (int) floor(cur_time/period);

        if(cur_time - period * fl < pf * period)
        {
            val = max;
        }
        else
        {
            val = min;
        }

        node->channel_data[0] = LIMIT(val, max, min);
        add(ret, node);
        ++(ret->num_samples);
        cur_time += incr;
    }
    return ret;
}

void apply_adsr(snd_t* snd, double a, double d, double s, double r, double v)
{
}
