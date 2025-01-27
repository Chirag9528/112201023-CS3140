%{
#include "y.tab.h"
void yyerror(char* s);
%}
%%
"exit"                      {return exit_command;}
[a-zA-Z]                    {yylval.id = yytext[0]; return identifier;}
[0-9]+                      {yylval.num = atoi(yytext); return number;}
[ \t\n]                     ;
[-+/*=;]                      {return yytext[0];}
.                           {yyerror ("unexpected character");}
%%

int yywrap (void) {return 1;}