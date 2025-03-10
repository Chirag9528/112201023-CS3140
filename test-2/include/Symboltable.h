#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef enum{
	INTEGER,
	BOOLEAN,
	STRING,
    FLOAT
} variable_type;

typedef struct _value_struct{
    int size;
    void* value;
} value_struct;

typedef struct _node{
    char* name;
    char* type;
    int flag;
    variable_type vartype;
    value_struct* val;
    struct _node* next;
} node;

extern int prime1;
extern int prime2 ;
extern node** hash_table;

#define mod 10000007

void init();

int rolling_hashfunction(char* s);

int insert_string(char* s , variable_type vartype , char* type , int size);

int update_string(char* s , void* value , int idx);

node* search_string(char* s);

void update_type_size(char* s , char* type , int size);


void print_hash_table();

#endif