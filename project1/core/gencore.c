#include <stdlib.h>
#include <string.h>
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
    else
    {
        return NULL;
    }
}

/*
* 1) Initializes a new sound
* 2) Fills sound with correct ammount of 0s
*/
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

/*
* 1) Initializes a new sound
* 2) Calculates the datapoint for the current time
* 3) Fills the sound data with the calculated data
*/
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

/*
* 1) Initializes a new sound
* 2) Calculates the datapoint for the current time
* 3) Fills the sound data with the calculated data
*/
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

/*
* 1) Initializes a new sound
* 2) Calculates the datapoint for the current time
* 3) Fills the sound data with the calculated data
*/
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

/*
* 1) Initializes a new sound
* 2) Calculates the datapoint for the current time
* 3) Fills the sound data with the calculated data
*/
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

/*
* 1) Calculate actual duration of a, d, s, and r
* 2) Find the number of samples each section will apply to
* 3) Apply the appropriate scalar to each sample
*/
void apply_adsr(snd_t* snd, adsr_t en)
{
    snd_dat_t* node = snd->data;
    double t = 1.0 / snd->rate * snd->num_samples;
    double s_dur = t - (en.a + en.r + en.d);
    double d_dur = en.d;
    double a_dur = en.a;
    double r_dur = en.r;
    int i = 0;
    int j = 0;
    int a_samp, d_samp, s_samp, r_samp;
    double a_mult = en.v/a_dur;
    double d_mult = (en.s - en.v)/d_dur;
    double r_mult = 0;
    double cur_time = 0;
    double scale = 0;

    if(s_dur < 0)
    {
        d_dur += s_dur;
        s_dur = 0;

        if(d_dur < 0)
        {
            a_dur += d_dur;
            d_dur = 0;

            if(a_dur <= 0)
            {
                a_dur = 0;
                en.v = 0;
                en.s = 0;
            }
            else
            {
                en.v = a_mult * a_dur;
            }
        }
        else
        {
            en.v = d_mult * d_dur + en.v;
        }
    }
    
    r_mult = (-1 * en.s) / r_dur;

    a_samp = (int) (a_dur * snd->rate);
    d_samp = (int) (d_dur * snd->rate);
    s_samp = (int) (s_dur * snd->rate);
    r_samp = (int) (r_dur * snd->rate);

    while(i < a_samp && node)
    {
        cur_time = i * 1.0 / snd->rate;
        scale = a_mult * cur_time;
        for(j = 0; j < snd->num_channels; ++j)
        {
            node->channel_data[j] = (int) (node->channel_data[j] * scale);
        }
        node = node->next;
        ++i;
    }
    
    while(i < a_samp + d_samp && node)
    {
        cur_time = (i - a_samp) * 1.0 / snd->rate;
        scale = d_mult * cur_time + en.v;
        for(j = 0; j < snd->num_channels; ++j)
        {
            node->channel_data[j] = (int) (node->channel_data[j] * scale);
        }
        node = node->next;
        ++i;
    }

    while(i < s_samp + a_samp + d_samp && node)
    {
        for(j = 0; j < snd->num_channels; ++j)
        {
            node->channel_data[j] = (int) (node->channel_data[j] * en.s);
        }
        node = node->next;
        ++i;
    }

    while(i < a_samp + d_samp + s_samp + r_samp && node)
    {
        cur_time = (i - (a_samp + d_samp + s_samp)) * 1.0/snd->rate;
        scale = r_mult * cur_time + en.s;
        for(j = 0; j < snd->num_channels; ++j)
        {
            node->channel_data[j] = (int) (node->channel_data[j] * scale);
        }
        node = node->next;
        ++i;
    }

    while(i < snd->num_samples)
    {
        for(j = 0; j < snd->num_channels; ++j)
        {
            node->channel_data[j] = 0;
        }
        node = node->next;
        ++i;
    }
}
