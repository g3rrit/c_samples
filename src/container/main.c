#include "stdio.h"

#define CONTAINER_C
#include "container.c"
#undef CONTAINER_C

int main()
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

    list_delete(mlist);

    free(mlist);


    return 0;
}


