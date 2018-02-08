#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358
#endif

#define PCM_C
#include "pcm.c"
#undef PCM_C

int main()
{
    /*
    short varr[16000];

    int count = 0;
    for(double t = 0; t < 2; t += 1./8000)      // 8000 is the samplerate
    {
        double sample = 15000 * sin(2 * M_PI * 1000 * t);   //1000 Hz sine wave
        short s16 = (short)sample;
        varr[count] = s16;
    }
    
    pcm_write("audtest.pcm", varr, 2, 16000);
    */


    FILE *file = fopen("audtest.pcm", "wb");

    double t;
    for(t = 0; t < 1; t += 1./8000)
    {
        double sample = 15000 * sin(2 * M_PI * 1000 * t);
        short s16 = (short)sample;
        unsigned char c;
        c = (unsigned)s16 % 256;
        fwrite(&c, 1, 1, file);
        c = (unsigned)s16 / 256 % 256;
        fwrite(&c, 1, 1, file);
    }

    //Encoding:     Signed 16-bit PCM
    //Byte Order:   Little-endian
    //Channels:     1 Channel(Mono)
    //Sample rate:  8000

    fclose(file);

    return 0;
}
