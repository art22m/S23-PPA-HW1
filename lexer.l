%option noyywrap

%{
    #include "parser.tab.h"
%}

%%


\n { return EOL; }
";" { return SEMICOLON; }
"," { return COMMA; }
":=" { return ASSIGNMENT; }
"(" { return L_PARENTHESIS; }
")" { return R_PARENTHESIS; }
"sum" { yylval.sval = "sum"; return FUN; }
"random_choice" { yylval.sval = "random_choice"; return FUN; }
"-" { return MINUS; }
[0-9]+ { yylval.uival = atoi(yytext); return NUM; }
[A-Za-z][A-Za-z0-9]* { yylval.sval = strdup(yytext); return IDENTIFIER; }
[ \t] { }

%%
