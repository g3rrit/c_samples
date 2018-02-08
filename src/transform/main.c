#define TRANSFORM_C
#include "transform.c"
#undef TRANSFORM_C

#include <stdio.h>


int main()
{
    FILE *fxbfile = fopen("fxb.txt", "w");
    FILE *gxfile = fopen("gx.txt", "w");
    FILE *fxafile = fopen("fxa.txt", "w");

    double fx[2000];
    double gx[2000];
    int count = 0;
    for(double i = 0;  i < 20; i += 0.01f)
    {
        fx[count] = f_sin(i);
        printf("sawtooth(%f) = %f\n", i, f_sawtooth(i));
        fprintf(fxbfile, "%f\n", fx[count]);
        count++;
    }

    f_transform(fx, gx,  2000, &f_sin);

    printf("after transform\n");

    for(int i = 0; i < 2000; i++)
        fprintf(gxfile, "%f\n", gx[i]);

    f_transform_inverse(gx, fx,  2000, &f_sin);

    printf("after transform_inverse\n");

    for(int i = 0; i < 2000; i++)
        fprintf(fxafile, "%f\n", fx[i]);

    fclose(fxbfile);
    fclose(fxafile);
    fclose(gxfile);

    return 0;
}

