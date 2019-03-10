#include "../gui/linked_list.h"
#include <stdlib.h>
#include <stdio.h>

int main(){

    ListHead* head;
    head = malloc(sizeof(ListHead));
    List_init(head, 4);

    List_insert(head, 1, 1);
    
    List_print(head);
    List_print_reverse(head);
    printf("-----\n");


    List_insert(head, 2,2);

    List_print(head);
    List_print_reverse(head);
    printf("-----\n");


    List_insert(head, 3,3);

    List_print(head);
    List_print_reverse(head);
    printf("-----\n");

    List_insert(head, 4,4);

    List_print(head);
    List_print_reverse(head);
    printf("-----\n");


    List_insert(head, 5,5);

    List_print(head);
    List_print_reverse(head);
    printf("-----\n");


    List_insert(head, 6,6);

    List_print(head);
    List_print_reverse(head);
    printf("-----\n");

    List_reset(head);
    free(head);
}