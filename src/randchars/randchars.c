#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
#define print_usage() printf("usage: ./randchars [char* filename]  [int - count]\n");

    if(argc <= 2)
    {
        print_usage(); 
        return 0;
    }

    int c_count = atoi(argv[2]);

    FILE *file = fopen(argv[1], "w");

    srand(time(0));

    char r_c = 0;
    for(int i = 0; i < c_count; i++)
    {
        r_c = rand() % 2 ? rand() % 10 + 48 : rand() % 26 + 97;
        fprintf(file, "%c", r_c);
    }

    fclose(file);

    return 0;
}
