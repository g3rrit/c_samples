#include "stdio.h"

#define CONTAINER_C
#include "container.c"
#undef CONTAINER_C

int omain()
{
    struct list *mlist = malloc(sizeof(struct list));

    list_init(mlist);

    //hello shiny big world !!!!
    list_push_back(mlist, "wold");
    list_push_front(mlist, "big");
    list_push_front(mlist, "shiny");
    list_push_front(mlist, "hello");
    list_push_back(mlist, "!!!!");

    for(int i = 0; i < mlist->size; i++)
    {
        printf("%s ", list_at(mlist, i));
    }
    printf("\n");


    char *shiny = list_remove_at(mlist, 1);     //shiny
    char *big = list_at(mlist, 1);              //big
    printf("big: %s\n", big);

    printf("%s ", list_pop_front(mlist));
    printf("%s ", shiny);
    printf("%s ", big);
    list_pop_front(mlist);
    printf("%s ", list_pop_front(mlist));
    printf("%s ", list_pop_front(mlist));
    printf("\nlist size: %i\n", mlist->size);

    printf("wtf");
    list_delete(mlist);

    printf("wtf");
    free(mlist);
    printf("wtf");

    mlist = malloc(sizeof(struct list));

    list_push_back(mlist, "hello9");
    list_push_front(mlist, "hello8");
    list_push_front(mlist, "hello7");
    list_push_front(mlist, "hello6");
    list_push_back(mlist, "hello10");
    list_push_front(mlist, "hello5");
    list_push_front(mlist, "hello4");
    list_push_front(mlist, "hello3");
    list_push_front(mlist, "hello2");
    list_push_back(mlist, "hello11");
    list_push_back(mlist, "hello12");
    list_push_front(mlist, "hello1");

    printf("wtf");

    for(int i = 0; i < mlist->size; i++)
    {
        printf("%s ", list_at(mlist, i));
    }
    printf("\n");

    printf("error here!?\n");

    list_push_back(mlist, list_remove_at(mlist, 12));
    list_push_back(mlist, list_remove_at(mlist, 12));
    printf("error here!?\n");
    list_push_back(mlist, list_remove_at(mlist, 12));
    list_push_back(mlist, list_remove_at(mlist, 12));

    list_push_front(mlist, list_remove_at(mlist, 0));
    list_push_front(mlist, list_remove_at(mlist, 0));
    //list_push_front(mlist, list_remove_at(mlist, 0));
    list_push_front(mlist, list_remove_at(mlist, 0));

    list_push_front(mlist, list_pop_front(mlist));
    //list_push_back(mlist, list_pop_back(mlist));
    printf("error here!?\n");

    int lsize = mlist->size - 1;
    for(int i = 0; i < lsize; i++)
    {
        //printf("error loop f here!?\n");
        //printf("%s ", list_at(mlist, i));
        printf("%s ", list_pop_back(mlist));
        //printf("error loop bhere!?\n");
    }
    printf("\n");

    //list_delete(mlist);
    printf("size 0 : %i\n", mlist->size);
   


    return 0;
}


