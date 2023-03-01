#include <stdlib.h>
#include <stdio.h>
#include "ast.h"

ast_node_children* _make_children(int size) {
    ast_node_children* children = (ast_node_children*) malloc(sizeof(ast_node_children));
    children->size = size;
    ast_node** node_pointers = (ast_node**) malloc(size * sizeof(ast_node*));
    children->nodes = node_pointers;
    return children;
}

int* _make_int_in_heap(int num) {
    int *content = (int*) malloc(sizeof(int));
    *content = num;
    return content;
}

ast_node* _make_node() {
    ast_node* node = (ast_node*) malloc(sizeof(ast_node));
    return node;
}

ast_node* make_int_node(int num) {
    ast_node* node = _make_node();
    node->type = TYPE_INT;
    node->children = _make_children(0);
    node->content = _make_int_in_heap(num);
    return node;
}

ast_node* make_identifier_node(char* identifier) {
    ast_node* node = _make_node();
    node->type = TYPE_IDENTIFIER;
    node->children = _make_children(0);
    node->content = identifier;
    
    return node;
}

ast_node* make_function_call_node(char* function_name, ast_node *node) {
    ast_node* res = _make_node();
    res->type = TYPE_FUNCTION_CALL;
    res->content = function_name;
    
    ast_node_children* children = _make_children(1);
    children->nodes[0] = node;
    
    res->children = children;
    return res;
}

ast_node* make_line_node(unsigned int line_number, ast_node *node) {
    ast_node* res = _make_node();
    res->type = TYPE_LINE;
    res->content = _make_int_in_heap(line_number);

    ast_node_children* children = _make_children(1);
    children->nodes[0] = node;
    
    res->children = children;
    return res;
}

ast_node* make_assignment_node(ast_node *a, ast_node *b) {
    ast_node* res = _make_node();
    res->type = TYPE_ASSIGNMENT;

    ast_node_children* children = _make_children(2);
    children->nodes[0] = a;
    children->nodes[1] = b;

    res->children = children;
    return res;
}

ast_node* make_expr_list_node(ast_node *a, ast_node *b) {
    ast_node* res = _make_node();
    res->type = TYPE_EXPR_LIST;
    res->content = NULL;
    if (b == NULL) {
        ast_node_children* children = _make_children(1);
        children->nodes[0] = a;
        res->children = children;
    } else {
        int size = b->children->size + 1;
        ast_node_children* children = _make_children(size);
        children->nodes[0] = a;
        for (int i = 1; i < size; i++) {
            children->nodes[i] = b->children->nodes[i - 1];
        }
        res->children = children;
    }
    return res;
}

ast_node* program;

ast_node* make_program_node(ast_node *a, ast_node *b) {
    ast_node* res = _make_node();
    res->type = TYPE_PROGRAM;
    res->content = NULL;
    if (b == NULL) {
        ast_node_children* children = _make_children(1);
        children->nodes[0] = a;
        res->children = children;
    } else {
        int size = b->children->size + 1;
        ast_node_children* children = _make_children(size);
        children->nodes[0] = a;
        for (int i = 1; i < size; i++) {
            children->nodes[i] = b->children->nodes[i - 1];
        }
        res->children = children;
    }
    program = res;
    return res;
}

void _print_node(ast_node *node, int level, FILE * f) {
    for (int i = 0; i < level; i++) {
        fprintf(f, "- ");
    }

    if (node->type == TYPE_PROGRAM) {
        fprintf(f, "PROGRAM\n");
        for (int i = 0; i < node->children->size; i++) {
            _print_node(node->children->nodes[i], level + 1, f);
        }
    } else if (node->type == TYPE_LINE) {
        fprintf(f, "LINE %d\n", *((int*)node->content));
        for (int i = 0; i < node->children->size; i++) {
            _print_node(node->children->nodes[i], level + 1, f);
        }
    } else if (node->type == TYPE_ASSIGNMENT) {
        fprintf(f, "ASSIGNMENT\n");
        for (int i = 0; i < node->children->size; i++) {
            _print_node(node->children->nodes[i], level + 1, f);
        }
    } else if (node->type == TYPE_IDENTIFIER) {
        fprintf(f, "IDENTIFIER \"%s\"\n", (char*)node->content);
    } else if (node->type == TYPE_INT) {
        fprintf(f, "INTEGER %d\n", *((int*)node->content));
    } else if (node->type == TYPE_FUNCTION_CALL) {
        fprintf(f, "FUNCTION CALL \"%s\"\n", (char*)node->content);
        for (int i = 0; i < node->children->size; i++) {
            _print_node(node->children->nodes[i], level + 1, f);
        }
    } else if (node->type == TYPE_EXPR_LIST) {
        fprintf(f, "EXPRESSIONS\n");
        for (int i = 0; i < node->children->size; i++) {
            _print_node(node->children->nodes[i], level + 1, f);
        }
    } else {
        // fprintf(f, "+");
    }
}

void print_ast() {
    FILE *fptr;
    fptr = fopen("ast.txt", "w");
    if(fptr == NULL)
    {
        printf("can not open file \"ast.txt\"");   
        exit(1);             
    }
    _print_node(program, 0, fptr);
    fclose(fptr);
}
