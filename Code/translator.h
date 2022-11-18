#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <stdbool.h>
#include "ast.h"
#include "table.h"
#include "ir.h"

void init_Program();
void Program(Node *cur);
void translate_ExtDefList(Node *cur);
void translate_ExtDef(Node *cur);

Operand translate_VarDec(Node *cur);
Function translate_FunDec(Node *cur);
OpList translate_VarList(Node *cur);
OpList translate_ParamDec(Node *cur);

void translate_CompSt(Node *cur);
void translate_StmtList(Node *cur);
void translate_Stmt(Node *cur);

void translate_DefList(Node *cur);
void translate_Def(Node *cur);
void translate_DecList(Node *cur);
void translate_Dec(Node *cur);

Operand translate_Exp(Node *cur);
OpList translate_Args(Node *cur);

#endif