#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _node{
    char* name;
    int value;
    int flag;
    struct _node* next;
} node;

extern int prime1;
extern int prime2 ;
extern node** hash_table;

#define mod 10000007

void init();

int rolling_hashfunction(char* s);

int insert_string(char* s , int value , int flag);

int update_string(char* s , int value);

int search_string(char* s);

void print_hash_table();

#endif