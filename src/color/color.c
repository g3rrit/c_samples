//hdr
#ifndef COLOR_H
#define COLOR_H

#include "stdint.h"

//typedef int32_t color

typedef char byte

struct color
{
    unsigned char a;
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

void color_init(struct color *this);

#endif

//src
#ifndef COLOR_C

void color_init(struct color *this)
{
    this->a = 0;
    this->r = 0;
    this->g = 0;
    this->b = 0;
}

#endif

