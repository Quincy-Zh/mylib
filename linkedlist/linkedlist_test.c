#include "linkedlist.h"

#include <stdio.h>

int _sum(void *data, void *ctx)
{
    int v = (int)data;
    int *p = ctx;

    printf(":: %d.\n", v);
    *p += v;

    return 0;
}

int main()
{
    sLinkedList *lst = linkedlist_create(0);
    int i = 0;
    int val = 8;

    printf("pList: %p.\n", lst);

    i = linkedlist_size(lst);
    printf("Size: %d.\n", i);

    for(i = 1; i < 10; ++i)
        linkedlist_append(lst, (void *)i);

    i = linkedlist_size(lst);
    printf("After appends, size: %d.\n", i);

    i = 0;
    linkedlist_foreach(lst, _sum, &i);
    printf("Sum: %d.\n", i);

    linkedlist_destroy(&lst);
    printf("destroyList: %p.\n", lst);

    return 0;
}

