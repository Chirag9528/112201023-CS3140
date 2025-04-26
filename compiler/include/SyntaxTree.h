#ifndef SYNTAXTREE_H
#define SYNTAXTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Queue.h"
#include "Symboltable.h"

typedef union __valunion{
    int num;
    bool bnum;
    char* str;
}valunion;

typedef struct __tree{
    char* name;
    int idx;
    char* var_idx;
    char* type;
    variable_type vartype;
    valunion value;
    int valid_flag;
    struct __tree* child;
    struct __tree* sibling;
    int indentation;
} tree;

tree* createnode(char* s , int flag , int idx , valunion* value , variable_type vartype, char* type , tree* child , tree* sibling);

void evaluate_global_declaration(tree* root);

void evaluate_global_declaration_list(tree* root);

void evaluate_expression(tree* expr);

void evaluate_statements(tree* root);

void evaluate_tree(tree* root);

void print(tree* root , int tabspace);



#endif