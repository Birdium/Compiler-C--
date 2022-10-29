#include <assert.h>
#include <string.h>
#include "table.h"

HashValue table[HASH_SIZE];
Region cur;
int depth;

unsigned int hash_pjw(char *name) {
    unsigned int val = 0, i;
    for (; *name; ++name) {
        val = (val << 2) + *name;
        if (i = val & MASK) val = (val ^ (i >> 12)) & MASK;
    }
    return val % HASH_SIZE;
};

int table_insert(char *key, Type value) {
    unsigned int hash = hash_pjw(key);
    HashValue val = table[hash];
    while (val) {
        if (val->type->kind != FUNCTION && streq(key, val->name)) break;
        val = val->next;
    }
    if (val && val->depth == depth) return 1;
    while (val) {
        if (val->type->kind == STRUCTURE && streq(key, val->name)) return 1;
        val = val->next;
    }
    HashValue new_field = (HashValue)malloc(sizeof(struct HashValue_));
    new_field->type = value;
    new_field->name = key;
    new_field->next = table[hash];
    new_field->depth = depth;
    table[hash] = new_field;
    new_field->succ = cur->succ;
    cur->succ = new_field;
    return 0;
}

int function_insert(char *key, Type value, bool is_def) {
    unsigned int hash = hash_pjw(key);
    HashValue val = table[hash];
    while (val) {
        if (val->type->kind == FUNCTION && val->name && streq(key, val->name)) break;
        val = val->next;
    }
    if (val) {
        if (!type_match(value, val->type)) return 1;
        if (val->is_def) {
            if (is_def) return 1;
        }
        else{
            if (is_def) val->is_def = true;
        }
    }
    HashValue new_field = (HashValue)malloc(sizeof(struct HashValue_));
    new_field->type = value;
    new_field->name = key;
    new_field->next = table[hash];
    new_field->depth = depth;
    new_field->is_def = is_def;
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
    depth++;
    Region new_region = (Region)malloc(sizeof(struct Region_));
    new_region->pred = cur;
    new_region->succ = NULL;
    cur = new_region;
}

void table_leave() {
    depth--;
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
    // printf("name: %s, type: %p\n", val->name, val->type);
    while (val) {
        if (val->type->kind != FUNCTION && streq(key, val->name)) return val->type;
        val = val->next;
    }
    return NULL;
}

Type function_lookup(char *key) {
    unsigned int hash = hash_pjw(key);
    HashValue val = table[hash];
    while (val) {
        if (val->type->kind == FUNCTION && streq(key, val->name)) return val->type;
        val = val->next;
    }
    return NULL;
}
