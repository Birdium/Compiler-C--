#ifndef AST_H
#define AST_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define Assert(cond, format, ...) \
  do { \
    if (!(cond)) { \
        (fprintf(stderr, ASNI_FMT(format, ASNI_FG_RED) "\n", ##  __VA_ARGS__)); \
      extern void assert_fail_msg(); \
      assert_fail_msg(); \
      assert(cond); \
    } \
  } while (0)

#define panic(format, ...) Assert(0, format, ## __VA_ARGS__)

#define FOREACH_NODE_TYPE(NODE_TYPE) \
/*      --- Terminator --- */   \
        NODE_TYPE(Null)   \
        NODE_TYPE(INT)   \
        NODE_TYPE(DEC)   \
        NODE_TYPE(HEX)   \
        NODE_TYPE(OCT)   \
        NODE_TYPE(FLOAT)   \
        NODE_TYPE(ID)   \
        NODE_TYPE(TYPE)   \
        NODE_TYPE(INT_TY)   \
        NODE_TYPE(FLOAT_TY)   \
        NODE_TYPE(SEMI)   \
        NODE_TYPE(COMMA)   \
        NODE_TYPE(ASSIGNOP)   \
        NODE_TYPE(RELOP)   \
        NODE_TYPE(GRT)   \
        NODE_TYPE(LSS)   \
        NODE_TYPE(GEQ)   \
        NODE_TYPE(LEQ)   \
        NODE_TYPE(EQ)   \
        NODE_TYPE(NEQ)   \
        NODE_TYPE(PLUS)   \
        NODE_TYPE(MINUS)   \
        NODE_TYPE(STAR)   \
        NODE_TYPE(DIV)   \
        NODE_TYPE(AND)   \
        NODE_TYPE(OR)   \
        NODE_TYPE(DOT)   \
        NODE_TYPE(NOT)   \
        NODE_TYPE(STRUCT)   \
        NODE_TYPE(RETURN)   \
        NODE_TYPE(IF)   \
        NODE_TYPE(ELSE)   \
        NODE_TYPE(WHILE)   \
        NODE_TYPE(LP)   \
        NODE_TYPE(RP)   \
        NODE_TYPE(LB)   \
        NODE_TYPE(RB)   \
        NODE_TYPE(LC)   \
        NODE_TYPE(RC)   \
        NODE_TYPE(TERM)   \
/*      --- Non-Terminator --- */   \
        NODE_TYPE(NTERM)   \
        NODE_TYPE(Program)   \
        NODE_TYPE(ExtDefList)   \
        NODE_TYPE(ExtDef)   \
        NODE_TYPE(ExtDecList)   \
        NODE_TYPE(Specifier)   \
        NODE_TYPE(StructSpecifier)   \
        NODE_TYPE(OptTag)   \
        NODE_TYPE(Tag)   \
        NODE_TYPE(VarDec)   \
        NODE_TYPE(FunDec)   \
        NODE_TYPE(VarList)   \
        NODE_TYPE(ParamDec)   \
        NODE_TYPE(CompSt)   \
        NODE_TYPE(StmtList)   \
        NODE_TYPE(Stmt)   \
        NODE_TYPE(DefList)   \
        NODE_TYPE(Def)   \
        NODE_TYPE(DecList)   \
        NODE_TYPE(Dec)   \
        NODE_TYPE(Exp)   \
        NODE_TYPE(Args)

#define GENERATE_ENUM(ENUM) ENUM##_NODE,
#define GENERATE_STRING(STRING) #STRING,

typedef enum NoneType{
    FOREACH_NODE_TYPE(GENERATE_ENUM)
} NodeType;

static const char *NODE_TYPE_STRING[] = {
    FOREACH_NODE_TYPE(GENERATE_STRING)
};

typedef struct{
    #define MAX_IDENT_LENGTH 32
    struct Node *son, *succ;
    NodeType type;

    union {
        char id[MAX_IDENT_LENGTH];
        int ival;
        float fval;
    } val;
    int lineno;
}Node;

bool is_terminator(NodeType type);

Node *newNode(NodeType type, ...);

void print_ast(Node*, int);

#endif