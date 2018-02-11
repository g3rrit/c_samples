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
    char *fname = malloc(9);
    strcpy(fname, "0aud.pcm");
    fname[9] = 0;

    FILE *tfile = fopen("tf.txt", "w");
    for(double l = 0; l < 1000; l += 1./100)
    {
        fprintf(tfile, "%f\n", f_sawtooth(l));
    }
    fclose(tfile);

    double *sarr;
    /*
    pcm_create_tone(&sarr, 800, 32000, 1 , 15000, &f_sin);
    pcm_write_s16_le(fname, sarr, 32000);
    free(sarr);
    fname[0]++;
    */

    pcm_create_tone(&sarr, 800, 8000, 1 , 15000, &f_sawtooth);
    pcm_write_s16_le(fname, sarr, 8000);
    wav_write_s16_le("twav.wav", sarr, 8000, 8000);
    free(sarr);
    fname[0]++;

    /*
    pcm_create_tone(&sarr, 800, 8000, 1 , 15000, &f_exp);
    pcm_write_s16_le(fname, sarr, 8000);
    free(sarr);
    fname[0]++;

    pcm_create_tone(&sarr, 800, 8000, 1 , 15000, &f_pow2);
    pcm_write_s16_le(fname, sarr, 8000);
    free(sarr);
    fname[0]++;

    pcm_create_tone(&sarr, 800, 8000, 1 , 15000, &f_sqrt);
    pcm_write_s16_le(fname, sarr, 8000);
    free(sarr);
    fname[0]++;

    pcm_create_tone(&sarr, 800, 8000, 1 , 15000, &f_pulse);
    pcm_write_s16_le(fname, sarr, 8000);
    free(sarr);
    fname[0]++;

    pcm_create_tone(&sarr, 800, 8000, 1 , 15000, &f_triangle);
    pcm_write_s16_le(fname, sarr, 8000);
    free(sarr);
    */

    //Encoding:     Signed 16-bit PCM
    //Byte Order:   Little-endian
    //Channels:     1 Channel(Mono)
    //Sample rate:  8000

    return 0;
}
