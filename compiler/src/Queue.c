#include "Queue.h"

queue* init_queue(){
    queue* qu = (queue*)malloc(sizeof(queue));
    if (!qu) return NULL;
    qu->head = (queuenode*)malloc(sizeof(queuenode));
    if (!qu->head) return NULL;
    qu->tail = (queuenode*)malloc(sizeof(queuenode));
    if (!qu->tail) return NULL;
    qu->head->next = qu->tail;
    qu->tail->prev = qu->head;
    qu->size = 0;
    return qu;
}

void push(queue* qu , tree* node , int number){
    queuenode* newnode = (queuenode*)malloc(sizeof(queuenode));
    if (newnode == NULL) return;
    newnode->treenode = node;
    newnode->number = number;
    newnode->next = qu->tail;
    newnode->prev = qu->tail->prev;
    qu->tail->prev->next = newnode;
    qu->tail->prev = newnode;
    qu->size++;
    return;
}

queuenode* pop(queue* qu){
    if (qu->head->next == qu->tail) return NULL;
    queuenode* popped = qu->head->next;
    qu->head->next = qu->head->next->next;
    qu->head->next->prev = qu->head;
    qu->size--;
    return popped;
}

void print_queue(queue* qu){
    queuenode* temp = qu->head->next;
    while (temp!=qu->tail){
        printf("%s  ",temp->treenode->name);
        temp = temp->next;
    }
    printf("\n\n");
}