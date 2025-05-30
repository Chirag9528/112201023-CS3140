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
        if (strcmp(expr->name , "PLUS") == 0 ||
            strcmp(expr->name , "MINUS") == 0 ||
            strcmp(expr->name , "MULT") == 0 ||
            strcmp(expr->name , "DIVIDE") == 0 ||
            strcmp(expr->name , "MODULO") == 0 || 
            strcmp(expr->name , "LESSTHAN") == 0 || 
            strcmp(expr->name , "GREATERTHAN") == 0 ||
            strcmp(expr->name , "GREATERTHANOREQUAL") == 0 ||
            strcmp(expr->name , "LESSTHANOREQUAL") == 0 ||
            strcmp(expr->name , "NOTEQUAL") == 0 ||
            strcmp(expr->name , "EQUALEQUAL") == 0 ||
            strcmp(expr->name , "LOGICAL_AND") == 0 ||
            strcmp(expr->name , "LOGICAL_OR") == 0
        ){
            evaluate_expression(expr->child);
            evaluate_expression(expr->child->sibling);
            switch (expr->child->vartype){
            case INTEGER:
                switch (expr->child->sibling->vartype){
                case INTEGER:
                    if (strcmp(expr->name , "PLUS") == 0){
                        expr->value.num = expr->child->value.num + expr->child->sibling->value.num;
                    } else if (strcmp(expr->name , "MINUS") == 0){
                        expr->value.num = expr->child->value.num - expr->child->sibling->value.num;
                    } else if (strcmp(expr->name ,  "MULT") == 0){
                        expr->value.num = expr->child->value.num * expr->child->sibling->value.num;
                    } else if (strcmp(expr->name , "DIVIDE") == 0){
                        expr->value.num = expr->child->value.num / expr->child->sibling->value.num;
                    } else if (strcmp(expr->name , "MODULO") == 0){
                        expr->value.num = expr->child->value.num % expr->child->sibling->value.num;
                    } else if (strcmp(expr->name , "LESSTHAN") == 0){
                        expr->value.bnum = expr->child->value.num < expr->child->sibling->value.num;
                    } else if (strcmp(expr->name , "GREATERTHAN") == 0){
                        expr->value.bnum = expr->child->value.num > expr->child->sibling->value.num;
                    } else if (strcmp(expr->name , "GREATERTHANOREQUAL") == 0){
                        expr->value.bnum = expr->child->value.num >= expr->child->sibling->value.num;
                    } else if (strcmp(expr->name , "LESSTHANOREQUAL") == 0){
                        expr->value.bnum = expr->child->value.num <= expr->child->sibling->value.num;
                    } else if (strcmp(expr->name , "NOTEQUAL") == 0){
                        expr->value.bnum = expr->child->value.num != expr->child->sibling->value.num;
                    } else if (strcmp(expr->name , "EQUALEQUAL") == 0){
                        expr->value.bnum = expr->child->value.num == expr->child->sibling->value.num;
                    } else if (strcmp(expr->name , "LOGICAL_AND") == 0){
                        expr->value.bnum = expr->child->value.num && expr->child->sibling->value.num;
                    } else if (strcmp(expr->name , "LOGICAL_OR") == 0){
                        expr->value.bnum = expr->child->value.num || expr->child->sibling->value.num;
                    }
                    break;
                case BOOLEAN:
                    if (strcmp(expr->name , "PLUS") == 0){
                        expr->value.num = expr->child->value.num + expr->child->sibling->value.bnum;
                    } else if (strcmp(expr->name , "MINUS") == 0){
                        expr->value.num = expr->child->value.num - expr->child->sibling->value.bnum;
                    } else if (strcmp(expr->name ,  "MULT") == 0){
                        expr->value.num = expr->child->value.num * expr->child->sibling->value.bnum;
                    } else if (strcmp(expr->name , "DIVIDE") == 0){
                        expr->value.num = expr->child->value.num / expr->child->sibling->value.bnum;
                    } else if (strcmp(expr->name , "MODULO") == 0){
                        expr->value.num = expr->child->value.num % expr->child->sibling->value.bnum;
                    } else if (strcmp(expr->name , "LESSTHAN") == 0){
                        expr->value.bnum = expr->child->value.num < expr->child->sibling->value.bnum;
                    } else if (strcmp(expr->name , "GREATERTHAN") == 0){
                        expr->value.bnum = expr->child->value.num > expr->child->sibling->value.bnum;
                    } else if (strcmp(expr->name , "GREATERTHANOREQUAL") == 0){
                        expr->value.bnum = expr->child->value.num >= expr->child->sibling->value.bnum;
                    } else if (strcmp(expr->name , "LESSTHANOREQUAL") == 0){
                        expr->value.bnum = expr->child->value.num <= expr->child->sibling->value.bnum;
                    } else if (strcmp(expr->name , "NOTEQUAL") == 0){
                        expr->value.bnum = expr->child->value.num != expr->child->sibling->value.bnum;
                    } else if (strcmp(expr->name , "EQUALEQUAL") == 0){
                        expr->value.bnum = expr->child->value.num == expr->child->sibling->value.bnum;
                    } else if (strcmp(expr->name , "LOGICAL_AND") == 0){
                        expr->value.bnum = expr->child->value.num && expr->child->sibling->value.bnum;
                    } else if (strcmp(expr->name , "LOGICAL_OR") == 0){
                        expr->value.bnum = expr->child->value.num || expr->child->sibling->value.bnum;
                    }
                    break;
                default:
                    break;
                }
                break;
            case BOOLEAN:
                switch (expr->child->sibling->vartype){
                case INTEGER:
                    if (strcmp(expr->name , "PLUS") == 0){
                        expr->value.num = expr->child->value.bnum + expr->child->sibling->value.num;
                    } else if (strcmp(expr->name , "MINUS") == 0){
                        expr->value.num = expr->child->value.bnum - expr->child->sibling->value.num;
                    } else if (strcmp(expr->name ,  "MULT") == 0){
                        expr->value.num = expr->child->value.bnum * expr->child->sibling->value.num;
                    } else if (strcmp(expr->name , "DIVIDE") == 0){
                        expr->value.num = expr->child->value.bnum / expr->child->sibling->value.num;
                    } else if (strcmp(expr->name , "MODULO") == 0){
                        expr->value.num = expr->child->value.bnum % expr->child->sibling->value.num;
                    } else if (strcmp(expr->name , "LESSTHAN") == 0){
                        expr->value.bnum = expr->child->value.bnum < expr->child->sibling->value.num;
                    } else if (strcmp(expr->name , "GREATERTHAN") == 0){
                        expr->value.bnum = expr->child->value.bnum > expr->child->sibling->value.num;
                    } else if (strcmp(expr->name , "GREATERTHANOREQUAL") == 0){
                        expr->value.bnum = expr->child->value.bnum >= expr->child->sibling->value.num;
                    } else if (strcmp(expr->name , "LESSTHANOREQUAL") == 0){
                        expr->value.bnum = expr->child->value.bnum <= expr->child->sibling->value.num;
                    } else if (strcmp(expr->name , "NOTEQUAL") == 0){
                        expr->value.bnum = expr->child->value.bnum != expr->child->sibling->value.num;
                    } else if (strcmp(expr->name , "EQUALEQUAL") == 0){
                        expr->value.bnum = expr->child->value.bnum == expr->child->sibling->value.num;
                    } else if (strcmp(expr->name , "LOGICAL_AND") == 0){
                        expr->value.bnum = expr->child->value.bnum && expr->child->sibling->value.num;
                    } else if (strcmp(expr->name , "LOGICAL_OR") == 0){
                        expr->value.bnum = expr->child->value.bnum || expr->child->sibling->value.num;
                    }
                    break;
                case BOOLEAN:
                    if (strcmp(expr->name , "PLUS") == 0){
                        expr->value.num = expr->child->value.bnum + expr->child->sibling->value.bnum;
                    } else if (strcmp(expr->name , "MINUS") == 0){
                        expr->value.num = expr->child->value.bnum - expr->child->sibling->value.bnum;
                    } else if (strcmp(expr->name ,  "MULT") == 0){
                        expr->value.num = expr->child->value.bnum * expr->child->sibling->value.bnum;
                    } else if (strcmp(expr->name , "DIVIDE") == 0){
                        expr->value.num = expr->child->value.bnum / expr->child->sibling->value.bnum;
                    } else if (strcmp(expr->name , "MODULO") == 0){
                        expr->value.num = expr->child->value.bnum % expr->child->sibling->value.bnum;
                    } else if (strcmp(expr->name , "LESSTHAN") == 0){
                        expr->value.bnum = expr->child->value.bnum < expr->child->sibling->value.bnum;
                    } else if (strcmp(expr->name , "GREATERTHAN") == 0){
                        expr->value.bnum = expr->child->value.bnum > expr->child->sibling->value.bnum;
                    } else if (strcmp(expr->name , "GREATERTHANOREQUAL") == 0){
                        expr->value.bnum = expr->child->value.bnum >= expr->child->sibling->value.bnum;
                    } else if (strcmp(expr->name , "LESSTHANOREQUAL") == 0){
                        expr->value.bnum = expr->child->value.bnum <= expr->child->sibling->value.bnum;
                    } else if (strcmp(expr->name , "NOTEQUAL") == 0){
                        expr->value.bnum = expr->child->value.bnum != expr->child->sibling->value.bnum;
                    } else if (strcmp(expr->name , "EQUALEQUAL") == 0){
                        expr->value.bnum = expr->child->value.bnum == expr->child->sibling->value.bnum;
                    } else if (strcmp(expr->name , "LOGICAL_AND") == 0){
                        expr->value.bnum = expr->child->value.bnum && expr->child->sibling->value.bnum;
                    } else if (strcmp(expr->name , "LOGICAL_OR") == 0){
                        expr->value.bnum = expr->child->value.bnum || expr->child->sibling->value.bnum;
                    }
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
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
        else if (strcmp(childs->name , "continue") == 0){
            // need to implement logic
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
                    // evaluate_statements(child);
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
        if (root->type && strcmp(root->type,"array") == 0){
            if (root->var_idx){
                printf("%s\n",root->name);
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

