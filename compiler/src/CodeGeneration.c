#include "./../include/CodeGeneration.h"
stack* st;
stack* st2;

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

void mips_code_expression(FILE* fp , tree* root){
    if (root->name){
        if (strcmp(root->name , "PLUS") == 0 ||
            strcmp(root->name , "MINUS") == 0 ||
            strcmp(root->name , "MULT") == 0 ||
            strcmp(root->name , "DIVIDE") == 0 ||
            strcmp(root->name , "MODULO") == 0 || 
            strcmp(root->name , "LESSTHAN") == 0 || 
            strcmp(root->name , "GREATERTHAN") == 0 ||
            strcmp(root->name , "GREATERTHANOREQUAL") == 0 ||
            strcmp(root->name , "LESSTHANOREQUAL") == 0 ||
            strcmp(root->name , "NOTEQUAL") == 0 ||
            strcmp(root->name , "EQUALEQUAL") == 0 ||
            strcmp(root->name , "LOGICAL_AND") == 0 ||
            strcmp(root->name , "LOGICAL_OR") == 0
        ){
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

                            if (strcmp(root->name , "PLUS") == 0){
                                fprintf(fp , "\tadd\t$t2,$t0,$t1\n");
                            } else if (strcmp(root->name , "MINUS") == 0){
                                fprintf(fp , "\tsub\t$t2,$t0,$t1\n");
                            } else if (strcmp(root->name ,  "MULT") == 0){
                                fprintf(fp , "\tmult\t$t0,$t1\n");
                                fprintf(fp , "\tmflo\t$t2\n");
                            } else if (strcmp(root->name , "DIVIDE") == 0){
                                fprintf(fp , "\tdiv\t$t0,$t1\n");
                                fprintf(fp , "\tmflo\t$t2\n");  // mfhi will store remainder
                            } else if (strcmp(root->name , "MODULO") == 0){
                                fprintf(fp , "\tdiv\t$t0,$t1\n");
                                fprintf(fp , "\tmfhi\t$t2\n");  // mflo will store quotient
                            } else if (strcmp(root->name , "LESSTHAN") == 0){
                                fprintf(fp , "\tslt\t$t2,$t0,$t1\n");
                            } else if (strcmp(root->name , "GREATERTHAN") == 0){
                                fprintf(fp , "\tslt\t$t2,$t1,$t0\n");
                            } else if (strcmp(root->name , "GREATERTHANOREQUAL") == 0){
                                fprintf(fp , "\tslt\t$t2,$t0,$t1\n");
                                fprintf(fp , "\txori\t$t2,$t2,1\n");
                            } else if (strcmp(root->name , "LESSTHANOREQUAL") == 0){
                                fprintf(fp , "\tslt\t$t2,$t1,$t0\n");
                                fprintf(fp , "\txori\t$t2,$t2,1\n");
                            } else if (strcmp(root->name , "NOTEQUAL") == 0){
                                fprintf(fp , "\txor\t$t2,$t0,$t1\n");         // t2 = a ^ b
                                fprintf(fp , "\tsltu\t$t2,$zero,$t2\n");       // t2 = (a != b)
                            } else if (strcmp(root->name , "EQUALEQUAL") == 0){
                                fprintf(fp , "\txor\t$t2,$t0,$t1\n");         // t2 = a ^ b
                                fprintf(fp , "\tsltu\t$t2,$t2,1\n");       // t2 = (a != b)
                            } else if (strcmp(root->name , "LOGICAL_AND") == 0){
                                fprintf(fp , "\tsltu\t$t3,$zero,$t0\n");        // t3 = (operand1 != 0)
                                fprintf(fp , "\tsltu\t$t4,$zero,$t1\n");        // t4 = (operand2 != 0)
                                fprintf(fp , "\tand\t$t2,$t3,$t4\n");           // t2 = t3 && t4      // t2 = (a != b)
                            } else if (strcmp(root->name , "LOGICAL_OR") == 0){
                                fprintf(fp , "\tsltu\t$t3,$zero,$t0\n");        // t3 = (operand1 != 0)
                                fprintf(fp , "\tsltu\t$t4,$zero,$t1\n");        // t4 = (operand2 != 0)
                                fprintf(fp , "\tor\t$t2,$t3,$t4\n");           // t2 = t3 && t4      // t2 = (a != b)
                            }

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
            node* variable = search_string(root->name);
            root->type = strdup(variable->type);
            if (strcmp(root->type ,  "array") == 0){
                switch (root->vartype){
                case INTEGER:
                    if (root->var_idx){
                        mips_code_expression(fp, root->child);
                        fprintf(fp , "\tlui\t$t0,%%hi(%s)\n",root->name);
                        fprintf(fp , "\taddiu\t$t0,$t0,%%lo(%s)\n",root->name);
                        fprintf(fp , "\tlw\t$t1,0($sp)\n");
                        fprintf(fp , "\taddiu\t$sp,$sp,4\n");
                        fprintf(fp , "\tsll\t$t1,$t1,2\n");              // t1 = i * 4 (for word offset)
                        fprintf(fp , "\tadd\t$t2,$t0,$t1\n");            // t2 = base + index * 4
                        fprintf(fp , "\tlw\t$t3,0($t2)\n");              // load value at x[i]
                        fprintf(fp , "\taddiu\t$sp,$sp,-4\n");
                        fprintf(fp , "\tsw\t$t3,0($sp)\n");
                    }
                    else{
                        fprintf(fp , "\tlui\t$t0,%%hi(%s)\n",root->name);
                        fprintf(fp , "\taddiu\t$t0,$t0,%%lo(%s)\n",root->name);
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

void mips_code_assign_stmt(FILE* fp , tree* root){
    tree* var = root->child;
    tree* exp = root->child->sibling;
    if (exp->name && strcmp(exp->name , "PLUSPLUS") == 0){
        node* variable = search_string(var->name);
        var->type = strdup(variable->type);
        if (strcmp(var->type ,  "array") == 0){
            switch (var->vartype){
            case INTEGER:
                if (var->var_idx){
                    mips_code_expression(fp, var->child);
                    fprintf(fp , "\tlui\t$t0,%%hi(%s)\n",var->name);
                    fprintf(fp , "\taddiu\t$t0,$t0,%%lo(%s)\n",var->name);
                    fprintf(fp , "\tlw\t$t1,0($sp)\n");
                    fprintf(fp , "\taddiu\t$sp,$sp,4\n");
                    fprintf(fp , "\tsll\t$t1,$t1,2\n");              // t1 = i * 4 (for word offset)
                    fprintf(fp , "\tadd\t$t2,$t0,$t1\n");            // t2 = base + index * 4
                    fprintf(fp , "\tlw\t$t3,0($t2)\n");              // load value at x[i]
                    fprintf(fp , "\taddi\t$t3,$t3,1\n");            // incrementing by 1
                    fprintf(fp , "\taddiu\t$sp,$sp,-4\n");
                    fprintf(fp , "\tsw\t$t3,0($sp)\n");
                }
                else{
                    fprintf(fp , "\tlui\t$t0,%%hi(%s)\n",var->name);
                    fprintf(fp , "\taddiu\t$t0,$t0,%%lo(%s)\n",var->name);
                    fprintf(fp , "\tlw\t$t1,%d($t0)\n",4*var->idx);
                    fprintf(fp , "\taddi\t$t1,$t1,1\n");            // incrementing by 1
                    fprintf(fp , "\taddiu\t$sp,$sp,-4\n");
                    fprintf(fp , "\tsw\t$t1,0($sp)\n");
                }
                break;
            default:
                break;
            }
        }
        else{
            switch (var->vartype){
            case INTEGER:
                fprintf(fp , "\tlui\t$t0,%%hi(%s)\n",var->name);
                fprintf(fp , "\tlw\t$t0,%%lo(%s)($t0)\n",var->name);
                fprintf(fp , "\taddi\t$t0,$t0,1\n");            // incrementing by 1
                fprintf(fp , "\taddiu\t$sp,$sp,-4\n");
                fprintf(fp , "\tsw\t$t0,0($sp)\n");
                break;
            
            default:
                break;
            }
        }
    }
    else mips_code_expression(fp , exp);
    node* variable = search_string(var->name);
    var->type = strdup(variable->type);
    if (strcmp(var->type , "array") == 0){
        switch (var->vartype){
        case INTEGER:
            fprintf(fp , "\tlui\t$t0,%%hi(%s)\n",var->name);
            fprintf(fp , "\taddiu\t$t0,$t0,%%lo(%s)\n",var->name);
            if (var->var_idx){
                fprintf(fp , "\taddiu\t$sp, $sp, -4\n");
                fprintf(fp , "\tsw\t$t0, 0($sp)\n");
                // Evaluate the index expression
                mips_code_expression(fp, var->child);
                // Pop evaluated index → $t1
                fprintf(fp , "\tlw\t$t1, 0($sp)\n");
                fprintf(fp , "\taddiu\t$sp, $sp, 4\n");
                // Restore $t0 (base address)
                fprintf(fp , "\tlw\t$t0, 0($sp)\n");
                fprintf(fp , "\taddiu\t$sp, $sp, 4\n");
                fprintf(fp , "\tsll\t$t1,$t1,2\n");              // t1 = i * 4 (for word offset)
                fprintf(fp , "\tadd\t$t2,$t0,$t1\n");            // t2 = base + index * 4
                fprintf(fp , "\tlw\t$t4,0($sp)\n");             // loading value from the stack to store in the given variable
                fprintf(fp , "\taddiu\t$sp,$sp,4\n");
                fprintf(fp , "\tsw\t$t4,0($t2)\n");              // store value at x[i]
            }
            else{
                fprintf(fp , "\tlw\t$t4,0($sp)\n");
                fprintf(fp , "\taddiu\t$sp,$sp,4\n");
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

int get_new_label_id() {
    static int label_counter = 1;
    return label_counter++;
}

void mips_code_for_loop(FILE* fp , tree* root){
    int label_start = get_new_label_id();
    int label_body  = get_new_label_id();
    int label_end   = get_new_label_id();
    int label_expr = get_new_label_id();
    pushOnStack(st , label_end); // will be useful in break statement
    pushOnStack(st2 , label_expr); // will be useful in continue statement
    tree* for_init = root->child;
    tree* for_cond = for_init->sibling;
    tree* for_expr = for_cond->sibling;
    tree* for_body = for_expr->sibling;
    if (for_init->child){
        mips_code_assign_stmt(fp , for_init->child);
    }
    fprintf(fp, "L%d:\n", label_start);
    if (for_cond->child){
        mips_code_expression(fp , for_cond->child);
        fprintf(fp , "\tlw\t$t2, 0($sp)\n");
        fprintf(fp , "\taddiu\t$sp, $sp, 4\n");
        fprintf(fp , "\tbeq\t$t2, $0, L%d\n", label_end);
    }
    fprintf(fp, "L%d:\n", label_body);
    if (for_body->child){
        mips_code_stmt_list(fp , for_body);
    }
    fprintf(fp , "L%d:\n", label_expr);
    if (for_expr->child){
        mips_code_assign_stmt(fp , for_expr->child);
    }
    fprintf(fp , "\tj\tL%d\n", label_start);
    fprintf(fp , "L%d:\n", label_end);
}

void mips_code_if_statement(FILE* fp , tree* root){
    int label_else = get_new_label_id();
    int label_end = get_new_label_id();
    tree* if_cond = root->child;
    tree* if_body = if_cond->sibling;
    mips_code_expression(fp , if_cond->child);
    fprintf(fp , "\tlw\t$t2,0($sp)\n");
    fprintf(fp , "\taddiu\t$sp,$sp,4\n");
    fprintf(fp , "\tbeq\t$t2,$0,L%d\n",label_else);
    mips_code_stmt_list(fp , if_body);
    fprintf(fp , "\tnop\n");
    fprintf(fp , "\tj\tL%d\n",label_end);
    fprintf(fp , "L%d:\n",label_else);
    if (root->sibling && root->sibling->child){
        tree* else_body = root->sibling->child;
        mips_code_stmt_list(fp , else_body);
    }
    fprintf(fp , "L%d:\n",label_end);
}

void mips_code_while_loop(FILE* fp , tree* root){
    int label_while = get_new_label_id();
    int label_end = get_new_label_id();
    int label_expr = get_new_label_id();
    pushOnStack(st , label_end); // will be useful in break statement
    pushOnStack(st2 , label_expr); // will be useful in continue statement
    tree* while_cond = root->child;
    tree* while_body = while_cond->sibling;
    fprintf(fp , "L%d:\n" , label_while);
    mips_code_stmt_list(fp , while_body);
    fprintf(fp , "L%d:\n",label_expr);
    mips_code_expression(fp , while_cond->child);
    fprintf(fp , "\tlw\t$t2,0($sp)\n");
    fprintf(fp , "\taddiu\t$sp,$sp,4\n");
    fprintf(fp , "\tbne\t$t2,$0,L%d\n",label_while);
    fprintf(fp , "\tnop\n");
    fprintf(fp , "L%d:\n",label_end);
}

void mips_code_stmt_list(FILE* fp , tree* root){
    tree* child = root->child;
    while (child){
        if (strcmp(child->name , "ASSIGN_STMT") == 0){
            mips_code_assign_stmt(fp , child);
        }
        else if (strcmp(child->name , "COND_STMT") == 0){
            if (strcmp(child->child->name , "IF_STMT") == 0){
                mips_code_if_statement(fp , child->child);
            }
            else if (strcmp(child->child->name , "FOR_STMT") == 0){
                mips_code_for_loop(fp , child->child);
            }
            else if (strcmp(child->child->name , "WHILE_STMT") == 0){
                mips_code_while_loop(fp , child->child);
            }
        }
        else if (strcmp(child->name , "CALL") == 0){
            mips_code_call_func(fp , child->child);
        }
        else if (strcmp(child->name , "break") == 0){
            fprintf(fp , "\tnop\n");
            stacknode* st_node = popFromStack(st);
            fprintf(fp , "\tj\tL%d\n",st_node->number);
            fprintf(fp , "\tnop\n");
        }
        else if (strcmp(child->name , "continue") == 0){
            fprintf(fp , "\tnop\n");
            stacknode* st_node = popFromStack(st2);
            fprintf(fp , "\tj\tL%d\n",st_node->number);
            fprintf(fp , "\tnop\n");
        }
        child = child->sibling;
    }
}

void mips_code_call_func(FILE* fp , tree* func_name){
    if (strcmp(func_name->name , "WRITE") == 0){
        tree* write_vars = func_name->sibling;
        while (write_vars){
            mips_code_expression(fp , write_vars);
            fprintf(fp , "\tlw\t$5,0($sp)\n");            // Load result from stack
            fprintf(fp , "\taddiu\t$sp,$sp,4\n");
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
            fprintf(fp , "\tnop\n\n");
            fprintf(fp , "\tlw\t$28,16($fp)\t\t\t# Restore global pointer\n");
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
                    fprintf(fp , "\tlui\t$t2,%%hi(%s)\n", read_vars->name);
                    fprintf(fp , "\taddiu\t$t2,$t2,%%lo(%s)\n", read_vars->name);
                    if (read_vars->var_idx){
                        fprintf(fp , "\taddiu\t$sp, $sp, -4\n");
                        fprintf(fp , "\tsw\t$t2, 0($sp)\n");
                        mips_code_expression(fp, read_vars->child);
                        fprintf(fp , "\tlw\t$t1, 0($sp)\n");
                        fprintf(fp , "\taddiu\t$sp, $sp, 4\n");
                        fprintf(fp , "\tlw\t$t0, 0($sp)\n");
                        fprintf(fp , "\taddiu\t$sp, $sp, 4\n");
                        fprintf(fp , "\tsll\t$t1,$t1,2\n");              // t1 = i * 4 (for word offset)
                        fprintf(fp , "\tadd\t$5,$t0,$t1\n");
                    }
                    else{
                        fprintf(fp , "\taddi\t$5,$t2,%d\n",4*read_vars->idx);
                    }
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
                    break;
                
                default:
                    break;
                }
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
            read_vars = read_vars->sibling;
        }
    }
}

void generate_mips_code(FILE* fp , tree* root){
    st = init_stack();
    st2 = init_stack();
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
    // setting global pointer for global data like scanf
    fprintf(fp , "\tlui\t$28,%%hi(__gnu_local_gp)\n");
    fprintf(fp , "\taddiu\t$28,$28,%%lo(__gnu_local_gp)\t\t\t# $gp ($28) is set up for accessing global data like scanf\n");
    fprintf(fp , "\t.cprestore\t16\t\t\t# tells compiler to store $gp at offset 16 in the stack frame\n");
    // setting done
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