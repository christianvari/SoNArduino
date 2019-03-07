#include "../gui/linked_list.h"
#include <stdlib.h>
#include <stdio.h>

int main(){

    ListHead* head;
    head = malloc(sizeof(ListHead));
    List_init(head, 4);

    ListItem* i=malloc(sizeof(ListItem));
    i->next=NULL;
    i->prev=NULL;
    i->angle=1;
    i->distance=1;
    List_insert(head, i);
    
    List_print(head);
    List_print_reverse(head);
    printf("-----\n");

    i=malloc(sizeof(ListItem));
    i->next=NULL;
    i->prev=NULL;
    i->angle=2;
    i->distance=2;
    List_insert(head, i);

    List_print(head);
    List_print_reverse(head);
    printf("-----\n");

    i=malloc(sizeof(ListItem));
    i->next=NULL;
    i->prev=NULL;
    i->angle=3;
    i->distance=3;
    List_insert(head, i);

    List_print(head);
    List_print_reverse(head);
    printf("-----\n");
    
    i=malloc(sizeof(ListItem));
    i->next=NULL;
    i->prev=NULL;
    i->angle=4;
    i->distance=4;
    List_insert(head, i);

    List_print(head);
    List_print_reverse(head);
    printf("-----\n");

    i=malloc(sizeof(ListItem));
    i->next=NULL;
    i->prev=NULL;
    i->angle=5;
    i->distance=5;
    List_insert(head, i);

    List_print(head);
    List_print_reverse(head);
    printf("-----\n");

    i=malloc(sizeof(ListItem));
    i->next=NULL;
    i->prev=NULL;
    i->angle=6;
    i->distance=6;
    List_insert(head, i);

    List_print(head);
    List_print_reverse(head);
    printf("-----\n");

    List_reset(head);
    free(head);
}