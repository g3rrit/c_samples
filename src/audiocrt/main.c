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

#define AUD_FILE_PCM "output/aud.pcm"
#define AUD_FILE_WAV "output/audwav.wav"
#define FREQ_FILE_TXT "output/freq.txt"
#define AUD_FILE_TXT "output/audwave.txt"

int main()
{

    double *sarr;
    double *farr;

    /* SIN TONE 800 hz sample rate: 16000 duration: 5
    int size_of_arr = pcm_create(&sarr, 800, 16000, 5, &f_sin);
    pcm_scale_const(sarr, size_of_arr, 15000);
    pcm_write_s16_le(AUD_FILE_PCM, sarr, size_of_arr);
    wav_write_s16_le("twav.wav", sarr, size_of_arr, 16000);
    free(sarr);
    */

    /* TRIANGLE TONE 800 hz sample rate: 16000 duration: 5
    int size_of_arr = pcm_create(&sarr, 800, 16000, 5, &f_triangle);
    pcm_scale_const(sarr, size_of_arr, 15000);
    pcm_write_s16_le(AUD_FILE_PCM, sarr, size_of_arr);
    wav_write_s16_le(AUD_FILE_WAV, sarr, size_of_arr, 16000);
    free(sarr);
    */

    /* TRAINGLE FREQU TONE 800 hz sample rate: 16000 duration: 5
    farr = malloc(sizeof(double) * 1000);
    pcm_fill(farr, 0, 500, &f_pow2);
    pcm_fill(farr, 500, 1000, &f_pow2_inv);
    pcm_add_f(farr, 1000, &f_const_1);
    pcm_scale_unit(farr, 1000);
    pcm_write_txt(FREQ_FILE_TXT, farr, 1000);
    int size_of_arr = pcm_create_frequenzy_tone(&sarr, farr, 1000, 16000, 5, &f_sin);
    pcm_scale_const(sarr, size_of_arr, 40000);
    pcm_write_txt(AUD_FILE_TXT, sarr, size_of_arr);
    wav_write_s16_le(AUD_FILE_WAV, sarr, size_of_arr, 16000);
    free(farr);
    free(sarr);
    */

    ///*ORGEL OLD
    farr = malloc(sizeof(double) * 16385);
    for(int i = 0; i < 16285; i++)
        farr[i] = 0;

    for(int i = 16384; i > 0; i /= 2)
    {
        farr[i] = 1;
    }
    int size_of_arr = pcm_create_frequenzy_tone(&sarr, farr, 16385, 32000, 3, &f_sin);
    pcm_scale_const(sarr ,size_of_arr, 30000);
    pcm_write_txt(FREQ_FILE_TXT, farr, 16385);
    pcm_write_txt(AUD_FILE_TXT, sarr, size_of_arr);
    wav_write_s16_le(AUD_FILE_WAV, sarr, size_of_arr, 32000);
    free(farr);
    free(sarr);
    //*/

    /* ORGEL NEW
    int size_of_arr = pcm_create(&sarr, 800, 64000, 5, &f_const_0);
    double *sin_arr; 
    for(int i = 16384; i > 0; i /= 2)
    {
        pcm_create(&sin_arr, i, 64000, 5, &f_sin);
        pcm_add(sarr, size_of_arr, sin_arr, size_of_arr, 0);
        free(sin_arr);
    }
    pcm_scale_unit(sarr, size_of_arr);
    pcm_scale_const(sarr, size_of_arr, 30000);
    pcm_write_txt(AUD_FILE_TXT, sarr, size_of_arr);
    wav_write_s16_le(AUD_FILE_WAV, sarr, size_of_arr, 64000);
    free(sarr);
    */

    /* BASS TEST
    int size_of_arr = pcm_create(&sarr, 200, 32000, 5, &f_sin);
    double *sin_arr;
    pcm_create(&sin_arr, 70, 32000, 5, &f_sin);
    pcm_mult(sarr, size_of_arr, sin_arr, size_of_arr, 0);
    double *sin2_arr;
    pcm_create(&sin2_arr, 3, 32000, 5, &f_pow2);
    pcm_mult(sarr, size_of_arr, sin2_arr, size_of_arr, 0);
    pcm_scale_unit(sarr, size_of_arr);
    pcm_scale_const(sarr, size_of_arr, 30000);
    pcm_write_txt(AUD_FILE_TXT, sarr, size_of_arr);
    wav_write_s16_le(AUD_FILE_WAV, sarr, size_of_arr, 32000);
    free(sarr);
    free(sin_arr);
    */

    /* RAND
    int size_of_arr = pcm_create(&sarr, 100, 64000, 5, &f_random);
    double *sin_arr;
    pcm_create(&sin_arr, 200, 64000, 5, &f_sin);
    pcm_mult(sarr, size_of_arr, sin_arr, size_of_arr, 0);
    pcm_scale_unit(sarr, size_of_arr);
    pcm_scale_const(sarr, size_of_arr, 30000);
    pcm_write_txt(AUD_FILE_TXT, sarr, size_of_arr);
    wav_write_s16_le(AUD_FILE_WAV, sarr, size_of_arr, 32000);
    free(sarr);
    free(sin_arr);
    */

    /* BASS TEST 2
    int size_of_arr = pcm_create(&sarr, 100, 64000, 5, &f_sawtooth);
    double *sin_arr = malloc(sizeof(double) * size_of_arr);
    pcm_fill_f(sin_arr, &f_sin, 0, size_of_arr/5, 0, 1);
    pcm_fill_f(sin_arr, &f_sawtooth, size_of_arr/5, (2*size_of_arr)/5, 0, 1);
    pcm_fill_f(sin_arr, &f_triangle, (2*size_of_arr)/5, (3*size_of_arr)/5, 0, 1);
    pcm_fill_f(sin_arr, &f_exp, (3*size_of_arr)/5, (4*size_of_arr)/5, 0, 1);
    pcm_fill_f(sin_arr, &f_pow2, (4*size_of_arr)/5, size_of_arr, 0, 1);
    pcm_mult(sarr, size_of_arr, sin_arr, size_of_arr, 0);
    pcm_scale_unit(sarr, size_of_arr);
    pcm_scale_const(sarr, size_of_arr, 15000);
    pcm_write_txt(FREQ_FILE_TXT, sin_arr, size_of_arr);
    pcm_write_txt(AUD_FILE_TXT, sarr, size_of_arr);
    wav_write_s16_le(AUD_FILE_WAV, sarr, size_of_arr, 64000);
    free(sarr);
    free(sin_arr);
    */

    /*
    int size_of_arr = pcm_create(&sarr, 300, 64000, 5, &f_pow2);
    double *sin_arr;
    pcm_create(&sin_arr, 600, 64000, 5, &f_sin);
    pcm_add(sarr, size_of_arr, sin_arr, size_of_arr, 0);
    pcm_scale_unit(sarr, size_of_arr);
    pcm_scale_const(sarr, size_of_arr, 30000);
    pcm_write_txt(AUD_FILE_TXT, sarr, size_of_arr);
    wav_write_s16_le(AUD_FILE_WAV, sarr, size_of_arr, 32000);
    free(sarr);
    free(sin_arr);
    */



    /*
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
    */


    //Encoding:     Signed 16-bit PCM
    //Byte Order:   Little-endian
    //Channels:     1 Channel(Mono)
    //Sample rate:  8000

    return 0;
}
