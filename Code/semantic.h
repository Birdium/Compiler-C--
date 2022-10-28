#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <stdbool.h>
#include "ast.h"

#define streq(a, b) (strcmp((a), (b)) == 0)

typedef struct Type_* Type;
typedef struct FieldList_* FieldList;
struct Type_{
	enum { BASIC, ARRAY, STRUCTURE, FUNCTION } kind;
	union{
		int basic;
		struct { Type elem; int size; } array;
		
		FieldList structure;	
	} u;
};

struct FieldList_{
	char *name;
	Type type;
	FieldList tail;
};


bool array_match(Type ty1, Type ty2);
bool field_match(FieldList , FieldList);
bool type_match(Type ty1, Type ty2);

Type return_type(Type functy);
FieldList param_type(Type functy);
Type get_field(Type type, char *id);

void Program(Node *);
void ExtDefList(Node *cur);
void ExtDef(Node *cur);
void ExtDecList(Node *cur, Type type);

Type Specifier(Node *cur);
Type StructSpecifier(Node *cur);

char *OptTag(Node *cur);
char *Tag(Node *cur);

FieldList VarDec(Node *cur, Type type);
FieldList FunDec(Node *cur, Type type);
FieldList VarList(Node *cur);
FieldList ParamDec(Node *cur);

void CompSt(Node *cur);
void StmtList(Node *cur);
void Stmt(Node *cur);

FieldList DefList(Node *cur);
FieldList Def(Node *cur);
FieldList DecList(Node *cur, Type type);
FieldList Dec(Node *cur, Type type);

Type Exp(Node *cur);
FieldList Args(Node *cur);

#endif