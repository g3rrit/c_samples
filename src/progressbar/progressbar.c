//hdr
#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

void progressbar_redraw(char *name, int size, double value);

void progressbar_draw(char *name, int size, double value);

#endif

//src
#ifndef PROGRESSBAR_C

void progressbar_redraw(char *name, int size, double value)
{
    printf("\r");
    progressbar_draw(name, size, value);
}

void progressbar_draw(char *name, int size, double value)
{
    printf("%s[%.2f]: [", name, value * 100);
    for(int i = 0 ; i < size; i++)
    {
        if(i <= value * size)
            printf("%c", '#');
        else
            printf("%c", ' ');
    }
    printf("%c", ']');
}

#endif
