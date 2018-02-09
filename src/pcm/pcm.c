//hdr
#ifndef PCM_H
#define PCM_H

int pcm_write_s16_le(char *fname, short *data, int size);

#endif

//src
#ifndef PCM_C

#include <stdio.h>

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

#endif
