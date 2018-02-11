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

#define TRANSFORM_C
#include "transform.c"
#undef TRANSFORM_C

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
    pcm_create_tone(&sarr, 800, 16000, 1 , 15000, &f_sin);
    wav_write_s16_le(fname, sarr, size_of_arr, 16000);
    free(sarr);
    fname[0]++;
    */

    int size_of_arr = pcm_create_tone(&sarr, 800, 16000, 5, &f_sin);
    printf("sizeof array is %i\n", size_of_arr);
    pcm_scale_const(sarr, size_of_arr, 15000);
    //pcm_write_s16_le(fname, sarr, 8000);
    wav_write_s16_le("twav.wav", sarr, size_of_arr, 16000);
    free(sarr);
    fname[0]++;

    double farr[16385];
    for(int i = 0; i < 16385; i++)
        farr[i] = 0;
    for(int i = 16384; i > 0; i/=2)
    {
        //farr[i] = exp((0.25 * abs(i)) - 600);
        farr[i] = 1;
    }
    size_of_arr = pcm_create_frequenzy_tone(&sarr, farr, 16385, 32000, 5, &f_triangle);
    pcm_scale(sarr, size_of_arr, &f_sin);
    pcm_scale_const(sarr, size_of_arr, 15000);
    wav_write_s16_le("fwav.wav", sarr, size_of_arr, 32000);
    pcm_write_txt("fwavtf.txt", farr, 16385);
    pcm_write_txt("fwavt.txt", sarr, size_of_arr);
    free(sarr);



    /*
    pcm_create_tone(&sarr, 800, 8000, 1 , 15000, &f_exp);
    wav_write_s16_le(fname, sarr, size_of_arr, 16000);
    free(sarr);
    fname[0]++;

    pcm_create_tone(&sarr, 800, 8000, 1 , 15000, &f_pow2);
    wav_write_s16_le(fname, sarr, size_of_arr, 16000);
    free(sarr);
    fname[0]++;

    pcm_create_tone(&sarr, 800, 8000, 1 , 15000, &f_sqrt);
    wav_write_s16_le(fname, sarr, size_of_arr, 16000);
    free(sarr);
    fname[0]++;

    pcm_create_tone(&sarr, 800, 8000, 1 , 15000, &f_pulse);
    wav_write_s16_le(fname, sarr, size_of_arr, 16000);
    free(sarr);
    fname[0]++;

    pcm_create_tone(&sarr, 800, 8000, 1 , 15000, &f_triangle);
    wav_write_s16_le(fname, sarr, size_of_arr, 16000);
    free(sarr);
    */

    //Encoding:     Signed 16-bit PCM
    //Byte Order:   Little-endian
    //Channels:     1 Channel(Mono)
    //Sample rate:  8000

    return 0;
}
