//hdr
#ifndef PCM_H
#define PCM_H

int pcm_write(char *fname, void *data, int size, int nmemb);

#endif

//src
#ifndef PCM_C

#include <stdio.h>

int pcm_write(char *fname, void *data, int size, int nmemb)
{
    //int fwrite(const void *ptr, int size, int nmemb, FILE *stream)
    FILE *file = fopen(fname, "wb");

    int retval = 0;

    if(fwrite(data, size, nmemb, file) == nmemb)
        retval = 1;
    else
        retval = 0;

    fclose(file);

    return retval;
}

#endif
