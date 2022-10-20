#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "ast.h"
#include "syntax.tab.h"
extern int yylineno;

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

bool is_terminator(NodeType type) {
    if (type <= TERM_NODE) return true;
    else return false;
}

Node *newNode(NodeType type, ...) {
    Node *node = malloc(sizeof(Node));
    // printf("%p: %s\n", node, NODE_TYPE_STRING[type]);
    node->type = type;
    node->son = NULL;
    va_list list;
    va_start(list, type);
    node->lineno = yylineno;
    switch(type) {
        case DEC_NODE: node->val.ival = atoi(va_arg(list, char*)); node->type = INT_NODE; break;
        case HEX_NODE: node->val.ival = strtol(va_arg(list, char*), NULL, 16); node->type = INT_NODE; break;
        case OCT_NODE: node->val.ival = strtol(va_arg(list, char*), NULL, 8); node->type = INT_NODE; break;
        case FLOAT_NODE: node->val.fval = atof(va_arg(list, char*)); break;
        case ID_NODE: strcpy(node->val.id, va_arg(list, char*)); break;
        case TYPE_NODE: {
            // if(strcmp(va_arg(list, char*), "int")) node->type = INT_TY_NODE;
            // else node->type = FLOAT_TY_NODE;
            // for lab1
            strcpy(node->val.id, va_arg(list, char*)); break;
        }
        break;
        case RELOP_NODE: {
            // if(strcmp(va_arg(list, char*), ">")) node->type = GRT_NODE;
            // else if(strcmp(va_arg(list, char*), "<")) node->type = LSS_NODE;
            // else if(strcmp(va_arg(list, char*), ">=")) node->type = GEQ_NODE;
            // else if(strcmp(va_arg(list, char*), "<=")) node->type = LEQ_NODE;
            // else if(strcmp(va_arg(list, char*), "==")) node->type = EQ_NODE;
            // else if(strcmp(va_arg(list, char*), "!=")) node->type = NEQ_NODE;
            // for lab1
            strcpy(node->val.id, va_arg(list, char*)); break;
        }
        break;
        case SEMI_NODE: case COMMA_NODE: case ASSIGNOP_NODE: case PLUS_NODE: case MINUS_NODE: case STAR_NODE: case DIV_NODE:
        case AND_NODE: case OR_NODE: case NOT_NODE: case DOT_NODE: case STRUCT_NODE: case RETURN_NODE:
        case IF_NODE: case ELSE_NODE: case WHILE_NODE: case LP_NODE: case RP_NODE: 
        case LB_NODE: case RB_NODE: case LC_NODE: case RC_NODE: {
            
        } // Terminator
        break;
        default: {
            node->son = va_arg(list, Node*);
            Node *node_s = node->son; 
            while (node_s) {
                node->lineno = MIN(node->lineno, node_s->lineno);
                node_s->succ = va_arg(list, Node*);
                node_s = node_s->succ; 
            }
        } // Non-Terminator

        break;
    }
    va_end(list);
    return node;
}

void print_ast(Node *cur, int dep) {
    if (cur->type == Null_NODE) return;
    for (int i = 0; i < dep; i++) {
        printf("  ");
    }
    switch (cur->type) {
        case TYPE_NODE: case ID_NODE: 
            printf("%s: %s\n", NODE_TYPE_STRING[cur->type], cur->val.id);
            break;
        case INT_NODE:
            printf("INT: %u\n", cur->val.ival);
            break;
        case FLOAT_NODE:
            printf("FLOAT: %f\n", cur->val.fval);
            break;
        default:
            if (is_terminator(cur->type)) {
                printf("%s\n", NODE_TYPE_STRING[cur->type]);
            }
            else {
                printf("%s (%d)\n", NODE_TYPE_STRING[cur->type], cur->lineno);
            }
            break;
    }
    Node *nxt = cur->son;
    while (nxt) {
        print_ast(nxt, dep+1);
        nxt = nxt->succ;
    }
}
