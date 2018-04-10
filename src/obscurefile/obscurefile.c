#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if(argc <= 1)
    {
        printf("usage: ./obscurefile [char* filename]\n");
        return 0;
    }
        
    srand(time(0));

    FILE *file = fopen(argv[1], "rb+");

    if(!file)
    {
        printf("couldnt open file\n");
        return 0;
    }

    fseek(file, 0L, SEEK_END);
    int file_size = ftell(file);
    rewind(file);

    printf("file[%s] size: %i\n", argv[1], file_size);

    for(int i = 0; i < 10; i++)
    {
        printf("iteration: %i\n", i);
        for(int n = 0; n < file_size; n++)
        {
            char rc = rand();
            fseek(file, n, SEEK_SET);
            fwrite(&rc, 1,1, file);
        }
        rewind(file);
    }

    printf("finished obscuring file\n");

    fclose(file);

    return 0;
}
