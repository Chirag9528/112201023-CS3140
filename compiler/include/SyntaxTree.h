#ifndef SYNTAXTREE_H
#define SYNTAXTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Queue.h"

typedef struct __tree{
    char* name;
    int value;
    struct __tree* left;
    struct __tree* right;
} tree;


void preorder(tree* root);

void inorder(tree* root);

int depth(tree* root);

tree* createnode(char* s , int value , tree* left , tree* right);

void print_tree(tree* root);

void print_info(tree* root);

void print_decl(tree* root);

void print_decl_info(tree* root);

#endif