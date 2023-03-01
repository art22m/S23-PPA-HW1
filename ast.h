enum ast_node_type {
    TYPE_PROGRAM,
    TYPE_LINE,
    TYPE_ASSIGNMENT,
    TYPE_IDENTIFIER,
    TYPE_FUNCTION_CALL,
    TYPE_EXPR_LIST,
    TYPE_INT
};

struct ast_node_children;

typedef struct ast_node {
    int type;
    struct ast_node_children *children;
    void *content;
} ast_node;

typedef struct ast_node_children {
    int size;
    ast_node **nodes;
} ast_node_children;

ast_node* make_int_node(int num);
ast_node* make_identifier_node(char *identifier);
ast_node* make_line_node(unsigned int line_number, ast_node *node);
ast_node* make_function_call_node(char* function_name, ast_node *node);
ast_node* make_assignment_node(ast_node *a, ast_node *b);
ast_node* make_expr_list_node(ast_node *a, ast_node *b);
ast_node* make_program_node(ast_node *a, ast_node *b);
void print_ast();
