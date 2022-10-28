#ifndef TABLE_H
#define TABLE_H

#include "semantic.h"

#define HASH_SIZE 16384
#define MASK (~(HASH_SIZE-1))

typedef struct HashValue_* HashValue;
typedef struct Region_* Region;

struct HashValue_{
    char *name;
    Type type;
    int depth;
    HashValue next, succ; // next 指向下个hash值, succ 指向同一层的下个值
};

struct Region_{
    Region pred;
    HashValue succ;
};


unsigned int hash_pjw(char *name) {
    unsigned int val = 0, i;
    for (; *name; ++name) {
        val = (val << 2) + *name;
        if (i = val & MASK) val = (val ^ (i >> 12)) & MASK;
    }
    return val;
}

int table_insert(char *key, Type value);
int table_remove(char *key);
void table_enter();
void table_leave();
Type table_lookup(char *key);
Type function_lookup(char *key);

#endif