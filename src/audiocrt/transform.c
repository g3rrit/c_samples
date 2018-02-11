//hdr
#ifndef TRANDFORM_H
#define TRANSFORM_H

//fun should oscelate between -1 and 1
//fun should have an wavelength of 1
void f_transform(double *input, double *output, int size, double (*fun)(double x));

//void f_transform_inverse(double *input, double *output, int size, double (*fun)(double x));

void f_transform_frequenzy(double *input, double *output, int isize, int osize, double(*fun)(double x));

#endif

//hdr
#ifndef TRANSFORM_C

#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265359
#endif

void f_transform(double *input, double *output, int size, double (*fun)(double x))
{
    register double dx = 0.01;

    output[0] = 0;
    for(int i = 1; i < size; i++)
    {
        register double id = (double)i;

        register double bk = 0;
        for(double xval = 0; xval < size; xval += dx)
            bk += (fun((1/id) * xval) * fun((1/id) * xval)) * dx;

        register double ck = 0;
        for(double xval = 0; xval < size; xval += dx)
            ck += (input[(int)ceil(xval)] * fun((1/id) * xval)) * dx;

        ck /= bk;

        output[i] = ck;
    }
}

/*
 * deprecated
void f_transform_inverse(double *input, double *output, int size, double (*fun)(double x))
{
    //double dx = 0.001;

    for(int i = 0; i < size; i++)
    {
        printf("in transform_inverse (%i/%i)\n", i, size);

        register double fx = 0;

        for(int xval = 1; xval < size; xval++)
            fx += (input[xval] * fun((1/(double) xval) * i));

        output[i] = fx;
    }
}
*/

void f_transform_frequenzy(double *input, double *output, int isize, int osize, double(*fun)(double x))
{
    double sum_of_all = 0;

    for(int i = 0; i < isize; i++)
        sum_of_all += input[i];

    for(int i = 0; i < osize; i++)
    {
        double fx = input[0];
        for(int xval = 1; xval < isize; xval++)
        {
            fx += (input[xval] * fun((1/(double) xval) * i));
            fx /= sum_of_all;
        }
        output[i] = fx;
    }
}

#endif
