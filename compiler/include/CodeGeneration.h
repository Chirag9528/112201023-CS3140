#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Queue.h"
#include "Symboltable.h"
#include "Stack.h"
#include "SyntaxTree.h"


void mips_code_globl_decl(FILE* fp , tree* root);

void mips_code_expression(FILE* fp , tree* root);

void mips_code_assign_stmt(FILE* fp , tree* root);

int get_new_label_id();

void mips_code_for_loop(FILE* fp , tree* root);

void mips_code_if_statement(FILE* fp , tree* root);

void mips_code_stmt_list(FILE* fp , tree* root);

void mips_code_call_func(FILE* fp , tree* func_name);

void generate_mips_code(FILE* fp , tree* root);