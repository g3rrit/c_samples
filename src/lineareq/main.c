#define LINEAREQ_C
#include "lineareq.c"
#undef LINEAREQ_C

int main()
{
    double mat[] = {1,2,3,4,21,22,23,24,31,32,33,34};
    double out[4];
    printf("out at 0 , 1 : %d\n", solve_linear(mat, out, 3, 4));
    return 0;
}
