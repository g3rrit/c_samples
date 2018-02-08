//hdr
#ifndef TRANDFORM_H
#define TRANSFORM_H

double f_sin(double x);

double f_sawtooth(double x);

//fun should oscelate between -1 and 1
//fun should have an wavelength of 1
void f_transform(double *input, double *output, int size, double (*fun)(double x));

void f_transform_inverse(double *input, double *output, int size, double (*fun)(double x));

#endif

//hdr
#ifndef TRANSFORM_C

#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265359
#endif

inline double f_sin(double x)
{
    return sin(2 * M_PI * x);
}

inline double f_sawtooth(double x)
{
    double intpart;
    return 2 * modf(x, &intpart) - 1;
}

void f_transform(double *input, double *output, int size, double (*fun)(double x))
{
    register double dx = 0.01;

    output[0] = 0;
    for(int i = 1; i < size; i++)
    {
        register double id = (double)i;
        printf("in transform (%i/%i)\n", i, size);

        register double bk = 0;
        for(double xval = 0; xval < size; xval += dx)
            bk += (fun((1/id) * xval) * fun((1/id) * xval)) * dx;

        printf("bk: %f\n", bk);

        register double ck = 0;
        for(double xval = 0; xval < size; xval += dx)
            ck += (input[(int)ceil(xval)] * fun((1/id) * xval)) * dx;

        ck /= bk;

        printf("ck: %f\n", ck);

        output[i] = ck;
    }
}

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

#endif
