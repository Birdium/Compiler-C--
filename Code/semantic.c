#include <stdio.h>
#include "ast.h"
#include "semantic.h"

static bool in_struct;

static inline bool is_intty(Type type) {
    return type->kind == BASIC && type->u.basic == INT_TY_NODE;
}
static inline bool is_floatty(Type type) {
    return type->kind == BASIC && type->u.basic == FLOAT_TY_NODE;
}
static inline bool is_basicty(Type type) {
    return type->kind == BASIC;
}

Type_ INT_TY_, FLOAT_TY_;
Type INT_TY, FLOAT_TY;

void serror(int type, int lineno, const char *str){
    printf("Error type %d at Line %d: %s.\n", type, lineno, str);
}

bool array_match(Type ty1, Type ty2) {
    return type_match(ty1->u.array.elem, ty2->u.array.elem);
}

bool field_match(FieldList iter1, FieldList iter2) {
    while (iter1 || iter2) {
        if (!iter1 || !iter2) return false;
        bool check = type_match(iter1->type, iter2->type);
        if (!check) return false;
        iter1 = iter1->tail;
        iter2 = iter2->tail;
    }
    return true;
}

bool type_match(Type ty1, Type ty2) {
    if (ty1 == NULL || ty2 == NULL) return false;
    if (ty1->kind != ty2->kind) return false;
    if (ty1->kind == BASIC) return ty1->u.basic == ty2->u.basic;
    if (ty1->kind == STRUCTURE) return field_match(ty1->u.structure, ty2->u.structure);
    if (ty1->kind == FUNCTION) return field_match(ty1->u.structure, ty2->u.structure);
    return false;
}

Type return_type(Type functy) {
    return functy->u.structure->type;
}

FieldList param_type(Type functy) {
    return functy->u.structure->tail;
}

void Program(Node *cur) {
    INT_TY_.kind = FLOAT_TY_.kind = BASIC;
    INT_TY_.u.basic = INT_TY_NODE;
    FLOAT_TY_.u.basic = FLOAT_TY_NODE;
    INT_TY = &FLOAT_TY_;
    FLOAT_TY = &FLOAT_TY_;
    table_enter();
    ExtDefList(cur->son);
    table_leave();
}

void ExtDefList(Node *cur) {
    if (cur->type == ExtDefList_NODE) {
        Node *ext_def = cur->son;
        ExtDef(ext_def);
        Node *ext_def_list = ext_def->succ;
        ExtDefList(ext_def_list);
    }
}

void ExtDef(Node *cur) {
    if (cur == NULL) return;
    if (cur->type == ExtDef_NODE) {
        Node *specifier = cur->son;
        Type type = Specifier(specifier);
        Node *node = specifier->succ;
        if (node->type == SEMI_NODE) {
            return;
        }
        else if (node->type == ExtDecList_NODE) {
            ExtDecList(node, type);
        }
        else if (node->type == FunDec_NODE) {
            type = FunDec(node, type);
            node = node->succ;
            CompSt(node);
        }
    }
}

void ExtDecList(Node *cur, Type type) {
    if (cur == NULL) return;
    Node *var_dec = cur->son;
    VarDec(var_dec, type);
    if (var_dec->succ) {
        var_dec = var_dec->succ;
        var_dec = var_dec->succ;
        ExtDecList(var_dec, type);
    }
}

Type Specifier(Node *cur) {
    if (cur == NULL) return NULL;
    Node *son = cur->son;
    Type type;
    if (son->type == TYPE_NODE) {
        type = (Type)malloc(sizeof(Type_));
        type->kind = BASIC;
        if (streq(son->val.id, "int")) {
            type->u.basic = INT_TY_NODE;
        }
        else if (streq(son->val.id, "float")) {
            type->u.basic = FLOAT_TY_NODE;
        }
    }
    else if (son->type == StructSpecifier_NODE) {
        return StructSpecifier(son);
    }
}

Type StructSpecifier(Node *cur) {
    if (cur == NULL) return NULL;
    table_enter();
    Node *tag = cur->son;
    tag = tag->succ;
    if (tag->type == OptTag_NODE) {
        char *name = OptTag(tag);
        Node *defList = tag->succ;
        defList = defList->succ;
        FieldList = DefList(defList);
        if (name) {

        }
    }
    else if (tag->type == Tag_NODE) {
        char *name = Tag(tag);
        Type struct_type = table_lookup(name);
        if (struct_type) {
            return struct_type;
        }
        else {
            serror(17, cur->lineno, "Undefined struct");
        }
    }
    return NULL;
}

char *OptTag(Node *cur) {
    if (cur == NULL) return NULL;
    if (cur->type == Null_NODE) return NULL;
    else if (cur->type == OptTag_NODE) {
        Node *id = cur->son;
        if (id->type == ID_NODE) return id->val.id;
    }
    return NULL;
}

char *Tag(Node *cur) {
    if (cur == NULL) return NULL;
    if (cur->type == OptTag_NODE) {
        Node *id = cur->son;
        if (id->type == ID_NODE) return id->val.id;
    }
    return NULL;
}

FieldList VarDec(Node *cur, Type type) {
    if (cur == NULL) return NULL;
    Node *son = cur->son;
    FieldList varDec;
    if (son->type == ID_NODE) {
        varDec = malloc(sizeof(FieldList_));
        varDec->name = son->val.id;
        varDec->type = type;
        varDec->tail = NULL;
        if (tabel_insert(varDec->name, type)) {
            serror(7, son->lineno, "Redefinition of Variable");
        }
        return varDec;
    }
    else if (son->type == VarDec_NODE) {
        Type arr_type = (Type)malloc(sizeof(Type_));
        Node *varDec = son;
        son = son->succ; son = son->succ;
        int size = son->val.ival;
        arr_type->kind = ARRAY;
        arr_type->u.array.elem = type;
        arr_type->u.array.size = size;
        return VarDec(varDec, type); 
    }
}

void CompSt(Node *cur) {
    if (cur == NULL) return;
    if (cur->type == CompSt_NODE) {
        table_enter();
        Node *defList = cur->son; defList = defList->succ;
        DefList(defList);
        Node *stmtList = defList->succ;
        StmtList(stmtList);
        table_leave();  
    }
}

void StmtList(Node *cur) {
    if (cur == NULL) return;
    if (cur->type == StmtList_NODE) {
        Node *stmt = cur->son;
        Stmt(stmt);
        stmt = stmt->succ;
        StmtList(stmt);
    }
}

void Stmt(Node *cur) {
    if (cur == NULL) return ;
    if (cur->type == Stmt_NODE) {
        Node *son = cur->son;
        assert(son);
        switch (son->type) {
        case Exp_NODE: Exp(son); break;
        case CompSt_NODE: Stmt(son); break;         
        case RETURN_NODE: 
        break;
        case IF_NODE: {

        }
        break;
        case WHILE_NODE: {
            son = son->succ; son = son->succ;
            Type cond_type = Exp(son);
            if ()
        }
        break;
        default: assert(0); break;
        }
    }
}

FieldList DefList(Node *cur) {
    if (cur == NULL) return;
    if (cur->type == DefList_NODE) {
        Node *son = cur->son;
        FieldList result = Def(son);
        son = son->succ;
        DefList(son);
    }
}

FieldList Def(Node *cur) {
    if (cur == NULL) return;
    if (cur->type == Def_NODE) {
        Node *specifier = cur->son;
        Type type = Specifier(specifier);
        Node *decList = specifier->succ;
        DecList(decList, type); 
    }
}

FieldList DecList(Node *cur, Type type) {
    if (cur == NULL) return;
    if (cur->type == DecList_NODE) {
        Node *dec = cur->son;
        FieldList new_field = Dec(dec, type);
        Node *decList = dec->succ;
        if (decList) {
            decList = decList->succ;
            new_field->tail = DecList(decList, type);
        }
        return new_field;
    }
}

FieldList Dec(Node *cur, Type type) {
    if (cur == NULL) return;
    if (cur->type == Dec_NODE) {
        Node *varDec = cur->son;
        FieldList new_field = VarDec(varDec, type);
        Node *exp = varDec->succ; exp = exp->succ;
        Type exptype = Exp(exp);
        if (!type_match(exptype, new_field->type)) {
            serror(5, cur->lineno, "Mismatch type");
        }
        return new_field;
    }
}

Type Exp(Node *cur) {
    if (cur == NULL) return NULL;
    Node *lhs = cur->son;
    if (lhs->type == LP_NODE) {
        lhs = lhs->succ;
        return Exp(lhs);
    }
    else if (lhs->type == MINUS_NODE) {
        lhs = lhs->succ;
        Type result = Exp(lhs);
        if (!is_basicty(result))
            serror(7, lhs->lineno, "Invalid type in arithmetic expression.");
    }
    else if (lhs->type == NOT_NODE) {
        lhs = lhs->succ;
        Type result = Exp(lhs);
        if (!is_intty(result)) 
            serror(7, lhs->lineno, "Invalid type in logic expression.");
    }
    else if (lhs->type == INT_NODE) {
        return INT_TY;
    }
    else if (lhs->type == FLOAT_NODE) {
        return FLOAT_TY;
    }
    else if (lhs->type == Exp_NODE) {
        Node *op = lhs->succ;
        switch (op->type){
        case ASSIGNOP_NODE: {

        }
        break;
        case AND_NODE: case OR_NODE: 
        case RELOP_NODE: case PLUS_NODE: case MINUS_NODE:
        case STAR_NODE: case DIV_NODE: {
            Type rhs = op->succ;
            if (is_intty(lhs) && is_intty(rhs)) return INT_TY;
            if (is_floatty(lhs) && is_floatty(rhs) 
            && (op->type != AND_NODE || op->type != OR_NODE)) {
                return op->type == RELOP_NODE ? INT_TY : FLOAT_TY;
            }
            serror(7, lhs->lineno, "Mismatched type in binary expression.");
        }
        break;
        case LB_NODE: {
            Node *id = op->succ;
            Type type = Exp(lhs);
            if (type->kind == ARRAY) {
                Node *exp = id->succ; exp = exp->succ;
                Type index_type = Exp(exp);
                if (is_intty(index_type)) {
                    return type->u.array.elem;
                }
                else {
                    serror(12, exp->lineno, "");
                }
            }
            else serror(10, lhs->lineno, "");
        }
        break;
        case DOT_NODE: {
            Node *id = op->succ;
            Type type = Exp(lhs);
            if (type->kind == STRUCTURE) {
                return get_field(type, id->val.id);
            }
            else {
                serror(13, lhs->lineno, "");
            }
        }
        break;
        default:
        break;
        }
    }
    else if (lhs->type == ID_NODE) {
        Node *args = lhs->succ;
        if (args == NULL) {
            Type result = table_lookup(lhs->val.id);
            if (result) {
                return result; 
            }
            else {
                serror(1, lhs->lineno, "Undefined Variable");
                return NULL;
            }
        }
        else {
            args = args->succ;
            Type func = function_lookup(lhs->val.id);
            if (func) {
                FieldList params = Args(args);
                if (field_match(params, param_type(func))) {
                    return return_type(func);
                }
                else {
                    serror(9, lhs->lineno, "");
                    return NULL;
                }
            }
            else {
                func = table_lookup(lhs->val.id);
                if (func) {
                    serror(11, lhs->lineno, "");
                }
                else {
                    serror(2, )
                }
                return NULL;
            }
        }
    }
}

FieldList Args(Node *cur) {
    if (cur == NULL) return NULL;
    if (cur->type == Args_NODE) {
        Node *exp = cur->son;
        Type type = Exp(exp);
        FieldList new_field = (FieldList)malloc(sizeof(FieldList_));
        new_field->name = NULL;
        new_field->type = type;
        Node *args = exp->succ;
        if (args) {
            args = args->succ;
            new_field->tail = Args(args);
        }
        else {
            new_field->tail = NULL;
        }
        return new_field;
    }
    return NULL;
}

