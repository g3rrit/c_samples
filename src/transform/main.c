#define TRANSFORM_C
#include "transform.c"
#undef TRANSFORM_C

#include <stdio.h>


int main()
{
    FILE *fxbfile = fopen("fxb.txt", "w");
    FILE *gxfile = fopen("gx.txt", "w");
    FILE *fxafile = fopen("fxa.txt", "w");

    double fx[500];
    double gx[500];
    int count = 0;
    for(double i = 0;  i < 5; i += 0.01f)
    {
        fx[count] = f_sin(i);
        printf("sawtooth(%f) = %f\n", i, f_sawtooth(i));
        fprintf(fxbfile, "%f\n", fx[count]);
        count++;
    }

    f_transform(fx, gx,  500, &f_sin);

    printf("after transform\n");

    for(int i = 0; i < 500; i++)
        fprintf(gxfile, "%f\n", gx[i]);

    f_transform_inverse(gx, fx,  500, &f_sin);

    printf("after transform_inverse\n");

    for(int i = 0; i < 500; i++)
        fprintf(fxafile, "%f\n", fx[i]);

    fclose(fxbfile);
    fclose(fxafile);
    fclose(gxfile);

    return 0;
}

