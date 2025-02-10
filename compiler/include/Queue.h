#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct __tree tree;  // forward declaration

#include "SyntaxTree.h"

typedef struct _queuenode{
    tree* treenode;
    struct _queuenode* next;
    struct _queuenode* prev;
    int number; // will use for printing
}queuenode;

typedef struct __queue{
    queuenode* head;
    queuenode* tail;
    int size;
}queue;

queue* init_queue();

void push(queue* qu , tree* node , int number);

queuenode* pop(queue* qu);

void print_queue(queue* qu);

#endif