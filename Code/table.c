#include "table.h"

HashValue table[HASH_SIZE];
Region cur;
int depth;
HashValue func_table[HASH_SIZE];

int table_insert(char *key, Type value) {
    Type check = table_lookup(key);
    if (check && check->depth == depth) return 1;
    unsigned int hash = hash_pjw(key);
    HashValue new_field = (HashValue)malloc(sizeof(HashValue_));
    new_field->next = table[hash];
    new_field->depth = depth;
    table[hash] = new_field;
    new_field->succ = cur->succ;
    cur->succ = new_field;
    return 0;
}

int table_remove(char *key) {
    unsigned int hash = hash_pjw(key);
    HashValue val = table[hash];
    if (!val) return 1;
    table[hash] = val->next;
    free(val);
}

void table_enter() {
    Region new_region = (Region)malloc(sizeof(Region_));
    new_region->pred = cur;
    new_region->succ = NULL;
    cur = new_region;
}

void table_leave() {
    assert(cur);
    Region pred = cur->pred;
    HashValue val = cur->succ;
    while (val) {
        HashValue tmp = val;
        val = val->succ;
        table_remove(tmp->name);
    }
    free(cur);
    cur = pred;
}

Type table_lookup(char *key) {
    unsigned int hash = hash_pjw(key);
    HashValue val = table[hash];
    while (val) {
        if (val->kind != FUNCTION && streq(key, val->name)) break;
        val = val->next;
    }
    return val;
}

Type function_lookup(char *key) {
    unsigned int hash = hash_pjw(key);
    HashValue val = table[hash];
    while (val) {
        if (val->kind == FUNCTION && streq(key, val->name)) break;
        val = val->next;
    }
    return val;
}



