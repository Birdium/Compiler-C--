#include <stdio.h>
#include <assert.h>
#include "ast.h"
extern Node *root;
extern bool lex_error, syn_error;
// extern int yydebug;
int main(int argc, char** argv) {
    if (argc <= 1) return 1;
    FILE *f = fopen(argv[1], "r"); 
    if (!f) {
        perror(argv[1]);
        return 1;
    }
    // yydebug = 1;
    yyrestart(f);
    yyparse();
    // assert(root);
    // if (!lex_error && !syn_error) print_ast(root, 0);
    Program(root);
    return 0;
}