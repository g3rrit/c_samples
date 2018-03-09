//hdr
#ifndef WAVE_H
#define WAVE_H

double f_sin(double x);

double f_sawtooth(double x);

double f_pow2(double x);

double f_pow2_inv(double x);

double f_pow(double x, double p);

double f_exp(double x);

double f_sqrt(double x);

double f_pulse(double x);

double f_pulse_v(double x, double v);

double f_triangle(double x);

double f_random(double x);

double f_const_1(double x);

double f_const_0(double x);

double f_const_neg1(double x);

//returns val between 0 and 1 with T = 1
double f_lin(double x);

#endif

//src
#ifndef WAVE_C

#include <math.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.14159265359
#endif

#ifndef M_E
#define M_E 2.718281828459045
#endif

#ifndef M_EI
// 1 / (1 - e^-1)
#define M_EI 1.581976707
#endif

#ifndef M_IE
// e^-1
#define M_IE 0.3678794412
#endif

//function oscelates between 0 and 1 with T = 1
inline double f_lin(double x)
{
    double intpart;
    return modf(x, &intpart);
}

inline double f_sin(double x)
{
    return sin(2 * M_PI * x);
}

inline double f_sawtooth(double x)
{
    return 2 * f_lin(x) - 1;
}

double f_pow2(double x)
{
    return 2 * pow(f_lin(x), 2) - 1;
}

double f_pow2_inv(double x)
{
    return 2 * pow(1 - f_lin(x), 2) - 1;
}

double f_pow(double x, double p)
{
    return 2 * pow(f_lin(x), p) - 1;
}

inline double f_exp(double x)
{
    return 2 * (M_EI * exp(f_lin(x) - 1) - M_EI * M_IE) - 1;
}

double f_sqrt(double x)
{
    return 2 * sqrt(f_lin(x)) - 1;
}

double f_pulse(double x)
{
    if(f_lin(x) < 0.5f)
        return -1;
    else
        return 1;
}

double f_pulse_v(double x, double v)
{
    if(f_lin(x) < v)
        return -1;
    else
        return 1;
}

double f_triangle(double x)
{
    if(f_lin(x) < 0.5f)
        return 2 * f_sawtooth(x) + 1;
    else
        return -2 * f_sawtooth(x) + 1;
}

double f_random(double x)
{
    return (double) rand()/RAND_MAX;
}

double f_const_1(double x)
{
    return 1;
}

double f_const_0(double x)
{
    return 0;
}

double f_const_neg1(double x)
{
    return -1;
}

#endif
