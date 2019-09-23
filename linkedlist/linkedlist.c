#include "linkedlist.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct LinkedList
{
    void *data;
    struct LinkedList *next;
};

typedef struct LinkedList sLinkedListNode;

sLinkedList *linkedlist_create(int option)
{
    sLinkedList * lst = malloc(sizeof(sLinkedList));

    if(lst)
    {
        memset(lst, 0, sizeof(sLinkedList));
    }

    return lst;
}

void linkedlist_destroy(sLinkedList **plist)
{
    sLinkedListNode *next;
    sLinkedListNode *node = *plist;

    while(node)
    {
        next = node->next;

        free(node);
        node = next;
    }

    *plist = 0;
}

int linkedlist_size(sLinkedList *lst)
{
    int cnt = 0;
    sLinkedListNode *node = lst;

    while(node->next)
    {
        node = node->next;
        cnt++;
    }

    return cnt;
}

int linkedlist_append(sLinkedList *lst, void *data)
{
    sLinkedListNode *node = lst;

    while(node->next)
    {
        node = node->next;
    }

    node->next = malloc(sizeof(sLinkedListNode));
    if(node->next == NULL)
    {
        return -1;
    }

    node = node->next;

    node->data = data;
    node->next = 0;

    return 0;
}

int linkedlist_remove(sLinkedList *lst, int index)
{
    return -1;
}

int linkedlist_foreach(sLinkedList *lst, fnForeach fn, void *ctx)
{
    int ret_val = 0;

    sLinkedListNode *node = lst->next;

    while(node)
    {
        ret_val = fn(node->data, ctx);

        if(ret_val)
        {
            return ret_val;
        }

        node = node->next;
    }

    return ret_val;
}

// EOF
