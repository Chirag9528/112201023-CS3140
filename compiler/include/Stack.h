#include <stdio.h>
#include <stdlib.h>

typedef struct _stacknode{
    struct _stacknode* next;
    struct _stacknode* prev;
    int number;
}stacknode;

typedef struct __stack{
    stacknode* head;
    stacknode* tail;
    int size;
}stack;

stack* init_stack();

void pushOnStack(stack* st , int number);

stacknode* popFromStack(stack* st);

void print_stack(stack* st);
