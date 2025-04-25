#ifndef SYNTAXTREE_H
#define SYNTAXTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Queue.h"
#include "Symboltable.h"
#include "Stack.h"

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

void mips_code_globl_decl(FILE* fp , tree* root);

void mips_code_call_func(FILE* fp , tree* func_name);

void mips_code_stmt_list(FILE* fp , tree* root);

void generate_mips_code(FILE* fp , tree* root);

#endif