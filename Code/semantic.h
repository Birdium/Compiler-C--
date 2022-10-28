#ifndef SEMANTIC_H
#define SEMANTIC_H

#define streq(a, b) (strcmp((a), (b)) == 0)
typedef Type_* Type;
typedef FieldList_* FieldList;

typedef struct Type_{
	enum { BASIC, ARRAY, STRUCTURE, FUNCTION } kind;
	union{
		int basic;
		struct { Type elem; int size; } array;
		
		FieldList structure;	
	} u;
}Type_;

typedef struct FieldList_{
	char *name;
	Type type;
	FieldList tail;
}FieldList_;

bool type_match(Type ty1, Type ty2);

Type get_field(Type type, char *id) {
    FieldList iter = type->structure;
    while (iter) {
        if (streq(id, iter->name)) return iter->type;

    }
    return NULL;
}

#endif