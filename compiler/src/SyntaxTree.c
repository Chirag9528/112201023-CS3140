#include "./../include/SyntaxTree.h"
int breakflag = 0;

tree* createnode(char* s , int flag , int idx , valunion* value , variable_type vartype , char* type , tree* child , tree* sibling){
    tree* newnode = (tree*)malloc(sizeof(tree));
    if (newnode == NULL) return newnode;
    if (s){
        newnode->name = strdup(s);
    }
    if (child){
        newnode->child = (tree*)malloc(sizeof(tree));
        if (newnode->child == NULL) return NULL;
        newnode->child = child;
    }
    if (sibling){
        newnode->sibling = (tree*)malloc(sizeof(tree));
        if (newnode->sibling == NULL) return NULL;
        newnode->sibling = sibling;
    }
    newnode->valid_flag = flag;
    newnode->vartype = vartype;
    newnode->idx = idx;
    if (type){
        newnode->type = strdup(type);
    }
    if (value){
        if (vartype == INTEGER){
            newnode->value.num = value->num;
        }
        else if (vartype == BOOLEAN){
            newnode->value.bnum = value->bnum;
        }
        else if (vartype == STRING){
            newnode->value.str = strdup(value->str);
        }
    }
    return newnode;
}

void evaluate_global_declaration(tree* root){
    if (!root) return;
    tree* decl_type = root->child;
    tree* vars = decl_type->sibling;
    switch (decl_type->vartype){
        case INTEGER:
            while (vars){
                if (strcmp(vars->type , "array") == 0){
                    insert_string(vars->name , INTEGER , "array" , vars->idx);
                }
                else{
                    insert_string(vars->name , INTEGER , "int" , 1);
                }
                vars = vars->sibling;
            }
            break;
        case BOOLEAN:
            while (vars){
                if (strcmp(vars->type , "array") == 0){
                    insert_string(vars->name , BOOLEAN , "array" , vars->idx);
                }
                else{
                    insert_string(vars->name , BOOLEAN , "bool" , 1);
                }
                vars = vars->sibling;
            }
            break;
        case STRING:
            while (vars){
                if (strcmp(vars->type , "array") == 0){
                    insert_string(vars->name , STRING , "array" , vars->idx);
                }
                else{
                    insert_string(vars->name , STRING , "char*" , 1);
                }
                vars = vars->sibling;
            }
            break;
        default:
            break;
    }
    
}

void evaluate_global_declaration_list(tree* root){
    if (!root) return;
    tree* child = root->child;
    while (child){
        evaluate_global_declaration(child);
        child = child->sibling;
    }
}

void evaluate_expression(tree* expr){
    if (expr->name){
        if (strcmp(expr->name , "LOGICAL_OR") == 0){
            evaluate_expression(expr->child);
            evaluate_expression(expr->child->sibling);
            if (expr->child->vartype == INTEGER && expr->child->sibling->vartype == INTEGER){
                expr->value.bnum = expr->child->value.num || expr->child->sibling->value.num;
            }
            else if (expr->child->vartype == INTEGER && expr->child->sibling->vartype == BOOLEAN){
                expr->value.bnum = expr->child->value.num || expr->child->sibling->value.bnum;
            }
            else if (expr->child->vartype == BOOLEAN && expr->child->sibling->vartype == INTEGER){
                expr->value.bnum = expr->child->value.bnum || expr->child->sibling->value.num;
            }
            else{
                expr->value.bnum = expr->child->value.bnum || expr->child->sibling->value.bnum;
            }
        }
        else if (strcmp(expr->name , "LOGICAL_AND") == 0){
            evaluate_expression(expr->child);
            evaluate_expression(expr->child->sibling);
            if (expr->child->vartype == INTEGER && expr->child->sibling->vartype == INTEGER){
                expr->value.bnum = expr->child->value.num && expr->child->sibling->value.num;
            }
            else if (expr->child->vartype == INTEGER && expr->child->sibling->vartype == BOOLEAN){
                expr->value.bnum = expr->child->value.num && expr->child->sibling->value.bnum;
            }
            else if (expr->child->vartype == BOOLEAN && expr->child->sibling->vartype == INTEGER){
                expr->value.bnum = expr->child->value.bnum && expr->child->sibling->value.num;
            }
            else{
                expr->value.bnum = expr->child->value.bnum && expr->child->sibling->value.bnum;
            }
        }
        else if (strcmp(expr->name , "LOGICAL_NOT") == 0){
            evaluate_expression(expr->child);   
            if (expr->vartype == INTEGER){
                expr->value.bnum = !expr->child->value.num;
            }
            else{
                expr->value.bnum = !expr->child->value.bnum;
            }
        }
        else if (strcmp(expr->name , "EQUALEQUAL") == 0){
            evaluate_expression(expr->child);
            evaluate_expression(expr->child->sibling);
            if (expr->child->vartype == INTEGER && expr->child->sibling->vartype == INTEGER){
                expr->value.bnum = expr->child->value.num == expr->child->sibling->value.num;
            }
            else if (expr->child->vartype == INTEGER && expr->child->sibling->vartype == BOOLEAN){
                expr->value.bnum = expr->child->value.num == expr->child->sibling->value.bnum;
            }
            else if (expr->child->vartype == BOOLEAN && expr->child->sibling->vartype == INTEGER){
                expr->value.bnum = expr->child->value.bnum == expr->child->sibling->value.num;
            }
            else{
                expr->value.bnum = expr->child->value.bnum == expr->child->sibling->value.bnum;
            }
        }
        else if (strcmp(expr->name , "NOTEQUAL") == 0){
            evaluate_expression(expr->child);
            evaluate_expression(expr->child->sibling);
            if (expr->child->vartype == INTEGER && expr->child->sibling->vartype == INTEGER){
                expr->value.bnum = expr->child->value.num != expr->child->sibling->value.num;
            }
            else if (expr->child->vartype == INTEGER && expr->child->sibling->vartype == BOOLEAN){
                expr->value.bnum = expr->child->value.num != expr->child->sibling->value.bnum;
            }
            else if (expr->child->vartype == BOOLEAN && expr->child->sibling->vartype == INTEGER){
                expr->value.bnum = expr->child->value.bnum != expr->child->sibling->value.num;
            }
            else{
                expr->value.bnum = expr->child->value.bnum != expr->child->sibling->value.bnum;
            }
        }
        else if (strcmp(expr->name , "LESSTHANOREQUAL") == 0){
            evaluate_expression(expr->child);
            evaluate_expression(expr->child->sibling);
            if (expr->child->vartype == INTEGER && expr->child->sibling->vartype == INTEGER){
                expr->value.bnum = expr->child->value.num <= expr->child->sibling->value.num;
            }
            else if (expr->child->vartype == INTEGER && expr->child->sibling->vartype == BOOLEAN){
                expr->value.bnum = expr->child->value.num <= expr->child->sibling->value.bnum;
            }
            else if (expr->child->vartype == BOOLEAN && expr->child->sibling->vartype == INTEGER){
                expr->value.bnum = expr->child->value.bnum <= expr->child->sibling->value.num;
            }
            else{
                expr->value.bnum = expr->child->value.bnum <= expr->child->sibling->value.bnum;
            }
        }
        else if (strcmp(expr->name , "GREATERTHANOREQUAL") == 0){
            evaluate_expression(expr->child);
            evaluate_expression(expr->child->sibling);
            if (expr->child->vartype == INTEGER && expr->child->sibling->vartype == INTEGER){
                expr->value.bnum = expr->child->value.num >= expr->child->sibling->value.num;
            }
            else if (expr->child->vartype == INTEGER && expr->child->sibling->vartype == BOOLEAN){
                expr->value.bnum = expr->child->value.num >= expr->child->sibling->value.bnum;
            }
            else if (expr->child->vartype == BOOLEAN && expr->child->sibling->vartype == INTEGER){
                expr->value.bnum = expr->child->value.bnum >= expr->child->sibling->value.num;
            }
            else{
                expr->value.bnum = expr->child->value.bnum >= expr->child->sibling->value.bnum;
            }
        }
        else if (strcmp(expr->name , "GREATERTHAN") == 0){
            evaluate_expression(expr->child);
            evaluate_expression(expr->child->sibling);
            if (expr->child->vartype == INTEGER && expr->child->sibling->vartype == INTEGER){
                expr->value.bnum = expr->child->value.num > expr->child->sibling->value.num;
            }
            else if (expr->child->vartype == INTEGER && expr->child->sibling->vartype == BOOLEAN){
                expr->value.bnum = expr->child->value.num > expr->child->sibling->value.bnum;
            }
            else if (expr->child->vartype == BOOLEAN && expr->child->sibling->vartype == INTEGER){
                expr->value.bnum = expr->child->value.bnum > expr->child->sibling->value.num;
            }
            else{
                expr->value.bnum = expr->child->value.bnum > expr->child->sibling->value.bnum;
            }
        }
        else if (strcmp(expr->name , "LESSTHAN") == 0){
            evaluate_expression(expr->child);
            evaluate_expression(expr->child->sibling); 
            if (expr->child->vartype == INTEGER && expr->child->sibling->vartype == INTEGER){
                expr->value.bnum = expr->child->value.num < expr->child->sibling->value.num;
            }
            else if (expr->child->vartype == INTEGER && expr->child->sibling->vartype == BOOLEAN){
                expr->value.bnum = expr->child->value.num < expr->child->sibling->value.bnum;
            }
            else if (expr->child->vartype == BOOLEAN && expr->child->sibling->vartype == INTEGER){
                expr->value.bnum = expr->child->value.bnum < expr->child->sibling->value.num;
            }
            else{
                expr->value.bnum = expr->child->value.bnum < expr->child->sibling->value.bnum;
            }
        }
        else if (strcmp(expr->name , "MODULO") == 0){
            evaluate_expression(expr->child);
            evaluate_expression(expr->child->sibling);
            if (expr->child->vartype == INTEGER && expr->child->sibling->vartype == INTEGER){
                expr->value.num = expr->child->value.num % expr->child->sibling->value.num;
            }
            else if (expr->child->vartype == INTEGER && expr->child->sibling->vartype == BOOLEAN){
                expr->value.num = expr->child->value.num % expr->child->sibling->value.bnum;
            }
            else if (expr->child->vartype == BOOLEAN && expr->child->sibling->vartype == INTEGER){
                expr->value.num = expr->child->value.bnum % expr->child->sibling->value.num;
            }
            else{
                expr->value.num = expr->child->value.bnum % expr->child->sibling->value.bnum;
            }
        }
        else if (strcmp(expr->name , "DIVIDE") == 0){
            evaluate_expression(expr->child);
            evaluate_expression(expr->child->sibling); 
            if (expr->child->vartype == INTEGER && expr->child->sibling->vartype == INTEGER){
                expr->value.num = expr->child->value.num / expr->child->sibling->value.num;
            }
            else if (expr->child->vartype == INTEGER && expr->child->sibling->vartype == BOOLEAN){
                expr->value.num = expr->child->value.num / expr->child->sibling->value.bnum;
            }
            else if (expr->child->vartype == BOOLEAN && expr->child->sibling->vartype == INTEGER){
                expr->value.num = expr->child->value.bnum / expr->child->sibling->value.num;
            }
            else{
                expr->value.num = expr->child->value.bnum / expr->child->sibling->value.bnum;
            }
        }
        else if (strcmp(expr->name , "MULT") == 0){
            evaluate_expression(expr->child);
            evaluate_expression(expr->child->sibling);
            if (expr->child->vartype == INTEGER && expr->child->sibling->vartype == INTEGER){
                expr->value.num = expr->child->value.num * expr->child->sibling->value.num;
            }
            else if (expr->child->vartype == INTEGER && expr->child->sibling->vartype == BOOLEAN){
                expr->value.num = expr->child->value.num * expr->child->sibling->value.bnum;
            }
            else if (expr->child->vartype == BOOLEAN && expr->child->sibling->vartype == INTEGER){
                expr->value.num = expr->child->value.bnum * expr->child->sibling->value.num;
            }
            else{
                expr->value.num = expr->child->value.bnum * expr->child->sibling->value.bnum;
            }
        }
        else if (strcmp(expr->name , "MINUS") == 0){
            evaluate_expression(expr->child);
            evaluate_expression(expr->child->sibling);
            if (expr->child->vartype == INTEGER && expr->child->sibling->vartype == INTEGER){
                expr->value.num = expr->child->value.num - expr->child->sibling->value.num;
            }
            else if (expr->child->vartype == INTEGER && expr->child->sibling->vartype == BOOLEAN){
                expr->value.num = expr->child->value.num - expr->child->sibling->value.bnum;
            }
            else if (expr->child->vartype == BOOLEAN && expr->child->sibling->vartype == INTEGER){
                expr->value.num = expr->child->value.bnum - expr->child->sibling->value.num;
            }
            else{
                expr->value.num = expr->child->value.bnum - expr->child->sibling->value.bnum;
            }
        }
        else if (strcmp(expr->name , "PLUS") == 0){
            evaluate_expression(expr->child);
            evaluate_expression(expr->child->sibling);
            if (expr->child->vartype == INTEGER && expr->child->sibling->vartype == INTEGER){
                expr->value.num = expr->child->value.num + expr->child->sibling->value.num;
            }
            else if (expr->child->vartype == INTEGER && expr->child->sibling->vartype == BOOLEAN){
                expr->value.num = expr->child->value.num + expr->child->sibling->value.bnum;
            }
            else if (expr->child->vartype == BOOLEAN && expr->child->sibling->vartype == INTEGER){
                expr->value.num = expr->child->value.bnum + expr->child->sibling->value.num;
            }
            else{
                expr->value.num = expr->child->value.bnum + expr->child->sibling->value.bnum;
            }
        }
        else {
            node* varnode = search_string(expr->name);
            expr->type = strdup(varnode->type);
            expr->vartype = varnode->vartype;

            if (strcmp(expr->type , "array") == 0){
                if (expr->var_idx){
                    node* var_value = search_string(expr->var_idx);
                    expr->idx = *((int*)var_value->val->value);
                }
                if (varnode->vartype == INTEGER){
                    int numvalue = ((int*)varnode->val->value)[expr->idx];
                    expr->value.num = numvalue;
                }
                else if (varnode->vartype == BOOLEAN){
                    bool boolvalue = ((bool*)varnode->val->value)[expr->idx];
                    expr->value.bnum = boolvalue;
                }
                else if (varnode->vartype == STRING){
                    char* strvalue = ((char**)varnode->val->value)[expr->idx];
                    expr->value.str = strdup(strvalue);
                }
            }
            else{
                if (varnode->vartype == INTEGER){
                    int numvalue = *((int*)varnode->val->value);
                    expr->value.num = numvalue;
                }
                else if (varnode->vartype == BOOLEAN){
                    bool boolvalue = *((bool*)varnode->val->value);
                    expr->value.bnum = boolvalue;
                }
                else if (varnode->vartype == STRING){
                    char* strvalue = *((char**)varnode->val->value);
                    expr->value.str = strdup(strvalue);
                }
            }
        }
    }
}

void evaluate_statements(tree* root){
    tree* childs = root->child;
    while (childs){
        if (strcmp(childs->name , "COND_STMT") == 0){
            if (breakflag) break;
            if (strcmp(childs->child->name , "IF_STMT") == 0){
                tree* if_node = childs->child;
                tree* if_cond = if_node->child;
                evaluate_expression(if_cond->child);
                if (if_cond->child->value.bnum){
                    tree* if_body = if_cond->sibling;
                    evaluate_statements(if_body);
                }
                else if (if_node->sibling){
                    tree* else_body = if_node->sibling->child;
                    evaluate_statements(else_body);
                }
            }
            else if (strcmp(childs->child->name , "FOR_STMT") == 0){
                tree* for_init = childs->child->child;
                tree* for_cond = for_init->sibling;
                tree* for_expr = for_cond->sibling;
                tree* for_body = for_expr->sibling;
                evaluate_statements(for_init);
                evaluate_expression(for_cond->child);
                while (for_cond->child->value.bnum){
                    evaluate_statements(for_body);
                    if (breakflag) break;
                    evaluate_statements(for_expr);
                    evaluate_expression(for_cond->child);
                }
                breakflag = 0;
            }
            else if (strcmp(childs->child->name , "WHILE_STMT") == 0){
                tree* while_cond = childs->child->child;
                tree* while_body = while_cond->sibling;
                evaluate_expression(while_cond->child);
                do {
                    evaluate_statements(while_body);
                    evaluate_expression(while_cond->child);
                } while (while_cond->child->value.bnum);
            }
        }
        else if (strcmp(childs->name , "ASSIGN_STMT") == 0){
            if (breakflag) break;
            valunion value;
            tree* dollarone = childs->child;
            tree* dollarthree = childs->child->sibling;
            if (dollarthree->name && strcmp(dollarthree->name , "PLUSPLUS") == 0){
                node* varnode = search_string(dollarone->name);
                int index = dollarone->idx;
                if (strcmp(varnode->type , "array") == 0){
                    if (dollarone->var_idx){
                        node* var_value = search_string(dollarone->var_idx);
                        index = *((int*)var_value->val->value);
                    }
                    if (varnode->vartype == INTEGER){
                        int numvalue = ((int*)varnode->val->value)[index];
                        dollarthree->value.num = numvalue + 1;
                    }
                }
                else{
                    if (varnode->vartype == INTEGER){
                        int numvalue = *((int*)varnode->val->value);
                        dollarthree->value.num = numvalue + 1;
                    }
                }
            }
            else evaluate_expression(dollarthree);

            node* varnode = search_string(dollarone->name);
            dollarone->type = strdup(varnode->type);
            dollarone->vartype = varnode->vartype;

            if (strcmp(dollarone->type , "array")==0){
                if (dollarone->var_idx){
                    node* var_value = search_string(dollarone->var_idx);
                    dollarone->idx = *((int*)var_value->val->value);
                }
                if (dollarone->vartype == INTEGER){
                    int val = dollarthree->value.num;
                    update_string(dollarone->name , (void*)&val , dollarone->idx);
                    value.num = val;
                }
                else if (dollarone->vartype == BOOLEAN){
                    bool val = dollarthree->value.bnum;
                    update_string(dollarone->name , (void*)&val , dollarone->idx);
                    value.bnum = val;
                }
                else if (dollarone->vartype == STRING){
                    char* val = dollarthree->value.str;
                    update_string(dollarone->name , (void*)&val , dollarone->idx);
                    value.str = strdup(val);
                }
            }
            else{
                if (dollarone->vartype == INTEGER){
                    int val = dollarthree->value.num;
                    update_string(dollarone->name , (void*)&val , 0);
                    value.num = val;
                }
                else if (dollarone->vartype == BOOLEAN){
                    bool val = dollarthree->value.bnum;
                    update_string(dollarone->name , (void*)&val , 0);
                    value.bnum = val;
                }
                else if (dollarone->vartype == STRING){
                    char* val = dollarthree->value.str;
                    update_string(dollarone->name , (void*)&val , 0);
                    value.str = strdup(val);
                }
            }
            childs->value = value;
        }
        else if (strcmp(childs->name , "break") == 0){
            breakflag = 1;
            break;
        }
        childs = childs->sibling;
    }
}

void evaluate_tree(tree* root){
    if (!root) return;
    if (strcmp(root->name , "PROG") == 0){
        tree* child = root->child;
        while (child){
            if (child && child->name){
                if (strcmp(child->name , "GDECL_LIST") == 0){
                    evaluate_global_declaration_list(child);
                }
                else if (strcmp(child->name , "STMT_LIST") == 0){
                    evaluate_statements(child);
                }
            }
            child = child->sibling;
        }
    }
    return;
}

void print(tree* root , int tabspace){
    if (root == NULL) return;
    int space = tabspace;
    while (space){
        printf("\t|");
        space--;
    }
    printf("--> ");
    if (root->name){
        if (root->valid_flag && root->type && strcmp(root->type,"array") == 0){
            if (root->var_idx){
                printf("%s[%s]\n",root->name , root->var_idx);
            }
            else{
                printf("%s[%d]\n",root->name , root->idx);
            }
        }
        else{
            printf("%s\n",root->name);
        }
    }
    else{
        if (root->vartype == INTEGER){
            printf("%d\n",root->value.num);
        }
        else if (root->vartype == BOOLEAN){
            printf("%d\n",root->value.bnum);
        }
        else if (root->vartype == STRING){
            printf("%s\n" , root->value.str);
        }
    }
    if (root->child){
        print(root->child , tabspace + 1);
        tree* childs = root->child->sibling;
        while (childs){
            print(childs , tabspace + 1);
            childs = childs->sibling;
        }
    }

}

void mips_code_globl_decl(FILE* fp , tree* root){
    int sectionflag = 0;
    tree* child = root->child;
    if (child) fprintf(fp , "\t.text\n");
    while (child){
        tree* type = child->child;
        if (type->name && strcmp(type->name , "integer") == 0){
            tree* vars = type->sibling;
            while (vars){
                if (strcmp(vars->type , "int") == 0){
                    fprintf(fp , "\t.globl\t%s\n",vars->name);
                    if (!sectionflag){
                        sectionflag = 1;
                        fprintf(fp , "\t.section\t.bss,\"aw\",@nobits\n");
                    }
                    fprintf(fp , "\t.align\t2\n");
                    fprintf(fp , "\t.type\t%s, @object\n",vars->name);
                    fprintf(fp , "\t.size\t%s, 4\n",vars->name);
                    fprintf(fp , "%s:\n",vars->name);
                    fprintf(fp , "\t.space\t4\n");
                }
                else if (strcmp(vars->type , "array") == 0){
                    fprintf(fp , "\t.globl\t%s\n",vars->name);
                    if (!sectionflag){
                        sectionflag = 1;
                        fprintf(fp , "\t.section\t.bss,\"aw\",@nobits\n");
                    }
                    fprintf(fp , "\t.align\t2\n");
                    fprintf(fp , "\t.type\t%s, @object\n",vars->name);
                    fprintf(fp , "\t.size\t%s, %d\n",vars->name , vars->idx*4);
                    fprintf(fp , "%s:\n",vars->name);
                    fprintf(fp , "\t.space\t%d\n",vars->idx*4);
                }
                vars = vars->sibling;
            }
        }
        child = child->sibling;
    }
}

void mips_code_call_func(FILE* fp , tree* func_name){
    if (strcmp(func_name->name , "WRITE") == 0){
        tree* write_vars = func_name->sibling;
        while (write_vars){
            node* variable = search_string(write_vars->name);
            if (strcmp(variable->type , "array") == 0){
                switch (variable->vartype){
                case INTEGER:
                    fprintf(fp , "\tlui\t$2,%%hi(%s)\n",write_vars->name);
                    fprintf(fp , "\taddiu\t$2,$2,%%lo(%s)\n",write_vars->name);
                    if (write_vars->var_idx){
                        fprintf(fp , "\tlui\t$3,%%hi(%s)\n", write_vars->var_idx);
                        fprintf(fp , "\tlw\t$3,%%lo(%s)($3)\n", write_vars->var_idx);
                        fprintf(fp , "\tsll\t$3,$3,2\n"); 
                        fprintf(fp , "\tadd\t$4,$2,$3\n");
                        fprintf(fp , "\tlw\t$2,0($4)\n");
                    }
                    else{
                        fprintf(fp , "\tlw\t$2,%d($2)\n",4*write_vars->idx);
                    }
                    fprintf(fp , "\tmove\t$5,$2\n");
                    if (write_vars->sibling){
                        fprintf(fp , "\tlui\t$2,%%hi($LC1)\n");
                        fprintf(fp , "\taddiu\t$4,$2,%%lo($LC1)\n");
                    }
                    else{
                        fprintf(fp , "\tlui\t$2,%%hi($LC2)\n");
                        fprintf(fp , "\taddiu\t$4,$2,%%lo($LC2)\n");
                    }
                    fprintf(fp , "\tlw\t$2,%%call16(printf)($28)\n");
                    fprintf(fp , "\tmove\t$25,$2\n");
                    fprintf(fp , "\t.reloc\t1f,R_MIPS_JALR,printf\n");
                    fprintf(fp , "1:\tjalr\t$25\n");
                    fprintf(fp , "\tnop\n");
                    fprintf(fp , "\n");
                    fprintf(fp , "\tlw\t$28,16($fp)\t\t\t# $28 is global pointer\n");
                    break;
                default:
                    break;
                }
            }
            else{
                switch (variable->vartype){
                case INTEGER:
                    fprintf(fp , "\tlui\t$2,%%hi(%s)\n",write_vars->name);
                    fprintf(fp , "\tlw\t$2,%%lo(%s)($2)\n",write_vars->name);
                    fprintf(fp , "\tmove\t$5,$2\n");
                    if (write_vars->sibling){
                        fprintf(fp , "\tlui\t$2,%%hi($LC1)\n");
                        fprintf(fp , "\taddiu\t$4,$2,%%lo($LC1)\n");
                    }
                    else{
                        fprintf(fp , "\tlui\t$2,%%hi($LC2)\n");
                        fprintf(fp , "\taddiu\t$4,$2,%%lo($LC2)\n");
                    }
                    fprintf(fp , "\tlw\t$2,%%call16(printf)($28)\n");
                    fprintf(fp , "\tmove\t$25,$2\n");
                    fprintf(fp , "\t.reloc\t1f,R_MIPS_JALR,printf\n");
                    fprintf(fp , "1:\tjalr\t$25\n");
                    fprintf(fp , "\tnop\n");
                    fprintf(fp , "\n");
                    fprintf(fp , "\tlw\t$28,16($fp)\t\t\t# $28 is global pointer\n");
                    break;
                
                default:
                    break;
                }
            }
            write_vars = write_vars->sibling;
        }
    }
    else if (strcmp(func_name->name , "READ") == 0){
        tree* read_vars = func_name->sibling;
        while (read_vars){
            node* variable = search_string(read_vars->name);
            if (strcmp(variable->type , "array") == 0){
                switch (variable->vartype){
                case INTEGER:
                    if (read_vars->var_idx){
                        fprintf(fp , "\tlui\t$2,%%hi(%s)\n", read_vars->name);
                        fprintf(fp , "\taddiu\t$2,$2,%%lo(%s)\n", read_vars->name);
                        fprintf(fp , "\tlui\t$4,%%hi(%s)\n", read_vars->var_idx);
                        fprintf(fp , "\tlw\t$4,%%lo(%s)($4)\n", read_vars->var_idx);
                        fprintf(fp , "\tsll\t$4,$4,2\n");   
                        fprintf(fp , "\tadd\t$5,$2,$4\n");
                    }
                    else{
                        fprintf(fp , "\tlui\t$2,%%hi(%s+%d)\n",read_vars->name,4*read_vars->idx);
                        fprintf(fp , "\taddiu\t$5,$2,%%lo(%s+%d)\n",read_vars->name,4*read_vars->idx);
                    }
                    fprintf(fp , "\tlui\t$2,%%hi($LC0)\n");
                    fprintf(fp , "\taddiu\t$4,$2,%%lo($LC0)\n");
                    fprintf(fp , "\tlw\t$2,%%call16(__isoc99_scanf)($28)\n");
                    fprintf(fp , "\tmove\t$25,$2\n");
                    fprintf(fp , "\t.reloc\t1f,R_MIPS_JALR,__isoc99_scanf\n");
                    fprintf(fp , "1:\tjalr\t$25\n");
                    fprintf(fp , "\tnop\n");
                    fprintf(fp , "\n");
                    fprintf(fp , "\tlw\t$28,16($fp)\t\t\t# $28 is global pointer\n");
                    break;
                default:
                    break;
                }
            }
            else{
                switch (variable->vartype){
                case INTEGER:
                    fprintf(fp , "\tlui\t$2,%%hi(%s)\n",read_vars->name);
                    fprintf(fp , "\taddiu\t$5,$2,%%lo(%s)\n",read_vars->name);
                    fprintf(fp , "\tlui\t$2,%%hi($LC0)\n");
                    fprintf(fp , "\taddiu\t$4,$2,%%lo($LC0)\n");
                    fprintf(fp , "\tlw\t$2,%%call16(__isoc99_scanf)($28)\n");
                    fprintf(fp , "\tmove\t$25,$2\n");
                    fprintf(fp , "\t.reloc\t1f,R_MIPS_JALR,__isoc99_scanf\n");
                    fprintf(fp , "1:\tjalr\t$25\n");
                    fprintf(fp , "\tnop\n");
                    fprintf(fp , "\n");
                    fprintf(fp , "\tlw\t$28,16($fp)\t\t\t# $28 is global pointer\n");
                    break;
                
                default:
                    break;
                }
            }
            read_vars = read_vars->sibling;
        }
    }
}

void mips_code_expression(FILE* fp , tree* root){
    if (root->name){
        if (strcmp(root->name , "PLUS") == 0){
            mips_code_expression(fp , root->child);
            mips_code_expression(fp , root->child->sibling);
            switch (root->child->vartype){
                case INTEGER:
                    switch (root->child->sibling->vartype){
                        case INTEGER:
                            fprintf(fp , "\tlw\t$t1,0($sp)\n");
                            fprintf(fp , "\taddiu\t$sp,$sp,4\n");
                            fprintf(fp , "\tlw\t$t0,0($sp)\n");
                            fprintf(fp , "\taddiu\t$sp,$sp,4\n");
                            fprintf(fp , "\tadd\t$t2,$t0,$t1\n");
                            fprintf(fp , "\taddiu\t$sp,$sp,-4\n");
                            fprintf(fp , "\tsw\t$t2,0($sp)\n");
                            break;
                        default:
                            break;
                    }
                    break;
                
                default:
                    break;
            }
        }
        else if (strcmp(root->name , "MINUS") == 0){
            mips_code_expression(fp , root->child);
            mips_code_expression(fp , root->child->sibling);
            switch (root->child->vartype){
                case INTEGER:
                    switch (root->child->sibling->vartype){
                        case INTEGER:
                            fprintf(fp , "\tlw\t$t1,0($sp)\n");
                            fprintf(fp , "\taddiu\t$sp,$sp,4\n");
                            fprintf(fp , "\tlw\t$t0,0($sp)\n");
                            fprintf(fp , "\taddiu\t$sp,$sp,4\n");
                            fprintf(fp , "\tsub\t$t2,$t0,$t1\n");
                            fprintf(fp , "\taddiu\t$sp,$sp,-4\n");
                            fprintf(fp , "\tsw\t$t2,0($sp)\n");
                            break;
                        default:
                            break;
                    }
                    break;
                
                default:
                    break;
            }
        }
        else if (strcmp(root->name , "MULT") == 0){
            mips_code_expression(fp , root->child);
            mips_code_expression(fp , root->child->sibling);
            switch (root->child->vartype){
                case INTEGER:
                    switch (root->child->sibling->vartype){
                        case INTEGER:
                            fprintf(fp , "\tlw\t$t1,0($sp)\n");
                            fprintf(fp , "\taddiu\t$sp,$sp,4\n");
                            fprintf(fp , "\tlw\t$t0,0($sp)\n");
                            fprintf(fp , "\taddiu\t$sp,$sp,4\n");
                            fprintf(fp , "\tmult\t$t0,$t1\n");
                            fprintf(fp , "\tmflo\t$t2\n");
                            fprintf(fp , "\taddiu\t$sp,$sp,-4\n");
                            fprintf(fp , "\tsw\t$t2,0($sp)\n");
                            break;
                        default:
                            break;
                    }
                    break;
                
                default:
                    break;
            }
        }
        else if (strcmp(root->name , "DIVIDE") == 0){
            mips_code_expression(fp , root->child);
            mips_code_expression(fp , root->child->sibling);
            switch (root->child->vartype){
                case INTEGER:
                    switch (root->child->sibling->vartype){
                        case INTEGER:
                            fprintf(fp , "\tlw\t$t1,0($sp)\n");
                            fprintf(fp , "\taddiu\t$sp,$sp,4\n");
                            fprintf(fp , "\tlw\t$t0,0($sp)\n");
                            fprintf(fp , "\taddiu\t$sp,$sp,4\n");
                            fprintf(fp , "\tdiv\t$t0,$t1\n");
                            fprintf(fp , "\tmflo\t$t2\n");  // mfhi will store remainder
                            fprintf(fp , "\taddiu\t$sp,$sp,-4\n");
                            fprintf(fp , "\tsw\t$t2,0($sp)\n");
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        else if (strcmp(root->name , "MODULO") == 0){
            mips_code_expression(fp , root->child);
            mips_code_expression(fp , root->child->sibling);
            switch (root->child->vartype){
                case INTEGER:
                    switch (root->child->sibling->vartype){
                        case INTEGER:
                            fprintf(fp , "\tlw\t$t1,0($sp)\n");
                            fprintf(fp , "\taddiu\t$sp,$sp,4\n");
                            fprintf(fp , "\tlw\t$t0,0($sp)\n");
                            fprintf(fp , "\taddiu\t$sp,$sp,4\n");
                            fprintf(fp , "\tdiv\t$t0,$t1\n");
                            fprintf(fp , "\tmfhi\t$t2\n");  // mflo will store quotient
                            fprintf(fp , "\taddiu\t$sp,$sp,-4\n");
                            fprintf(fp , "\tsw\t$t2,0($sp)\n");
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        else if (strcmp(root->name , "LESSTHAN") == 0){
            mips_code_expression(fp , root->child);
            mips_code_expression(fp , root->child->sibling);
            switch (root->child->vartype){
                case INTEGER:
                    switch (root->child->sibling->vartype){
                        case INTEGER:
                            fprintf(fp , "\tlw\t$t1,0($sp)\n");
                            fprintf(fp , "\taddiu\t$sp,$sp,4\n");
                            fprintf(fp , "\tlw\t$t0,0($sp)\n");
                            fprintf(fp , "\taddiu\t$sp,$sp,4\n");
                            fprintf(fp , "\tslt\t$t2,$t0,$t1\n");
                            fprintf(fp , "\taddiu\t$sp,$sp,-4\n");
                            fprintf(fp , "\tsw\t$t2,0($sp)\n");
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        else if (strcmp(root->name , "GREATERTHAN") == 0){
            mips_code_expression(fp , root->child);
            mips_code_expression(fp , root->child->sibling);
            switch (root->child->vartype){
                case INTEGER:
                    switch (root->child->sibling->vartype){
                        case INTEGER:
                            fprintf(fp , "\tlw\t$t1,0($sp)\n");
                            fprintf(fp , "\taddiu\t$sp,$sp,4\n");
                            fprintf(fp , "\tlw\t$t0,0($sp)\n");
                            fprintf(fp , "\taddiu\t$sp,$sp,4\n");
                            fprintf(fp , "\tslt\t$t2,$t1,$t0\n");
                            fprintf(fp , "\taddiu\t$sp,$sp,-4\n");
                            fprintf(fp , "\tsw\t$t2,0($sp)\n");
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        else if (strcmp(root->name , "GREATERTHANOREQUAL") == 0){
            mips_code_expression(fp , root->child);
            mips_code_expression(fp , root->child->sibling);
            switch (root->child->vartype){
                case INTEGER:
                    switch (root->child->sibling->vartype){
                        case INTEGER:
                            fprintf(fp , "\tlw\t$t1,0($sp)\n");
                            fprintf(fp , "\taddiu\t$sp,$sp,4\n");
                            fprintf(fp , "\tlw\t$t0,0($sp)\n");
                            fprintf(fp , "\taddiu\t$sp,$sp,4\n");
                            fprintf(fp , "\tslt\t$t2,$t0,$t1\n");
                            fprintf(fp , "\txori\t$t2,$t2,1\n");
                            fprintf(fp , "\taddiu\t$sp,$sp,-4\n");
                            fprintf(fp , "\tsw\t$t2,0($sp)\n");
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        else if (strcmp(root->name , "LESSTHANOREQUAL") == 0){
            mips_code_expression(fp , root->child);
            mips_code_expression(fp , root->child->sibling);
            switch (root->child->vartype){
                case INTEGER:
                    switch (root->child->sibling->vartype){
                        case INTEGER:
                            fprintf(fp , "\tlw\t$t1,0($sp)\n");
                            fprintf(fp , "\taddiu\t$sp,$sp,4\n");
                            fprintf(fp , "\tlw\t$t0,0($sp)\n");
                            fprintf(fp , "\taddiu\t$sp,$sp,4\n");
                            fprintf(fp , "\tslt\t$t2,$t1,$t0\n");
                            fprintf(fp , "\txori\t$t2,$t2,1\n");
                            fprintf(fp , "\taddiu\t$sp,$sp,-4\n");
                            fprintf(fp , "\tsw\t$t2,0($sp)\n");
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        else if (strcmp(root->name , "NOTEQUAL") == 0){
            mips_code_expression(fp , root->child);
            mips_code_expression(fp , root->child->sibling);
            switch (root->child->vartype){
                case INTEGER:
                    switch (root->child->sibling->vartype){
                        case INTEGER:
                            fprintf(fp , "\tlw\t$t1,0($sp)\n");
                            fprintf(fp , "\taddiu\t$sp,$sp,4\n");
                            fprintf(fp , "\tlw\t$t0,0($sp)\n");
                            fprintf(fp , "\taddiu\t$sp,$sp,4\n");
                            fprintf(fp , "\txor\t$t2,$t0,$t1\n");         // t2 = a ^ b
                            fprintf(fp , "\tsltu\t$t2,$zero,$t2\n");       // t2 = (a != b)
                            fprintf(fp , "\taddiu\t$sp,$sp,-4\n");
                            fprintf(fp , "\tsw\t$t2,0($sp)\n");
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        else if (strcmp(root->name , "EQUALEQUAL") == 0){
            mips_code_expression(fp , root->child);
            mips_code_expression(fp , root->child->sibling);
            switch (root->child->vartype){
                case INTEGER:
                    switch (root->child->sibling->vartype){
                        case INTEGER:
                            fprintf(fp , "\tlw\t$t1,0($sp)\n");
                            fprintf(fp , "\taddiu\t$sp,$sp,4\n");
                            fprintf(fp , "\tlw\t$t0,0($sp)\n");
                            fprintf(fp , "\taddiu\t$sp,$sp,4\n");
                            fprintf(fp , "\txor\t$t2,$t0,$t1\n");         // t2 = a ^ b
                            fprintf(fp , "\tsltu\t$t2,$t2,1\n");       // t2 = (a != b)
                            fprintf(fp , "\taddiu\t$sp,$sp,-4\n");
                            fprintf(fp , "\tsw\t$t2,0($sp)\n");
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        else if (strcmp(root->name , "LOGICAL_AND") == 0){
            mips_code_expression(fp , root->child);
            mips_code_expression(fp , root->child->sibling);
            switch (root->child->vartype){
                case INTEGER:
                    switch (root->child->sibling->vartype){
                        case INTEGER:
                            fprintf(fp , "\tlw\t$t1,0($sp)\n");
                            fprintf(fp , "\taddiu\t$sp,$sp,4\n");
                            fprintf(fp , "\tlw\t$t0,0($sp)\n");
                            fprintf(fp , "\taddiu\t$sp,$sp,4\n");
                            fprintf(fp , "\tsltu\t$t3,$zero,$t0\n");        // t3 = (operand1 != 0)
                            fprintf(fp , "\tsltu\t$t4,$zero,$t1\n");        // t4 = (operand2 != 0)
                            fprintf(fp , "\tand\t$t2,$t3,$t4\n");           // t2 = t3 && t4      // t2 = (a != b)
                            fprintf(fp , "\taddiu\t$sp,$sp,-4\n");
                            fprintf(fp , "\tsw\t$t2,0($sp)\n");
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        else if (strcmp(root->name , "LOGICAL_OR") == 0){
            mips_code_expression(fp , root->child);
            mips_code_expression(fp , root->child->sibling);
            switch (root->child->vartype){
                case INTEGER:
                    switch (root->child->sibling->vartype){
                        case INTEGER:
                            fprintf(fp , "\tlw\t$t1,0($sp)\n");
                            fprintf(fp , "\taddiu\t$sp,$sp,4\n");
                            fprintf(fp , "\tlw\t$t0,0($sp)\n");
                            fprintf(fp , "\taddiu\t$sp,$sp,4\n");
                            fprintf(fp , "\tsltu\t$t3,$zero,$t0\n");        // t3 = (operand1 != 0)
                            fprintf(fp , "\tsltu\t$t4,$zero,$t1\n");        // t4 = (operand2 != 0)
                            fprintf(fp , "\tor\t$t2,$t3,$t4\n");           // t2 = t3 && t4      // t2 = (a != b)
                            fprintf(fp , "\taddiu\t$sp,$sp,-4\n");
                            fprintf(fp , "\tsw\t$t2,0($sp)\n");
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        else{
            if (strcmp(root->type ,  "array") == 0){
                switch (root->vartype){
                case INTEGER:
                    fprintf(fp , "\tlui\t$t0,%%hi(%s)\n",root->name);
                    fprintf(fp , "\taddiu\t$t0,$t0,%%lo(%s)\n",root->name);
                    if (root->var_idx){
                        fprintf(fp , "\tlui\t$t1,%%hi(%s)\n",root->var_idx);
                        fprintf(fp , "\tlw\t$t1,%%lo(%s)($t1)\n",root->var_idx);
                        fprintf(fp , "\tsll\t$t1,$t1,2\n");              // t1 = i * 4 (for word offset)
                        fprintf(fp , "\tadd\t$t2,$t0,$t1\n");            // t2 = base + index * 4
                        fprintf(fp , "\tlw\t$t3,0($t2)\n");              // load value at x[i]
                        fprintf(fp , "\taddiu\t$sp,$sp,-4\n");
                        fprintf(fp , "\tsw\t$t3,0($sp)\n");
                    }
                    else{
                        fprintf(fp , "\tlw\t$t1,%d($t0)\n",4*root->idx);
                        fprintf(fp , "\taddiu\t$sp,$sp,-4\n");
                        fprintf(fp , "\tsw\t$t1,0($sp)\n");
                    }
                    break;
                default:
                    break;
                }
            }
            else{
                switch (root->vartype){
                case INTEGER:
                    fprintf(fp , "\tlui\t$t0,%%hi(%s)\n",root->name);
                    fprintf(fp , "\tlw\t$t0,%%lo(%s)($t0)\n",root->name);
                    fprintf(fp , "\taddiu\t$sp,$sp,-4\n");
                    fprintf(fp , "\tsw\t$t0,0($sp)\n");
                    break;
                
                default:
                    break;
                }
            }
        }
    }
    else{
        switch (root->vartype){
        case INTEGER:
            fprintf(fp , "\tli\t$t0,%d\n", root->value.num);
            fprintf(fp , "\taddiu\t$sp,$sp,-4\n");
            fprintf(fp , "\tsw\t$t0,0($sp)\n");
            break;
        default:
            break;
        }
    }
}

void mips_code_stmt_list(FILE* fp , tree* root){
    int global_pointer_setup = 0;
    tree* child = root->child;
    while (child){
        if (strcmp(child->name , "ASSIGN_STMT") == 0){
            int reg_count = 0;
            tree* var = child->child;
            tree* exp = child->child->sibling;
            mips_code_expression(fp , exp);
            if (strcmp(var->type , "array") == 0){
                switch (var->vartype){
                case INTEGER:
                    fprintf(fp , "\tlw\t$t4,0($sp)\n");
                    fprintf(fp , "\taddiu\t$sp,$sp,4\n");
                    fprintf(fp , "\tlui\t$t0,%%hi(%s)\n",var->name);
                    fprintf(fp , "\taddiu\t$t0,$t0,%%lo(%s)\n",var->name);
                    if (var->var_idx){
                        fprintf(fp , "\tlui\t$t1,%%hi(%s)\n",var->var_idx);
                        fprintf(fp , "\tlw\t$t1,%%lo(%s)($t1)\n",var->var_idx);
                        fprintf(fp , "\tsll\t$t1,$t1,2\n");              // t1 = i * 4 (for word offset)
                        fprintf(fp , "\tadd\t$t2,$t0,$t1\n");            // t2 = base + index * 4
                        fprintf(fp , "\tsw\t$t4,0($t2)\n");              // store value at x[i]
                    }
                    else{
                        fprintf(fp , "\tsw\t$t4,%d($t0)\n",4*var->idx);
                    }
                    break;
                
                default:
                    break;
                }
            }
            else{
                switch (var->vartype){
                case INTEGER:
                    fprintf(fp , "\tlw\t$t0,0($sp)\n");
                    fprintf(fp , "\taddiu\t$sp,$sp,4\n");
                    fprintf(fp , "\tlui\t$t1,%%hi(%s)\n" , var->name);
                    fprintf(fp , "\tsw\t$t0,%%lo(%s)($t1)\n" , var->name);
                    break;
                
                default:
                    break;
                }
            }
        }
        else if (strcmp(child->name , "COND_STMT") == 0){

        }
        else if (strcmp(child->name , "CALL") == 0){
            if (!global_pointer_setup){
                fprintf(fp , "\tlui\t$28,%%hi(__gnu_local_gp)\n");
	            fprintf(fp , "\taddiu\t$28,$28,%%lo(__gnu_local_gp)\t\t\t# $gp ($28) is set up for accessing global data like scanf\n");
	            fprintf(fp , "\t.cprestore\t16\t\t\t# tells compiler to store $gp at offset 16 in the stack frame\n");
                global_pointer_setup = 1;
            }
            mips_code_call_func(fp , child->child);
        }
        else if (strcmp(child->name , "break") == 0){

        }
        child = child->sibling;
    }
}

void generate_mips_code(FILE* fp , tree* root){
    fprintf(fp , "\t.file\t1\t\"code.c\"\n");
    fprintf(fp , "\t.section\t.mdebug.abi32\n");
    fprintf(fp , "\t.previous\n");
    fprintf(fp , "\t.nan\tlegacy\n");
    fprintf(fp , "\t.module\tfp=32\n");
    fprintf(fp , "\t.module\tnooddspreg\n");
    fprintf(fp , "\t.abicalls\n");
    mips_code_globl_decl(fp , root->child);
    fprintf(fp , "\t.rdata\n");
    fprintf(fp , "\t.align\t2\n");
    fprintf(fp , "$LC0:\n");
    fprintf(fp , "\t.ascii\t\"%%d\\000\"\n");
    fprintf(fp , "\t.align\t2\n");
    fprintf(fp , "$LC1:\n");
    fprintf(fp , "\t.ascii\t\"%%d \\000\"\n");
    fprintf(fp , "\t.align\t2\n");
    fprintf(fp , "$LC2:\n");
    fprintf(fp , "\t.ascii\t\"%%d\\012\\000\"\n");
    fprintf(fp , "\t.text\n");
    fprintf(fp , "\t.align\t2\n");
    fprintf(fp , "\t.globl\tmain\n");
    fprintf(fp , "\t.set\tnomips16\n");
	fprintf(fp , "\t.set\tnomicromips\n");
    fprintf(fp , "\t.ent\tmain\n");
	fprintf(fp , "\t.type\tmain,\t@function\n");
    fprintf(fp , "main:\n");
	fprintf(fp , "\t.frame\t$fp,32,$31\t\t# vars= 0, regs= 3/0, args= 16, gp= 8\n");
	fprintf(fp , "\t.mask\t0xc0000000,-4\t\t\t# tells debugger which callee-saved registers are saved ($fp and $ra)\n");
	fprintf(fp , "\t.fmask\t0x00000000,0\t\t\t# no floating-point registers saved\n");
	fprintf(fp , "\t.set\tnoreorder\n");
	fprintf(fp , "\t.set\tnomacro\n");
	fprintf(fp , "\taddiu\t$sp,$sp,-32\t\t\t# make space for 32 bytes on the stack\n");
    fprintf(fp , "\tsw\t$31,28($sp)\t\t\t# save return address ($ra = $31) at offset 28\n");
	fprintf(fp , "\tsw\t$fp,24($sp)\n");
	fprintf(fp , "\tmove\t$fp,$sp\n");
    mips_code_stmt_list(fp , root->child->sibling);
    fprintf(fp , "\tmove\t$2,$0\t\t\t# set return value to 0 (in $v0 = $2)\n");
	fprintf(fp , "\tmove\t$sp,$fp\n");
	fprintf(fp , "\tlw\t$31,28($sp)\t\t\t# restore return address\n");
	fprintf(fp , "\tlw\t$fp,24($sp)\t\t\t# restore old frame pointer\n");
	fprintf(fp , "\taddiu\t$sp,$sp,32\n");
	fprintf(fp , "\tjr\t$31\n");
	fprintf(fp , "\tnop\n\n");
	fprintf(fp , "\t.set\tmacro\n");
	fprintf(fp , "\t.set\treorder\n");
	fprintf(fp ,  "\t.end\tmain\n");
	fprintf(fp , "\t.size\tmain,\t.-main\n");
	fprintf(fp , "\t.ident\t\"GCC: (Ubuntu 10.5.0-4ubuntu2) 10.5.0\"\n");
	fprintf(fp , "\t.section\t.note.GNU-stack,\"\",@progbits\n");
}