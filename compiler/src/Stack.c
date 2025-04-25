#include "Stack.h"

stack* init_stack(){
    stack* st= (stack*)malloc(sizeof(stack));
    if (!st) return NULL;
    st->head = (stacknode*)malloc(sizeof(stacknode));
    if (!st->head) return NULL;
    st->tail = (stacknode*)malloc(sizeof(stacknode));
    if (!st->tail) return NULL;
    st->head->next = st->tail;
    st->tail->prev = st->head;
    st->size = 0;
    return st;
}

void pushOnStack(stack* st , int number){
    stacknode* newnode = (stacknode*)malloc(sizeof(stacknode));
    if (newnode == NULL) return;
    newnode->number = number;
    newnode->next = st->head->next;
    newnode->prev = st->head;
    st->head->next->prev = newnode;
    st->head->next = newnode;
    st->size++;
    return;
}

stacknode* popFromStack(stack* st){
    if (st->head->next == st->tail) return NULL;
    stacknode* popped = st->head->next;
    st->head->next = st->head->next->next;
    st->head->next->prev = st->head;
    st->size--;
    return popped;
}

void print_stack(stack* st){
    stacknode* temp = st->head->next;
    while (temp!=st->tail){
        printf("%d  ",temp->number);
        temp = temp->next;
    }
    printf("\n\n");
}