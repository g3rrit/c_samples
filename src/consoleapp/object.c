//hdr
#ifndef OBJECT_H
#define OBJECT_H

struct object
{
    void *data;
    void (*draw)(void *this);
};

void object_init(struct object *this, void *data, void (*draw)(void *data));

void object_draw(struct object *this);

#define text_field_width 7
#define text_field_height 3

struct text_field
{
    char text[5];
    int xpos;
    int ypos;
};

void text_field_init(struct text_field *this, char *_text, int xpos, int ypos);

void text_field_draw(void *this);

#endif

//src
#ifndef OBJECT_C

// --- object

void object_draw(struct object *this)
{
    this->draw(this->data);
}

void object_init(struct object *this, void *data, void (*draw)(void *data))
{
    this->data = data;
    this->draw = draw;
}

// --- text_field

void text_field_init(struct text_field *this, char *_text, int xpos, int ypos)
{
    memcpy(this->text, _text, 4);
    this->text[4] = 0;

    this->xpos = xpos;
    this->ypos = ypos;
}

void text_field_draw(void *this)
{
    struct text_field *pthis = this;

    printf("in textfielddraw ypos: %i\n", pthis->ypos);
}


#endif
