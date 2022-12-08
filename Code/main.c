#include <stdio.h>
#include <assert.h>
#include "ast.h"
extern Node *root;
extern bool lex_error, syn_error;
// extern int yydebug;
int main(int argc, char** argv) {
    if (argc <= 1) return 1;
    FILE *fin = fopen(argv[1], "r"); 
    if (!fin) {
        perror(argv[1]);
        return 1;
    }
    FILE *fout = NULL;
    if (argc >= 2)
        fout = freopen(argv[2], "w", stdout);

    // yydebug = 1;
    yyrestart(fin);
    yyparse();
    // assert(root);
    // if (!lex_error && !syn_error) print_ast(root, 0);
    Program(root);
    if (fout) fclose(fout);
    return 0;
}