//hdr
#ifndef S_FFT_H
#define S_FFT_H

#include <complex.h>
typedef double complex cplx;

#define PI 3.141594

void _fft(cplx buf[], cplx out[], int n, int step);

void fft(cplx buf[], int n);

void show_data(const char * s, cplx buf[]);

#endif

//src
#ifndef S_FFT_C

#include <math.h>

void _fft(cplx buf[], cplx out[], int n, int step)
{
	if (step < n)
    {
		_fft(out, buf, n, step * 2);
		_fft(out + step, buf + step, n, step * 2);

		for (int i = 0; i < n; i += 2 * step)
        {
			cplx t = cexp(-I * PI * i / n) * out[i + step];
			buf[i / 2]     = out[i] + t;
			buf[(i + n)/2] = out[i] - t;
		}
	}
}

void fft(cplx buf[], int n)
{
	cplx out[n];
	for (int i = 0; i < n; i++)
        out[i] = buf[i];

	_fft(buf, out, n, 1);
}


void show_data(const char * s, cplx buf[]) {
	printf("%s", s);
	for (int i = 0; i < 8; i++)
		if (!cimag(buf[i]))
			printf("%g ", creal(buf[i]));
		else
			printf("(%g, %g) ", creal(buf[i]), cimag(buf[i]));
}

#endif
