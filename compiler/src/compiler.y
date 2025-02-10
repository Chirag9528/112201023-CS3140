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
int insert_string(char* , int , int);
int update_string(char* , int);
void print_hash_table(void);

void print_info(tree*);
void print_decl_info(tree*);
tree* createnode(char* , int , tree*, tree*);
int i;	
%}

%union {
	char* name;
	int num;
	struct __tree* treenode;
}


%token BEG END
%token T_INT
/* %token T_BOOL */
%token READ WRITE
%token DECL ENDDECL
%token <name> VAR
%token <num> NUM
%token IF THEN ELSE ENDIF
%token LOGICAL_AND LOGICAL_NOT LOGICAL_OR
%token EQUALEQUAL LESSTHANOREQUAL GREATERTHANOREQUAL NOTEQUAL
%token FOR 
/* %token WHILE DO ENDWHILE */
%token T F 
%token MAIN RETURN

%left '<' '>'
%left EQUALEQUAL LESSTHANOREQUAL GREATERTHANOREQUAL NOTEQUAL
%left '+' '-'
%left '*' '/'
%left '%'
%left LOGICAL_AND LOGICAL_OR
%left LOGICAL_NOT

%type <treenode> expr var_expr assign_stmt Gid Glist ret_type Gdecl statement write_stmt
%type <name> str_expr


%%

	Prog	:	Gdecl_sec Fdef_sec MainBlock
		| Gdecl_sec stmt_list
		;
		
	Gdecl_sec:	DECL Gdecl_list ENDDECL{}
		;
		
	Gdecl_list: 
		| 	Gdecl Gdecl_list
		;
		
	Gdecl 	:	ret_type Glist ';'			{
												tree* root = createnode("DECL_STMT",0,$1,$2);
												$$ = root;
												print_decl_info(root);
											}
		;
		
	ret_type:	T_INT		{ 
								$$ = createnode("integer",0,NULL,NULL);
							}
		;
		
	Glist 	:	Gid				{$$ = $1;}
		| 	func 				{}
		|	Gid ',' Glist 		{
									$1->right = $3;
									$$ = $1;
								}
		|	func ',' Glist		{}
		;
	
	Gid	:	VAR		{
						insert_string($1 , 0 , 0);
						$$ = createnode($1,0,NULL,NULL);
					}
		|	Gid '[' NUM ']'	{                                                   }

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

	stmt_list:	/* NULL */		{  }
		|	statement stmt_list	{						}
		|	error ';' 		{  }
		;

	statement:	assign_stmt  ';'		{$$ = $1; print_info($1);}
		|	read_stmt ';'				{ }
		|	write_stmt ';'				{$$ = $1; print_info($1);}
		|	cond_stmt 		{ }
		|	func_stmt ';'		{ }
		;

	read_stmt:	READ '(' var_expr ')' {						 }
		;

	write_stmt:	WRITE '(' expr ')' 	{
										tree* leftchild = createnode("WRITE",0,NULL,NULL);
										tree* root = createnode("CALL",0,leftchild,$3);
										$$ = root;
									}
		 | WRITE '(''"' str_expr '"'')'     {
												tree* leftchild = createnode("WRITE",0 , NULL,NULL);
												tree* rightchild = createnode($4,0,NULL,NULL);
												tree* root = createnode("CALL",0,leftchild,rightchild);
												$$ = root;
		 									}

		;
	
	assign_stmt:	var_expr '=' expr 	{
											update_string($1->name , $3->value);
											$1->value = $3->value;
											$$ = createnode("ASS_STMT",$3->value,$1,$3);
										}
		;

	cond_stmt:	IF expr THEN stmt_list ENDIF 	{ 						}
		|	IF expr THEN stmt_list ELSE stmt_list ENDIF 	{ 						}
	        |    FOR '(' assign_stmt  ';'  expr ';'  assign_stmt ')' '{' stmt_list '}'                                             {                                                 }
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
									$$ = createnode(NULL,$1,NULL,NULL);

								}
		|	'-' NUM				{
									$$ = createnode(NULL,-$2,NULL,NULL);
								}
		|	var_expr			{ $$ = $1;}
		|	T					{ }
		|	F					{ }
		|	'(' expr ')'		{ $$ = $2;}
		|	expr '+' expr 		{ 
									$$ = createnode("+",$1->value+$3->value,$1,$3);
								}
		|	expr '-' expr	 	{ 
									$$ = createnode("-",$1->value-$3->value,$1,$3);
								}
		|	expr '*' expr 		{
									$$ = createnode("*",$1->value*$3->value,$1,$3);
								}
		|	expr '/' expr 		{ 
									$$ = createnode("/",$1->value/$3->value,$1,$3);
								}
		|	expr '%' expr 		{ }
		|	expr '<' expr		{ }
		|	expr '>' expr		{ }
		|	expr GREATERTHANOREQUAL expr				{			 }
 		|	expr LESSTHANOREQUAL expr	{  						}
		|	expr NOTEQUAL expr			{ 						}
		|	expr EQUALEQUAL expr	{ 						}
		|	LOGICAL_NOT expr	{ 						}
		|	expr LOGICAL_AND expr	{ 						}
		|	expr LOGICAL_OR expr	{ 						}
		|	func_call		{  }

		;
	str_expr :  VAR                       {$$ = $1;}
                  | str_expr VAR   		{$$ = strcat($1,$2);}
                ;
	
	var_expr:	VAR	{	
						$$ = createnode($1,search_string($1),NULL,NULL);
					}
		|	var_expr '[' expr ']'	{                                                 }
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
