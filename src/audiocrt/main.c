#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358
#endif

#define PCM_C
#include "pcm.c"
#undef PCM_C

#define WAVE_C
#include "wave.c"
#undef WAVE_C

int main()
{
    char *fname = malloc(13);
    strcpy(fname, "0audtest.pcm");
    fname[12] = 0;

    short *sarr;
    pcm_create_tone_s16(&sarr, 800, 8000, 1 , 15000, &f_sin);
    pcm_write_s16_le(fname, sarr, 8000);
    free(sarr);
    fname[0]++;

    pcm_create_tone_s16(&sarr, 800, 8000, 1 , 15000, &f_sawtooth);
    pcm_write_s16_le(fname, sarr, 8000);
    free(sarr);
    fname[0]++;

    pcm_create_tone_s16(&sarr, 800, 8000, 1 , 15000, &f_exp);
    pcm_write_s16_le(fname, sarr, 8000);
    free(sarr);
    fname[0]++;

    pcm_create_tone_s16(&sarr, 800, 8000, 1 , 15000, &f_pow2);
    pcm_write_s16_le(fname, sarr, 8000);
    free(sarr);
    fname[0]++;

    pcm_create_tone_s16(&sarr, 800, 8000, 1 , 15000, &f_sqrt);
    pcm_write_s16_le(fname, sarr, 8000);
    free(sarr);
    fname[0]++;

    pcm_create_tone_s16(&sarr, 800, 8000, 1 , 15000, &f_pulse);
    pcm_write_s16_le(fname, sarr, 8000);
    free(sarr);
    fname[0]++;

    pcm_create_tone_s16(&sarr, 800, 8000, 1 , 15000, &f_triangle);
    pcm_write_s16_le(fname, sarr, 8000);
    free(sarr);

    //Encoding:     Signed 16-bit PCM
    //Byte Order:   Little-endian
    //Channels:     1 Channel(Mono)
    //Sample rate:  8000

    return 0;
}
