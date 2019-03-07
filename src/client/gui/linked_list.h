#pragma once
#include <stdint.h>

typedef struct ListItem {
  struct ListItem* prev;
  struct ListItem* next;
  uint8_t angle;
  uint8_t distance;
} ListItem;

typedef struct ListHead {
  ListItem* first;
  ListItem* last;
  int size;
  int max_length;
} ListHead;

void List_init(ListHead* head, int max_length);

//set the max size of the list
void List_set_max_length(ListHead* head, int max_length);

//add item in the head of the list, if all ok return item else NULL
ListItem* List_insert(ListHead* head, ListItem* item);

//deletes all nodes of the list
int List_reset(ListHead* head);

//delete all list
ListItem* List_delete(ListHead* head);

void List_print(ListHead* head);

void List_print_reverse(ListHead* head);