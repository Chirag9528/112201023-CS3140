%{
void yyerror (char *s);
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
int symbols[52];
int symbolVal(char symbol);
void updateSymbolVal(char symbol , int val);
extern int yylex();
%}

%union {int num; char id;}  /* Yacc definitions */
%start line
%token exit_command
%token <num> number
%token <id> identifier
%type <num> line exp term
%type <num> assignment
%left '+' '-'
%left '*' '/'

%%

line    : assignment ';'            {printf("%d\n", $1);}
        | exit_command ';'          {exit(EXIT_SUCCESS);}
        | line assignment ';'       {printf("%d\n",$2);}
        | line exit_command ';'     {exit(EXIT_SUCCESS);}
        ;

assignment : identifier '=' exp     {updateSymbolVal($1 , $3); $$ = $3;}
        ;

exp     : term                     {$$ = $1;}
        | exp '+' exp              {$$ = $1 + $3;}
        | exp '-' exp              {$$ = $1 - $3;}
        | exp '/' exp              {$$ = $1 / $3;}
        | exp '*' exp              {$$ = $1 * $3;}
        ;

term    : number                    {$$ = $1;}
        | identifier                {$$ = symbolVal($1);}
        ;

%%

int computeSymbolIndex(char token)
{
        int idx = -1;
        if (islower(token)){
                idx = token - 'a' + 26;
        } else if (isupper(token)) {
                idx = token - 'A';
        }
        return idx;
}

int symbolVal(char symbol)
{
        int bucket = computeSymbolIndex(symbol);
        return symbols[bucket];
}

void updateSymbolVal(char symbol , int val)
{
        int bucket = computeSymbolIndex(symbol);
        symbols[bucket] = val;
}

int main(void) {
        int i;
        for (i = 0 ; i<52 ; i++) {
                symbols[i] = 0;
        }

        yyparse();
        return 0;
}

void yyerror (char *s){fprintf(stderr , "%s\n" , s);}