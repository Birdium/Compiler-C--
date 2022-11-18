#ifndef TABLE_H
#define TABLE_H

#include "semantic.h"
#include "ir.h"

#define HASH_SIZE 0x4000
#define MASK (~(HASH_SIZE-1))

typedef struct HashValue_* HashValue;
typedef struct Region_* Region;
typedef struct FuncNameNode_* FuncNameNode;
struct HashValue_{
    char *name;
    Type type;
    Operand op;
    int depth;
    bool is_def;
    HashValue next, succ; // next 指向下个hash值, succ 指向同一层的下个值
};

struct Region_{
    Region pred;
    HashValue succ;
};

struct FuncNameNode_ {
    char *name;
    FuncNameNode next;
    bool is_def;
    int lineno;
};


unsigned int hash_pjw(char *name);
bool check_funclist();
void funclist_insert(char *key, bool is_def, int lineno);
int table_insert(char *key, Type value);
int table_update(char *key, Operand op);
int table_remove(char *key);
void table_enter();
void table_leave();
Type table_lookup(char *key);
Operand table_getop(char *key);
int function_insert(char *key, Type value, bool is_def, int lineno);
Type function_lookup(char *key);

HashValue newHashValue(char *name, Type type, int depth, bool is_def, HashValue next, HashValue succ);

#endif