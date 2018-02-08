#define LINEAREQ_C
#include "lineareq.c"
#undef LINEAREQ_C

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void test_prim();

void test_sin();

int main()
{
    test_prim();
    return 0;
}

void test_sin()
{
    double mat[2550];
    double out[50];

    srand(time(0));
    for(int i = 0; i < 2550; i++)
    {
        int rnum = rand() % 2550; 

        double dnum = ((double)rnum)/2550;

        printf("random double %f\n", dnum);

        mat[i] = dnum;
    }

    matrix_print(mat, 50, 51);
    printf("matrix solvable? 1 - yes ... 0 - no ----: %i\n", solve_linear(mat, out, 50, 51));
    matrix_print(mat, 50, 51);
    matrix_print(out, 1, 50);
}

void test_prim()
{
    FILE *pfile = fopen("./src/lineareq/prim.txt", "r");

    char *numbuf = malloc(100);
    size_t psize = 100;

    double mat[49];

    for(int i = 0; i < 49; i++)
    {
        getline(&numbuf, &psize, pfile); 

        mat[i] = (double) atoi(numbuf);
    }

    free(numbuf);

    fclose(pfile);

    matrix_print(mat, 6, 7);
    double out[6];
    printf("matrix solvable? 1 - yes ... 0 - no ----: %i\n", solve_linear(mat, out, 6, 7));
    matrix_print(mat, 6, 7);
    matrix_print(out, 1, 6);
}
