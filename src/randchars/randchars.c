#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
#define print_usage() printf("usage: ./randchars [char* filename] [char* alphabet] [int - count]\n");

    if(argc <= 3)
    {
        print_usage(); 
        return 0;
    }

    int c_count = atoi(argv[3]);
    int alphabet_count = strlen(argv[2]);

    FILE *file = fopen(argv[1], "w");

    srand(time(0));

    int r_c = 0;
    for(int i = 0; i < c_count; i++)
    {
        r_c = rand() % alphabet_count;
        //rand() % 2 ? rand() % 10 + 48 : rand() % 26 + 97;
        fprintf(file, "%c", argv[2][r_c]);
    }

    fclose(file);

    return 0;
}
