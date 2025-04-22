/*
 *   This file is part of SIL Compiler.
 *
 *  SIL Compiler is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  SIL Compiler is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with SIL Compiler.  If not, see <http://www.gnu.org/licenses/>.
 */

%{	
#include <stdio.h>
#include "../include/Symboltable.h"
#include "../include/SyntaxTree.h"
// #define YYSTYPE double
int yylex();
void yyerror( char* );

int insert_string(char* ,variable_type , char* , int);
int update_string(char* , void* , int);
node* search_string(char*);
void update_type_size(char* , char* , int);
void print_hash_table(void);

void evaluate_expression(tree* expr);
void evaluate_tree(tree* root);
tree* createnode(char* , int , int , valunion* , variable_type , char* , tree* , tree*);
void print(tree* , int);
void generate_mips_code(FILE* , tree*);

variable_type vartype; 

%}

%union {
	char* name;
	int num;
	struct __tree* treenode;
}


%token BEG END
%token T_INT
%token T_BOOL
%token T_STRING
%token READ WRITE
%token DECL ENDDECL
%token <name> VAR
%token <num> NUM
%token IF THEN ELSE ENDIF
%token LOGICAL_AND LOGICAL_NOT LOGICAL_OR
%token EQUALEQUAL LESSTHANOREQUAL GREATERTHANOREQUAL NOTEQUAL
%token PLUSPLUS
%token FOR 
%token BREAK
%token WHILE DO
/* %token ENDWHILE */
%token T F 
%token MAIN RETURN

%left LOGICAL_AND LOGICAL_OR
%left LOGICAL_NOT
%left '<' '>'
%left EQUALEQUAL LESSTHANOREQUAL GREATERTHANOREQUAL NOTEQUAL
%left '+' '-'
%left '*' '/'
%left '%'

%type <treenode> expr var_expr assign_stmt Gid Glist ret_type Gdecl statement write_stmt read_stmt stmt_list cond_stmt Gdecl_list Gdecl_sec expr_list var_expr_list
%type <name> str_expr


%%

	Prog	:	Gdecl_sec Fdef_sec MainBlock	{

												}
		| Gdecl_sec BEG stmt_list END			{
												tree* stmt_list = createnode("STMT_LIST" , 0 , 0 , NULL , 0 , NULL , $3 , NULL);
												$1->sibling = stmt_list;
												tree* prog = createnode("PROG" , 0 , 0 , NULL , 0 , NULL , $1 , NULL);
												
												evaluate_tree(prog);

												printf("Abstract Syntax Tree (AST):\n===========================\n\n");
												print(prog , 0);
												printf("\n");

												FILE* fp = fopen("code.s" , "w");
												if (fp == NULL){
													printf("Failed to Open File\n");
												}
												else{
													generate_mips_code(fp , prog);
												}

											}
		;
		
	Gdecl_sec:	DECL Gdecl_list ENDDECL		{
												tree* gdecl_list = createnode("GDECL_LIST", 0 , 0 , NULL , 0 , NULL , $2 , NULL);
												$$ = gdecl_list;
											}
		;
		
	Gdecl_list:  /*  Do nothing */			{
												$$ = NULL;
											}
		| 	Gdecl Gdecl_list				{
												$1->sibling = $2;
												$$ = $1;
											}
		;
		
	Gdecl 	:	ret_type Glist ';'			{
												$1->sibling = $2;
												$$ = createnode("GDECL" , 0 , 0 , NULL , 0 , NULL , $1 , NULL);
											}
		;
		
	ret_type:	T_INT		{ 
								vartype = INTEGER;
								$$ = createnode("integer" , 0 , 0 , NULL , vartype , NULL , NULL , NULL);
								
							}
		|		T_BOOL		{
								vartype = BOOLEAN;
								$$ = createnode("boolean" , 0 , 0 , NULL , vartype , NULL , NULL , NULL);
							}
		|		T_STRING	{
								vartype = STRING;
								$$ = createnode("string" , 0 , 0 , NULL , vartype , NULL , NULL , NULL);
							}		
		;
		
	Glist 	:	Gid				{$$ = $1;}
		| 	func 				{}
		|	Gid ',' Glist 		{
									$1->sibling = $3;
									$$ = $1;
								}
		|	func ',' Glist		{}
		;
	
	Gid	:	VAR		{
						if (vartype == INTEGER){
							$$ = createnode($1,1,0,NULL,vartype,"int",NULL,NULL);
						}
						else if (vartype == BOOLEAN){
							$$ = createnode($1,1,0,NULL,vartype,"bool",NULL,NULL);
						}
						else if (vartype == STRING){
							$$ = createnode($1,1,0,NULL,vartype,"char*",NULL,NULL);
						}
					}
		|	Gid '[' NUM ']'	{	
								$1->type = strdup("array");
								$1->idx = $3;
							}

		;
		
	func 	:	VAR '(' arg_list ')' 					{ 					}
		;
			
	arg_list:	
		|	arg_list1
		;
		
	arg_list1:	arg_list1 ';' arg
		|	arg
		;
		
	arg 	:	arg_type var_list	
		;
		
	arg_type:	T_INT		 {  }
		;

	var_list:	VAR 		 { }
		|	VAR ',' var_list { 	}
		;
		
	Fdef_sec:	
		|	Fdef_sec Fdef
		;
		
	Fdef	:	func_ret_type func_name '(' FargList ')' '{' Ldecl_sec BEG stmt_list ret_stmt END '}'	{	 				}
		;
		
	func_ret_type:	T_INT		{ }
		;
			
	func_name:	VAR		{ 					}
		;
		
	FargList:	arg_list	{ 					}
		;

	ret_stmt:	RETURN expr ';'	{ 					}
		;
			
	MainBlock: 	func_ret_type main '('')''{' Ldecl_sec BEG stmt_list ret_stmt END  '}'		{ 				  	  }
					  
		;
		
	main	:	MAIN		{ 					}
		;
		
	Ldecl_sec:	DECL Ldecl_list ENDDECL	{}
		;

	Ldecl_list:		
		|	Ldecl Ldecl_list
		;

	Ldecl	:	type Lid_list ';'		
		;

	type	:	T_INT			{ }
		;

	Lid_list:	Lid
		|	Lid ',' Lid_list
		;
		
	Lid	:	VAR			{ 						}
		;

	stmt_list:	/* NULL */		{ 
									$$ = NULL; 
								}
		|	statement stmt_list	{		

									$1->sibling = $2;
									$$ = $1;
								}
		|	error ';' 		{  }
		;

	statement:	assign_stmt  ';'		{
											$$ = $1; 
										}
		|	read_stmt ';'				{ 

										}
		|	write_stmt ';'				{
											$$ = $1;
										}
		|	cond_stmt 					{
											$$ = $1;
		 								}
		|	func_stmt ';'				{

		 								}
		|	BREAK ';'					{
											$$ = createnode("break" , 0 , 0 , NULL , 0 , NULL , NULL , NULL);
										}
		;

	read_stmt:	READ '(' var_expr_list ')' 	{
												tree* read_child = createnode("READ" , 0 , 0 , NULL , 0 , NULL , NULL , $3);
												tree* root = createnode("CALL" , 0 , 0 , NULL , 0 , NULL , read_child , NULL);
												$$ = root;
	 									}
		;
	
	var_expr_list:	var_expr			{
											$$ = $1;
										}
			|		var_expr ',' var_expr_list		{
														$1->sibling = $3;
														$$ = $1;
													}
			;

	write_stmt:	WRITE '(' expr_list ')' 	{
												tree* write_child = createnode("WRITE" , 0 , 0 , NULL , 0 , NULL , NULL , $3);
												tree* root = createnode("CALL" , 0 , 0 , NULL , 0 , NULL , write_child , NULL);
												$$ = root;
											}
		 	|	WRITE '(''"' str_expr '"'')'     {
												tree* str_expr = createnode($4 , 0 , 0 , NULL , STRING , "char*" , NULL , NULL);
												tree* write_child = createnode("WRITE" , 0 , 0 , NULL , 0 , NULL , NULL , str_expr);
												tree* root = createnode("CALL" , 0 , 0 , NULL , 0 , NULL , write_child , NULL);
												$$ = root;
		 									}

		;
	expr_list: expr				{
									$$ = $1;
								}
		|	expr ',' expr_list	{
									$1->sibling = $3;
									$$ = $1;
								}
		;
	
	assign_stmt:	/* do nothing */		{ $$ = NULL; }
					|	var_expr PLUSPLUS 	{
												valunion value1;
												tree* plusplus = createnode("PLUSPLUS" , 0 , 0 , &value1 , INTEGER , "int" , NULL , NULL);

												$1->sibling = plusplus;

												valunion value;
												$$ = createnode("ASSIGN_STMT" , 0 , 0 , &value , 0 , NULL , $1 , NULL);
												
											}
					|	var_expr '=' expr 	{	
												valunion value;
												$$ = createnode("ASSIGN_STMT", 0 , 0 , &value , 0 , NULL , $1 , NULL);
												$1->sibling = $3;
										}
		|			var_expr '=' '"' str_expr '"' 	{
														valunion value;
														value.str = strdup("");
														valunion strvalue;
														strvalue.str = strdup($4);
														tree* strnode = createnode(NULL , 1 , 0 , &strvalue , STRING , "char*" , NULL , NULL);
														$1->sibling = strnode;
														$$ = createnode("ASSIGN_STMT" , 0, 0 , &value , $1->vartype , $1->type , $1 , NULL);
													}
		;

	cond_stmt:	IF expr THEN stmt_list ENDIF 	{
													tree* if_body = createnode("if_body" , 0 , 0 , NULL , 0 , NULL , $4 , NULL);
													tree* if_condition = createnode("if_condition" , 0 , 0 , NULL , 0 , NULL , $2 , if_body);
													tree* if_cond = createnode("IF_STMT" , 0 , 0 , NULL , 0 , NULL , if_condition , NULL);
													tree* cond_stmt = createnode("COND_STMT",0 ,0 , NULL , 0 , NULL , if_cond , NULL);
													$$ = cond_stmt;
												}
		|	IF expr THEN stmt_list ELSE stmt_list ENDIF 	{
																tree* else_body = createnode("else_body" , 0 ,0 , NULL , 0 , NULL , $6 , NULL);
																tree* else_cond = createnode("ELSE_STMT" , 0 , 0 , NULL , 0 , NULL , else_body , NULL);
																tree* if_body = createnode("if_body" , 0 , 0 , NULL , 0 , NULL , $4 , NULL);
																tree* if_condition = createnode("if_condition" , 0 , 0 , NULL , 0 , NULL , $2 , if_body);
																tree* if_cond = createnode("IF_STMT" , 0 , 0 , NULL , 0 , NULL , if_condition , NULL);
																tree* cond_stmt = createnode("COND_STMT",0 ,0 , NULL , 0 , NULL , if_cond , NULL);
																if_cond->sibling = else_cond;
																$$ = cond_stmt;
															}
	    |    FOR '(' assign_stmt  ';'  expr ';'  assign_stmt ')' '{' stmt_list '}'  {
																						tree* for_body = createnode("for_body" , 0 , 0 , NULL ,  0 , NULL , $10 , NULL);
																						tree* for_init = createnode("for_init" , 0 , 0 , NULL , 0  , NULL , $3 , NULL);
																						tree* for_cond = createnode("for_condition" , 0 , 0 , NULL , 0  , NULL , $5 , NULL);
																						tree* for_expr = createnode("for_expr" , 0 , 0 , NULL , 0  , NULL , $7 , NULL);
																						for_init->sibling = for_cond;
																						for_cond->sibling = for_expr;
																						for_expr->sibling = for_body;
																						tree* for_stmt = createnode("FOR_STMT" , 0 , 0 , NULL , 0 , NULL , for_init , NULL);
																						$$ = createnode("COND_STMT" , 0 , 0 , NULL , 0 , NULL , for_stmt , NULL);
																					}
		|	DO '{' stmt_list '}' WHILE expr ';'			{
															tree* while_body = createnode("while_body",0,0,NULL,0,NULL,$3 , NULL);
															tree* while_cond = createnode("while_condition",0,0,NULL , 0 , NULL , $6 , NULL);
															while_cond->sibling = while_body;
															tree* while_stmt = createnode("WHILE_STMT" , 0 , 0 , NULL , 0 , NULL , while_cond , NULL);
															$$ = createnode("COND_STMT" , 0 , 0 , NULL , 0 , NULL , while_stmt , NULL);
														}
		;
	
	func_stmt:	func_call 		{ 						}
		;
		
	func_call:	VAR '(' param_list ')'	{ 						   }
		;
		
	param_list:				
		|	param_list1		
		;
		
	param_list1:	para			
		|	para ',' param_list1	
		;

	para	:	expr			{ 						}
		;

	expr	:	NUM 			{ 
									valunion value;
									value.num = $1;
									$$ = createnode(NULL , 1  , 0 , &value , INTEGER , "int" , NULL,NULL);

								}
		|	'-' NUM				{
									valunion value;
									value.num = -$2;
									$$ = createnode(NULL , 1  , 0 , &value , INTEGER , "int" , NULL,NULL);
								}
		|	var_expr			{ 	
									valunion value;
									tree* var_expr = createnode($1->name , 1 , $1->idx , &value , $1->vartype , NULL , $1->child , $1->sibling);
									if ($1->var_idx){
										var_expr->var_idx = strdup($1->var_idx);
									}
									$$ = var_expr;
								}
		|	T					{
									valunion value;
									value.bnum = 1;
									$$ = createnode(NULL , 1  , 0 , &value , BOOLEAN , "bool" , NULL,NULL); 
								}
		|	F					{ 
									valunion value;
									value.bnum = 0;
									$$ = createnode(NULL , 1  , 0 , &value , BOOLEAN , "bool" , NULL,NULL);
								}
		|	'(' expr ')'		{ $$ = $2;}
		|	expr '+' expr 		{ 
									valunion value;
									tree* root = createnode("PLUS" , 0 , 0 , &value , INTEGER , "int" ,$1 , NULL);
									$1->sibling = $3;
									$$ = root;
								}
		|	expr '-' expr	 	{ 
									valunion value;
									tree* root = createnode("MINUS" , 0 , 0 , &value , INTEGER , "int" ,$1, NULL);
									$1->sibling = $3;
									$$ = root;
								}
		|	expr '*' expr 		{
									valunion value;
									tree* root = createnode("MULT" , 0 , 0 , &value , INTEGER , "int" ,$1,NULL);
									$1->sibling = $3;
									$$ = root;
								}
		|	expr '/' expr 		{ 
									valunion value;
									tree* root = createnode("DIVIDE" , 0 , 0 , &value , INTEGER , "int" ,$1 , NULL);
									$1->sibling = $3;
									$$ = root;
								}
		|	expr '%' expr 		{
									valunion value;
									tree* root = createnode("MODULO" , 0 , 0 , &value , INTEGER , "int" ,$1,NULL);
									$1->sibling = $3;
									$$ = root;
		 						}
		|	expr '<' expr		{
									valunion value;
									tree* root = createnode("LESSTHAN" , 0 , 0 , &value , BOOLEAN, "bool", $1 , NULL);
									$1->sibling = $3;
									$$ = root;
								}
		|	expr '>' expr		{ 
									valunion value;
									tree* root = createnode("GREATERTHAN" , 0 , 0 , &value , BOOLEAN, "bool", $1 , NULL);
									$1->sibling = $3;
									$$ = root;
								}
		|	expr GREATERTHANOREQUAL expr	{
												valunion value;
												tree* root = createnode("GREATERTHANOREQUAL" , 0 , 0 , &value , BOOLEAN, "bool", $1 , NULL);
												$1->sibling = $3;
												$$ = root;
					 						}	
 		|	expr LESSTHANOREQUAL expr	{  		
											valunion value;
											tree* root = createnode("LESSTHANOREQUAL" , 0 , 0 , &value , BOOLEAN, "bool", $1 , NULL);
											$1->sibling = $3;
											$$ = root;
										}
		|	expr NOTEQUAL expr			{ 		
											valunion value;
											tree* root = createnode("NOTEQUAL" , 0 , 0 , &value , BOOLEAN, "bool", $1 , NULL);
											$1->sibling = $3;
											$$ = root;
										}
		|	expr EQUALEQUAL expr	{ 			
										valunion value;
										tree* root = createnode("EQUALEQUAL" , 0 , 0 , &value , BOOLEAN, "bool", $1 , NULL);
										$1->sibling = $3;
										$$ = root;
									}
		|	LOGICAL_NOT expr	{ 
									valunion value;
									tree* root = createnode("LOGICAL_NOT" , 0 , 0 , &value , BOOLEAN, "bool", $2 , NULL);
									$$ = root;
								}
		|	expr LOGICAL_AND expr	{ 				
										valunion value;
										tree* root = createnode("LOGICAL_AND" , 0 , 0 , &value , BOOLEAN, "bool", $1 , NULL);
										$1->sibling = $3;
										$$ = root;
									}
		|	expr LOGICAL_OR expr	{ 		
										valunion value;
										tree* root = createnode("LOGICAL_OR" , 0 , 0 , &value , BOOLEAN, "bool", $1 , NULL);
										$1->sibling = $3;
										$$ = root;
									}
		|	func_call		{  }

		;
	str_expr :  VAR                       {$$ = $1;}
                  | str_expr VAR   		{$$ = strcat($1,$2);}
                ;
	
	var_expr:	VAR	{	
						$$ = createnode($1, 1 , 0 , NULL , 0 , NULL , NULL , NULL);
					}
		|	var_expr '[' expr ']'	{	
										if ($3->name){
											tree* temp = createnode($1->name , 1 , 0 , NULL , 0 , "array" , NULL , NULL);
											temp->var_idx = strdup($3->name);
											$$ = temp;
										}
										else{
											$$ =  createnode($1->name , 1 , $3->value.num , NULL , 0 , "array" , NULL , NULL);
										}
									}

		;
%%
void yyerror ( char  *s) {
   fprintf (stderr, "%s\n", s);
}

int main(){
	init();
	yyparse();
	print_hash_table();
	return 0;
}
