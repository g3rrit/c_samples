#define FFT_C
#include "fft.c"
#undef FFT_C


int main()
{
    cplx buf[] = {1,1,1,1,0,0,0,0};

    show_data("Data: \n", buf);
    fft(buf, 8);
    show_data("\nFFT: \n", buf);
    printf("\n");

    return 0;
}
