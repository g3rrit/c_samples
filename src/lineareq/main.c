#define LINEAREQ_C
#include "lineareq.c"
#undef LINEAREQ_C

#include <stdio.h>
#include <string.h>

int main()
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
    return 0;
}
