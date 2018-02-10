//hdr
#ifndef PCM_H
#define PCM_H

int pcm_write_s16_le(char *fname, short *data, int size);

int pcm_create_tone_s16(short **res, int frequenzy, int samplerate, double duration, double amplitude, double (*fun)(double x));

#endif

//src
#ifndef PCM_C

#define WAVE_C
#include "wave.c"
#undef WAVE_C

#include <stdio.h>
#include <stdlib.h>

int pcm_write_s16_le(char *fname, short *data, int size)
{
    //int fwrite(const void *ptr, int size, int nmemb, FILE *stream)
    FILE *file = fopen(fname, "wb");

    for(int i = 0; i < size; i++)
    {
        unsigned char c;
        c = (unsigned)data[i] % 256;
        fwrite(&c, 1, 1, file);
        c = (unsigned)data[i] / 256 % 256;
        fwrite(&c, 1, 1, file);
    }

    fclose(file);

    return 1;
}

//note:     When samplerate is to low res array is gonna be messed up
//          because the function is not gonna go to the maximum
int pcm_create_tone_s16(short **res, int frequenzy, int samplerate, double duration, double amplitude, double (*fun)(double x))
{
    int malsize = 2 * samplerate * duration;
    *res = malloc(malsize);

    int i = 0;
    double t = 0;
    for(t = 0; t < duration; t += 1./((double)samplerate))
    {
        (*res)[i] = amplitude * fun(t * frequenzy);

        i++;
    }

    return samplerate * duration;
}

#endif
