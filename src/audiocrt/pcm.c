//hdr
#ifndef PCM_H
#define PCM_H

// ----- FILE CREATION -----

int pcm_write_txt(char *fname, double *data, int size);

int pcm_write_s16_le(char *fname, double *data, int size);

int wav_write_s16_le(char *fname, double *data, int size, int samplerate);

// ----- FILE CREATION -----

// ----- ARRAY CREATION -----

int pcm_create(double *res, int frequenzy, int samplerate, double duration, double (*fun)(double x));

int pcm_get_size(int samplerate, double duration);

void pcm_add_f(double *res, int start_off_x, int end_off_x, int start_off_y, int end_off_y, double (*fun)(double x));

void pcm_mult_f(double *res, int start_off_x, int end_off_x, int start_off_y, int end_off_y, double (*fun)(double x));

void pcm_add(double *dest, double *src, int off_dest, int s_off_src, int e_off_src);

void pcm_mult(double *dest, double *src, int off_dest, int s_off_src, int e_off_src);

//
int pcm_create_frequenzy_tone(double *res, double *input, int isize, int samplerate, double duration, double (*fun)(double x));

//where input[i][0] is the amplitude and input[i][1] is the frequenzy
int pcm_create_freq_array(double *res, double *input, int isize, int samplerate, double duration, double (*fun)(double x));

void pcm_scale(double *res, int size, double amplitude);

void pcm_scale_unit(double *data, int size);

// ----- ARRAY CREATION -----

//---------- endianess ---------- 
int is_big_endian();
void reverse_endianness(const long long int size, void *value);
void to_big_endian(const long long int size, void *value);
void to_little_endian(const long long int size, void *value);


#endif

//src
#ifndef PCM_C

#ifndef WAVE_C
#define WAVE_C
#include "wave.c"
#undef WAVE_C
#endif

#include <stdio.h>
#include <stdlib.h>

int pcm_write_txt(char *fname, double *data, int size)
{
    FILE *file = fopen(fname, "w");

    for(int i = 0; i < size; i++)
        fprintf(file, "%f\n", data[i]);

    fclose(file);

    return 1;
}

int pcm_write_s16_le(char *fname, double *data, int size)
{
    //int fwrite(const void *ptr, int size, int nmemb, FILE *stream)
    FILE *file = fopen(fname, "wb");

    for(int i = 0; i < size; i++)
    {
        short s16 = data[i];
        unsigned char c;
        c = (unsigned)s16 % 256;
        fwrite(&c, 1, 1, file);
        c = (unsigned)s16 / 256 % 256;
        fwrite(&c, 1, 1, file);
    }

    fclose(file);

    return 1;
}

int wav_write_s16_le(char *fname, double *data, int size, int samplerate)
{
    struct 
    {
        //riff wave header
        char chunk_id[4];
        int chunk_size;
        char format[4];

        //format subchunk
        char sub_chunk1_id[4];
        int sub_chunk1_size;
        short int audio_format;
        short int num_channels;
        int samplerate;
        int byterate;
        short int blockalign;
        short int bits_per_sample;
        //short int extra_param_size;
        
        //data subchunk
        char sub_chunk2_id[4];
        int sub_chunk2_size; 
    } wave_header;

    //fill header
    //riff wave header
    wave_header.chunk_id[0] = 'R';
    wave_header.chunk_id[1] = 'I';
    wave_header.chunk_id[2] = 'F';
    wave_header.chunk_id[3] = 'F';
    wave_header.format[0] = 'W';
    wave_header.format[1] = 'A';
    wave_header.format[2] = 'V';
    wave_header.format[3] = 'E';

    //format subchunks
    wave_header.sub_chunk1_id[0] = 'f';
    wave_header.sub_chunk1_id[1] = 'm';
    wave_header.sub_chunk1_id[2] = 't';
    wave_header.sub_chunk1_id[3] = ' ';
    wave_header.audio_format = 1;           //1 for pcm
    wave_header.num_channels = 1;           //mono
    wave_header.samplerate = samplerate;
    wave_header.bits_per_sample = 16;       //16 bits 
    
    wave_header.byterate = wave_header.samplerate * wave_header.num_channels * wave_header.bits_per_sample / 8;
    wave_header.blockalign = wave_header.num_channels * wave_header.bits_per_sample/8;

    //data subchunks
    wave_header.sub_chunk2_id[0] = 'd';
    wave_header.sub_chunk2_id[1] = 'a';
    wave_header.sub_chunk2_id[2] = 't';
    wave_header.sub_chunk2_id[3] = 'a';

    wave_header.chunk_size = 4+8+16+8+0;
    wave_header.sub_chunk1_size = 16;
    wave_header.sub_chunk2_size = 0;

    //set duration
    //note:     not quite sure if this is right
    long long int total_bytes = (long long int) wave_header.num_channels * size * wave_header.bits_per_sample/8;
    wave_header.chunk_size = 4+8+16+8+total_bytes;
    wave_header.sub_chunk2_size = total_bytes;

    //convert header to little endian
    to_little_endian(sizeof(int), &(wave_header.chunk_size));
    to_little_endian(sizeof(int), &(wave_header.sub_chunk1_size));
    to_little_endian(sizeof(short int), &(wave_header.audio_format));
    to_little_endian(sizeof(short int), &(wave_header.num_channels));
    to_little_endian(sizeof(int), &(wave_header.samplerate));
    to_little_endian(sizeof(int), &(wave_header.byterate));
    to_little_endian(sizeof(short int), &(wave_header.blockalign));
    to_little_endian(sizeof(short int), &(wave_header.bits_per_sample));
    to_little_endian(sizeof(int), &(wave_header.sub_chunk2_size));

    //write to file
    FILE *file = fopen(fname, "wb");

    fwrite(&wave_header, sizeof(wave_header), 1, file);

    //16bit
    for(int i = 0; i < size; i++)
    {
        short s16 = data[i];
        unsigned char c;
        c = (unsigned)s16 % 256;
        fwrite(&c, 1, 1, file);
        c = (unsigned)s16 / 256 % 256;
        fwrite(&c, 1, 1, file);
    }

    fclose(file);

    return 1;
}


//note:     When samplerate is to low res array is gonna be messed up
//          because the function is not gonna go to the maximum
int pcm_create(double *res, int frequenzy, int samplerate, double duration, double (*fun)(double x))
{
    int i = 0;
    double t = 0;
    for(t = 0; t < duration; t += 1./((double)samplerate))
    {
        res[i] = 2 * fun(t * frequenzy) - 1;
        i++;
    }

    return samplerate * duration;
}

int pcm_get_size(int samplerate, double duration)
{
    return samplerate * duration;
}

void pcm_add_f(double *res, int start_off_x, int end_off_x, int start_off_y, int end_off_y, double (*fun)(double x))
{
    int count = start_off_x;
    double dt = 1./(end_off_x-start_off_x);
    double scale_f = ((double)(end_off_y-start_off_y))/2;
    for(double i = 0; i < 1; i += dt)
    {
        res[count] += scale_f * fun(i) + start_off_y;
        count++;
    }
}

void pcm_mult_f(double *res, int start_off_x, int end_off_x, int start_off_y, int end_off_y, double (*fun)(double x))
{
    int count = start_off_x;
    double dt = 1./(end_off_x-start_off_x);
    double scale_f = ((double)(end_off_y-start_off_y))/2;
    for(double i = 0; i < 1; i += dt)
    {
        res[count] *= scale_f * fun(i) + start_off_y;
        count++;
    }
}

void pcm_add(double *dest, double *src, int off_dest, int s_off_src, int e_off_src)
{
    for(int i = s_off_src; i < e_off_src; i++)
        dest[i + off_dest] += src[i];
}

void pcm_mult(double *dest, double *src, int off_dest, int s_off_src, int e_off_src)
{
    for(int i = s_off_src; i < e_off_src; i++)
        dest[i + off_dest] *= src[i];
}


int pcm_create_frequenzy_tone(double *res, double *input, int isize, int samplerate, double duration, double (*fun)(double x))
{
    double sum_of_all = 0;
    for(int i = 0; i < isize; i++)
        sum_of_all += input[i];

    int array_size = samplerate * duration;

    int count = 1;
    for(double t = 0; t < duration; t+= 1./(double) samplerate)
    {
        printf("sound(%i - %i)\r", count, array_size);

        double fx = input[0];

        for(int i = 0; i < isize; i++)
            fx += (input[i] * fun(t * i))/sum_of_all;

        res[count] = fx;

        count++;
    }

    printf("\n");

    return array_size;
}

int pcm_create_freq_array(double *res, double *input, int isize, int samplerate, double duration, double (*fun)(double x))
{
    double sum_of_all = 0;
    for(int i = 0; i < isize; i++)
        sum_of_all += input[i * 2];

    int array_size = samplerate * duration;

    int count = 0;
    for(double t = 0; t < duration; t+= 1./(double) samplerate)
    {
        printf("sound(%i - %i)\r", count, array_size);

        double fx = 0;
        for(int i = 0; i < isize; i++)
            fx += (2 * input[i * 2] * fun(t * input[i * 2 + 1]) - 1)/sum_of_all;

        res[count] = fx;

        count++;
    }

    printf("\n");

    return array_size;

}

void pcm_scale(double *res, int size, double amplitude)
{
    for(int i = 0; i < size; i++)
        res[i] *= amplitude;
}

void pcm_scale_unit(double *data, int size)
{
    double biggest_val = 0;
    for(int i = 0; i < size; i++)
    {
        if(data[i] > biggest_val)
            biggest_val = data[i];
    }

    if(biggest_val != 1)
        pcm_scale(data, size, 1./biggest_val);
}



//---------- endianess ---------- 

int is_big_endian()
{
    int test = 1;
    char *p = (char*)&test;
    return p[0] == 0;
}

void reverse_endianness(const long long int size, void *value)
{
    int i;
    char result[32];
    for(i = 0; i < size; i++)
        result[i] = ((char*)value)[size-i-1];
    for(i = 0; i < size; i++)
        ((char*)value)[i] = result[i];
}

void to_big_endian(const long long int size, void *value)
{
    char needsfix = !( (1 && is_big_endian()) || (0 && !is_big_endian()) );
    if(needsfix)
        reverse_endianness(size, value);
}

void to_little_endian(const long long int size, void *value)
{
    char needsfix = !( (0 && is_big_endian()) || (1 && !is_big_endian()) );
    if(needsfix)
        reverse_endianness(size, value);
}


#endif
