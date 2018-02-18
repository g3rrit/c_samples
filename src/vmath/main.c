#define VMATH_C
#include "vmath.c"
#undef VMATH_C


int main()
{
#define print_vec(v) printf("vector: (%f|%f|%f)\n", v.x,v.y,v.z)

    struct vec3 v1 = { 2 , 3, 4};
    struct vec3 v2 = { 3 , 1, 3};

    print_vec(v1);
    print_vec(v2);

    vec3_add(&v1,&v2);
    printf("v1 + v2 = %f|%f|%f\n", v1.x,v1.y,v1.z);

    return 0;
}
