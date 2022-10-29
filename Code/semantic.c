#include <stdio.h>
#include <assert.h>
#include "ast.h"
#include "table.h"
#include "semantic.h"

static int in_struct;
static bool is_fun_dec;
static bool is_lexp;
static Type retype;

static inline bool is_intty(Type type) {
    return type->kind == BASIC && type->u.basic == INT_TY_NODE;
}
static inline bool is_floatty(Type type) {
    return type->kind == BASIC && type->u.basic == FLOAT_TY_NODE;
}
static inline bool is_basicty(Type type) {
    return type->kind == BASIC;
}

struct Type_ INT_TY_, FLOAT_TY_;
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
    if (ty1->kind == ARRAY) return array_match(ty1, ty2);
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

Type get_field(Type type, char *id) {
    FieldList iter = type->u.structure;
    while (iter) {
        if (streq(id, iter->name)) return iter->type;
        iter = iter->tail;
    }
    return NULL;
}

void Program(Node *cur) {
    INT_TY_.kind = FLOAT_TY_.kind = BASIC;
    INT_TY_.u.basic = INT_TY_NODE;
    FLOAT_TY_.u.basic = FLOAT_TY_NODE;
    INT_TY = &INT_TY_;
    FLOAT_TY = &FLOAT_TY_;
    table_enter();
    ExtDefList(cur->son);
    check_funclist();
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
        Node *son = cur->son;
        Type type = Specifier(son);
        if (!type) return NULL;
        Node *node = son->succ;
        if (node->type == SEMI_NODE) {
            return;
        }
        else if (node->type == ExtDecList_NODE) {
            ExtDecList(node, type);
        }
        else if (node->type == FunDec_NODE) {
            table_enter();
            Node *funDec = node;
            node = node->succ;
            if (node->type == CompSt_NODE) {
                type = FunDec(funDec, type);
                retype = return_type(type);
                CompSt(node);
                retype = NULL;
                table_leave();
                char *name = type->u.structure->name;
                function_insert(name, type, true, funDec->lineno);
            }
            else if (node->type == SEMI_NODE) {
                is_fun_dec = true;
                type = FunDec(funDec, type);
                is_fun_dec = false;
                retype = NULL;
                table_leave();
                char *name = type->u.structure->name;
                function_insert(name, type, false, funDec->lineno);
            }
            else assert(0);
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
        type = (Type)malloc(sizeof(struct Type_));
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

static inline void printfield(FieldList f) {
    if (!f) { printf("\n"); return; }
    printf("%s %u ", f->name, f->type->kind); printfield(f->tail);
}

Type StructSpecifier(Node *cur) {
    if (cur == NULL) return NULL;
    Node *son = cur->son;
    son = son->succ;
    if (son->type == OptTag_NODE || son->type == Null_NODE) {
        table_enter();
        in_struct++;
        char *name = OptTag(son);
        son = son->succ; son = son->succ;
        FieldList defList = DefList(son);
        Type strty = (Type)malloc(sizeof(struct Type_));
        strty->kind = STRUCTURE;
        strty->u.structure = defList;
        in_struct--;
        table_leave();
        if (name) {
            if (table_insert(name, strty)) {
                serror(16, cur->lineno, "");
            }
        }
        return strty;
    }
    else if (son->type == Tag_NODE) {
        char *name = Tag(son);
        Type strty = table_lookup(name);
        if (strty) {
            return strty;
        }
        else {
            serror(17, cur->lineno, "Undefined structure");
            return NULL;
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
    if (cur->type == Tag_NODE) {
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
        varDec = malloc(sizeof(struct FieldList_));
        varDec->name = son->val.id;
        varDec->type = type;
        varDec->tail = NULL;
        if (table_insert(varDec->name, type)) {
            serror(in_struct ? 15 : 3, son->lineno, "Redefinition of Variable");
        }
        return varDec;
    }
    else if (son->type == VarDec_NODE) {
        Type arr_type = (Type)malloc(sizeof(struct Type_));
        Node *varDec = son;
        son = son->succ; son = son->succ;
        int size = son->val.ival;
        arr_type->kind = ARRAY;
        arr_type->u.array.elem = type;
        arr_type->u.array.size = size;
        return VarDec(varDec, arr_type); 
    }
}

Type FunDec(Node *cur, Type type) {
    if (cur == NULL) return NULL;
    if (cur->type == FunDec_NODE) {
        Node *son = cur->son;
        char *id = son->val.id;
        son = son->succ; son = son->succ;
        FieldList retfield = (FieldList)malloc(sizeof(struct FieldList_));
        retfield->name = id;
        retfield->type = type;
        FieldList varlist = NULL;
        if (son->type == VarList_NODE) {
            varlist = VarList(son);
        }
        retfield->tail = varlist;
        Type functy = (Type)malloc(sizeof(struct Type_));
        functy->kind = FUNCTION;
        functy->u.structure = retfield;
        if (is_fun_dec) { // declaration
            // deadcode
            if (function_insert(id, functy, false, cur->lineno)) {
                serror(19, son->lineno, "Declaration error");
            }
            // deadcode
        }
        else { // define 
            if (function_insert(id, functy, true, cur->lineno)) {
                serror(4, son->lineno, "Redefined function");
            }
        }
        return functy;
    }
    return NULL;
}

FieldList VarList(Node *cur) {
    if (cur == NULL) return NULL;
    if (cur->type == VarList_NODE) {
        Node *son = cur->son;
        FieldList param = ParamDec(son);
        son = son->succ;
        if (son) {
            son = son->succ;
            param->tail = VarList(son);
        } 
        return param;
    }
    return NULL;
}

FieldList ParamDec(Node *cur) {
    if (cur == NULL) return NULL;
    if (cur->type == ParamDec_NODE) {
        Node *son = cur->son;
        Type spec = Specifier(son);
        if (!spec) return NULL;
        son = son->succ;
        return VarDec(son, spec);
    }
}

void CompSt(Node *cur) {
    if (cur == NULL) return;
    if (cur->type == CompSt_NODE) {
        table_enter();
        Node *son = cur->son; son = son->succ;
        DefList(son);
        son = son->succ;
        StmtList(son);
        table_leave();  
    }
}

void StmtList(Node *cur) {
    if (cur == NULL) return;
    if (cur->type == StmtList_NODE) {
        Node *son = cur->son;
        Stmt(son);
        son = son->succ;
        StmtList(son);
    }
}

void Stmt(Node *cur) {
    if (cur == NULL) return ;
    if (cur->type == Stmt_NODE) {
        Node *son = cur->son;
        assert(son);
        switch (son->type) {
        case Exp_NODE: Exp(son); break;
        case CompSt_NODE: CompSt(son); break;         
        case RETURN_NODE: {
            son = son->succ;
            Type type = Exp(son);
            if (!type_match(type, retype)) {
                serror(8, son->lineno, "Type mismatched for return");
            }
        }
        break;
        case IF_NODE: {
            son = son->succ; son = son->succ;
            Exp(son);
            son = son->succ; son = son->succ;
            Stmt(son);
            son = son->succ;
            if (son) {
                son = son->succ;
                Stmt(son);
            }
        }
        break;
        case WHILE_NODE: {
            son = son->succ; son = son->succ;
            Exp(son);
            son = son->succ; son = son->succ;
            Stmt(son);
        }
        break;
        default: assert(0); break;
        }
    }
}

FieldList DefList(Node *cur) {
    if (cur == NULL) return NULL;
    if (cur->type == DefList_NODE) {
        Node *son = cur->son;
        FieldList result = Def(son);
        son = son->succ;
        if (!result) return DefList(son);
        FieldList tail = result;
        while (tail->tail) {
            tail = tail->tail;
        }
        tail->tail = DefList(son);
        return result;
    }
    return NULL;
}

FieldList Def(Node *cur) {
    if (cur == NULL) return NULL;
    if (cur->type == Def_NODE) {
        Node *son = cur->son;
        Type type = Specifier(son);
        if (!type) return NULL;
        son = son->succ;
        return DecList(son, type); 
    }
    return NULL;
}

FieldList DecList(Node *cur, Type type) {
    if (cur == NULL) return NULL;
    if (cur->type == DecList_NODE) {
        Node *son = cur->son;
        FieldList new_field = Dec(son, type);
        son = son->succ;
        if (son) {
            son = son->succ;
            new_field->tail = DecList(son, type);
        }
        return new_field;
    }
    return NULL;
}

FieldList Dec(Node *cur, Type type) {
    if (cur == NULL) return NULL;
    if (cur->type == Dec_NODE) {
        Node *son = cur->son;
        FieldList new_field = VarDec(son, type);
        son = son->succ;
        if (son) {
            son = son->succ;
            Type exptype = Exp(son);
            if (in_struct) {
                serror(15, cur->lineno, "Cannot initialize field in struct");
            }
            if (!type_match(exptype, new_field->type)) {
                serror(5, cur->lineno, "Mismatch type");
            }
        }
        return new_field;
    }
}

Type Exp(Node *cur) {
    if (cur == NULL) return NULL;
    if (cur->type == Null_NODE) return NULL;
    Node *son = cur->son;
    if (son == NULL) return NULL;
    if (son->type == LP_NODE) {
        son = son->succ;
        return Exp(son);
    }
    else if (son->type == MINUS_NODE) {
        son = son->succ;
        Type exp = Exp(son);
        if (!is_basicty(exp)) {
            serror(7, son->lineno, "Invalid type in arithmetic expression");
        }
        is_lexp = false;
        return exp;
    }
    else if (son->type == NOT_NODE) {
        son = son->succ;
        Type exp = Exp(son);
        if (!is_intty(exp)) {
            serror(7, son->lineno, "Invalid type in logic expression");
        }
        is_lexp = false;
        return INT_TY;
    }
    else if (son->type == INT_NODE) {
        is_lexp = false;
        return INT_TY;
    }
    else if (son->type == FLOAT_NODE) {
        is_lexp = false;
        return FLOAT_TY;
    }
    else if (son->type == Exp_NODE) {
        Type lhs = Exp(son);
        if (!lhs) return NULL;
        son = son->succ;
        Node *op = son;
        switch (op->type){
        case ASSIGNOP_NODE: {
            if (lhs == NULL) return NULL;
            son = son->succ;
            if (!is_lexp) {
                serror(6, son->lineno, "The left-hand side of an assignment must be a variable");
            }
            Type rhs = Exp(son);
            if (rhs == NULL) return NULL;
            if (!type_match(lhs, rhs)){
                serror(5, son->lineno, "Type mismatch for assign");
            }
            is_lexp = true;
            return lhs;
        }
        break;
        case AND_NODE: case OR_NODE: 
        case RELOP_NODE: {
            son = son->succ;
            Type rhs = Exp(son);
            if (!rhs) return NULL;
            is_lexp = false;
            if (is_intty(lhs) && is_intty(rhs)) return INT_TY;
            if (is_floatty(lhs) && is_floatty(rhs)) return INT_TY;
            serror(7, son->lineno, "Mismatched type in binary expression");
            return INT_TY;
        }
        break;
        case PLUS_NODE: case MINUS_NODE:
        case STAR_NODE: case DIV_NODE: {
            son = son->succ;
            Type rhs = Exp(son);
            if (!rhs) return NULL;
            is_lexp = false;
            if (is_intty(lhs) && is_intty(rhs)) return INT_TY;
            if (is_floatty(lhs) && is_floatty(rhs)) return FLOAT_TY;
            serror(7, son->lineno, "Mismatched type in binary expression");
            return lhs;
        }
        break;
        case LB_NODE: {
            son = son->succ;            
            if (lhs->kind == ARRAY) {
                Type index = Exp(son);
                if (is_intty(index)) {
                    is_lexp = true;
                    return lhs->u.array.elem;
                }
                else {
                    serror(12, son->lineno, "");
                    return NULL;
                }
            }
            else {
                serror(10, op->lineno, "");
                return NULL;
            }
        }
        break;
        case DOT_NODE: {
            Node *id = op->succ;
            if (lhs && lhs->kind == STRUCTURE) {
                Type field_type = get_field(lhs, id->val.id);
                if (field_type) {
                    is_lexp = true;
                    return field_type;
                }
                else {
                    serror(14, id->lineno, "Non-existence field");
                    return NULL;
                }
            }
            else {
                serror(13, id->lineno, "Illegal use of \".\"");
                return NULL;
            }
        }
        break;
        default:
        break;
        }
    }
    else if (son->type == ID_NODE) {
        char *id = son->val.id;
        Node *args = son->succ; 
        if (args == NULL) {
            Type result = table_lookup(id);
            if (result) {
                is_lexp = true;
                return result; 
            }
            else {
                serror(1, son->lineno, "Undefined Variable");
                return NULL;
            }
        }
        else {
            args = args->succ;
            Type func = function_lookup(id);
            if (func) {
                FieldList params = Args(args);
                if (field_match(params, param_type(func))) {
                    is_lexp = false;
                    return return_type(func);
                }
                else {
                    serror(9, args->lineno, "");
                    return NULL;
                }
            }
            else {
                func = table_lookup(id);
                if (func) {
                    serror(11, args->lineno, "");
                }
                serror(2, args->lineno, "Undefined Function");
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
        FieldList new_field = (FieldList)malloc(sizeof(struct FieldList_));
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