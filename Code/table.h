#ifndef TABLE_H
#define TABLE_H

#include "semantic.h"

#define HASH_SIZE 0x4000
#define MASK (~(HASH_SIZE-1))

typedef struct HashValue_* HashValue;
typedef struct Region_* Region;

struct HashValue_{
    char *name;
    Type type;
    int depth;
    bool is_def;
    HashValue next, succ; // next 指向下个hash值, succ 指向同一层的下个值
};

struct Region_{
    Region pred;
    HashValue succ;
};


unsigned int hash_pjw(char *name);

int table_insert(char *key, Type value);
int table_remove(char *key);
void table_enter();
void table_leave();
Type table_lookup(char *key);
int function_insert(char *key, Type value, bool is_def);
Type function_lookup(char *key);

#endif