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

#define SAMPLERATE 32000
#define DURATION 5

int main()
{
    int array_size = pcm_get_size(SAMPLERATE, DURATION);

    double arr[array_size];
    double farr[] = {{1,100}, {2,200}, {3,300}};

    pcm_scale(arr, array_size, 0);
    pcm_create_freq_array(arr, farr, 3, SAMPLERATE, DURATION, &f_sin);
    pcm_scale(arr, array_size, 30000);
    wav_write_s16_le(AUD_FILE_WAV, arr, array_size, SAMPLERATE);


    //Encoding:     Signed 16-bit PCM
    //Byte Order:   Little-endian
    //Channels:     1 Channel(Mono)
    //Sample rate:  8000

    return 0;
}
