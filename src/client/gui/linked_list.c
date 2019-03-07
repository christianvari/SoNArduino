#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>

void List_init(ListHead* head, int max_length) {
  head->first=0;
  head->last=0;
  head->size=0;
  head->max_length=max_length;
}

void List_set_max_length(ListHead* head, int max_length){
  head->max_length=max_length;
}


ListItem* List_insert(ListHead* head, ListItem* item) {
  if(head->first==0){
    if(head->max_length==0) return NULL;
    printf("caso1\n");
    head->first=item;
    head->last=item;
    head->size=1;
    return item;
  }

  if(head->size+1 <= head->max_length){
    printf("caso2\n");

    item->next=head->first;
    head->first->prev=item;
    head->first=item;
    head->size++;
    return item;
  }
  
  printf("caso3\n");
  ListItem* t=head->last;
  head->last=head->last->prev;
  t->prev->next=NULL;

  free(t);
  item->next=head->first;
  head->first->prev=item;
  head->first=item;

  return item;
}

int List_reset(ListHead* head){

  int n=0;
  ListItem* t;
  while(head->first){
    t=head->first;
    head->first=head->first->next;
    head->size--;
    n++;
    free(t);
  }
  return n;
}

void List_print(ListHead* head){

  ListItem* aux=head->first;
  while(aux){
    printf("angle: %d, distance: %d\n", aux->angle, aux->distance);
    aux=aux->next;
  }

}

void List_print_reverse(ListHead* head){

  ListItem* aux=head->last;
  while(aux){
    printf("angle: %d, distance: %d\n", aux->angle, aux->distance);
    aux=aux->prev;
  }

}