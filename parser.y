%define parse.error detailed


%union  {
    unsigned int uival;
    char *sval;
    void *node;
}

%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "ast.h"
    int yylex ();
    void yyerror ( char const *msg) {
        printf("ERR: %s\n", msg);
        exit(1);
    }
%}

%token 
EOL 
SEMICOLON
COMMA
ASSIGNMENT
L_PARENTHESIS
R_PARENTHESIS
MINUS

%token<uival> NUM
%token<sval> FUN
%token<sval> IDENTIFIER

%type<node> 
input
line
command
var
expr
exprs
integer

%type<uival> number
%type<sval> fun

%%

input: 
    line SEMICOLON EOL input { $$ = make_program_node($1, $4); }
|   line { $$ = make_program_node($1, NULL); }

line: 
    number command { $$ = make_line_node($1, $2); }

number:
    NUM { $$ = $1; }

command:
    var ASSIGNMENT expr { $$ = make_assignment_node($1, $3); }

var:
    IDENTIFIER { $$ = make_identifier_node($1); }

expr:
    var { $$ = $1; }
|   integer { $$ = $1; }
|   fun L_PARENTHESIS exprs R_PARENTHESIS { $$ = make_function_call_node($1, $3); }

exprs:
    expr COMMA exprs { $$ = make_expr_list_node($1, $3); }
|   expr { $$ = make_expr_list_node($1, NULL); }

fun:
    FUN { $$ = $1; }

integer:
    NUM { $$ = make_int_node($1); }
|   MINUS NUM { $$ = make_int_node(-$2); }

%%


int main() {
    printf("\n***start***\n");

    yyparse();

    print_ast();

    return 0;
}
