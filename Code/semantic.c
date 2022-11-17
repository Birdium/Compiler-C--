#include <stdio.h>
#include <assert.h>
#include "ast.h"
#include "table.h"
#include "semantic.h"
#include "ir.h"

static int in_struct;
static bool is_fun_dec;
static bool is_lexp;
static Type retype;
static Function func;

static inline bool is_intty(Type type) {
    return type->kind == BASIC && type->u.basic == INT_TY_NODE;
}
static inline bool is_basicty(Type type) {
    return type->kind == BASIC;
}

Module module;

struct Type_ INT_TY_;
Type INT_TY;

Type newArrayType(Type elem, int size) {
    Type newNode = (Type)malloc(sizeof(struct Type_));
    newNode->kind = ARRAY;
    newNode->u.array.elem = elem;
    newNode->u.array.size = size;
    return newNode;
}

Type newStructType(FieldList fieldlist) {
    Type newNode = (Type)malloc(sizeof(struct Type_));
    newNode->kind = STRUCTURE;
    newNode->u.structure = fieldlist;
    return newNode;
} 

Type newFuncType(FieldList fieldlist) {
    Type newNode = (Type)malloc(sizeof(struct Type_));
    newNode->kind = FUNCTION;
    newNode->u.structure = fieldlist;
    return newNode;
} 

FieldList newFieldList(char *name, Type type, FieldList tail){
    FieldList new_field = (FieldList)malloc(sizeof(struct FieldList_));
    new_field->name = name;
    new_field->type = type;
    new_field->tail = tail;
    return new_field;
}

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

void init_Program() {
    INT_TY_.kind = BASIC;
    INT_TY_.u.basic = INT_TY_NODE;
    INT_TY = &INT_TY_;
    FieldList field_int_void = newFieldList("", INT_TY, NULL);  
    FieldList field_int_int = newFieldList("", INT_TY, field_int_void);  

    Type fun_read = newFuncType(field_int_void);
    Type fun_write = newFuncType(field_int_int);
    function_insert("read", fun_read, true, 0);
    function_insert("write", fun_read, true, 0);
    module = newModule();
}

void Program(Node *cur) {
    table_enter();
    init_Program();
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
        else if (node->type == FunDec_NODE) {
            Node *funDec = node;
            node = node->succ;
            if (node->type == CompSt_NODE) {
                func = FunDec(funDec, type);
                CompSt(node);
                char *name = type->u.structure->name;
            }
            else assert(0);
        }
    }
}

Type Specifier(Node *cur) {
    if (cur == NULL) return NULL;
    Node *son = cur->son;
    Type type;
    if (son->type == TYPE_NODE) {
        if (streq(son->val.id, "int")) {
            return INT_TY;
        }
        return NULL;
    }
    else if (son->type == StructSpecifier_NODE) {
        return StructSpecifier(son);
    }
    return NULL;
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
        in_struct++;
        char *name = OptTag(son);
        son = son->succ; son = son->succ;
        FieldList defList = DefList(son);
        Type strty = newStructType(defList);
        in_struct--;
        if (name) {
            if (table_insert(name, strty)) {
                serror(16, cur->lineno, "");
            }
        }
        return strty;
    }
    else if (son->type == Tag_NODE) {
        char *name = Tag(son);
        return table_lookup(name);
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

OpList VarDec(Node *cur, Type type) {
    if (cur == NULL) return NULL;
    Node *son = cur->son;
    if (son->type == ID_NODE) {
        char *id = son->val.id;
        OpList var = newVariable(id, type, NULL);
        table_insert(id, var);
        return var;
    }
    else if (son->type == VarDec_NODE) {
        Node *varDec = son;
        son = son->succ; son = son->succ;
        int size = son->val.ival;
        Type arr_type = newArrayType(type, size);
        return VarDec(varDec, arr_type); 
    }
}

Function FunDec(Node *cur, Type type) {
    if (cur == NULL) return NULL;
    if (cur->type == FunDec_NODE) {
        Node *son = cur->son;
        char *id = son->val.id;
        son = son->succ; son = son->succ;
        OpList params = NULL;
        if (son->type == VarList_NODE) {
            params = VarList(son);
        }
        func = newFunction(id, params);
        return func;
    }
    return NULL;
}

OpList VarList(Node *cur) {
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

OpList ParamDec(Node *cur) {
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
        Node *son = cur->son; son = son->succ;
        DefList(son);
        son = son->succ;
        StmtList(son);
    }
}

void StmtList(Node *cur) {
    if (cur == NULL) return;
    if (cur->type == StmtList_NODE) {
        Node *son = cur->son;
        Stmt(son);
        son = son->succ;
    FieldList varDec;
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
            Operand value = Exp(son);
            InterCode ir1 = newReturnIR(value);
            insert_IR(func, ir1);
        }
        break;
        case IF_NODE: {
            InterCode label1 = newLabelIR();
            InterCode label2 = newLabelIR();
            son = son->succ; son = son->succ;
            Cond(son, label1, label2);
            insert_IR(func, label1);
            son = son->succ; son = son->succ;
            Stmt(son);
            son = son->succ;
            if (son) {
                InterCode label3 = newLabelIR();
                InterCode goto3 = newJumpIR(label3);
                insert_IR(func, goto3);
                insert_IR(func, label2);
                son = son->succ;
                Stmt(son);
                insert_IR(func, label3);
            }
            else {
                insert_IR(func, label2);
            }
        }
        break;
        case WHILE_NODE: {
            InterCode label1 = newLabelIR();
            InterCode label2 = newLabelIR();
            InterCode label3 = newLabelIR();
            insert_IR(func, label1);
            son = son->succ; son = son->succ;
            Cond(son, label2, label3);
            insert_IR(func, label2);
            son = son->succ; son = son->succ;
            Stmt(son);
            InterCode goto1 = newJumpIR(label1);
            insert_IR(func, goto1);
            insert_IR(func, label3);
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
        }
        return new_field;
    }
}

int get_relop(char *name) {
    if (streq(name, "<")) return LSS;
    if (streq(name, ">")) return GRT;
    if (streq(name, "<=")) return LEQ;
    if (streq(name, ">=")) return GEQ;
    if (streq(name, "==")) return EQ;
    if (streq(name, "!=")) return NEQ;
    return -1;
}

void Cond(Node *cur, InterCode label_true, InterCode label_false) {
    Node *son = cur->son;
    if (son->type == NOT_NODE) {
        Cond(son, label_false, label_true);
    }
    else if (cur->type == Exp_NODE) {
        Node *lhs_node = son;
        son = son->succ;
        if (son->type == AND_NODE) {
            Node *rhs_node = son->succ;
            InterCode label1 = newLabelIR();
            Cond(lhs_node, label1, label_false);
            insert_IR(func, label1);
            Cond(rhs_node, label_true, label_false);
        }
        else if (son->type == OR_NODE) {
            Node *rhs_node = son->succ;
            InterCode label1 = newLabelIR();
            Cond(lhs_node, label_true, label1);
            insert_IR(func, label1);
            Cond(rhs_node, label_true, label_false);
        }
        else if (son->type == RELOP_NODE) {
            int op = get_relop(son->val.id);
            Node *rhs_node = son->succ;
            Operand lhs = Exp(lhs_node);
            Operand rhs = Exp(rhs_node);
            InterCode ir1 = newBranchIR(label_true, lhs, rhs, op);
            InterCode ir2 = newJumpIR(label_false);
            insert_IR(func, ir1);
            insert_IR(func, ir2);
        }
    }
    else {
        Operand t1 = newTemp();
        Operand zero = newConstant(0);
        InterCode ir1 = newBranchIR(label_true, t1, zero, NEQ);
        InterCode ir2 = newJumpIR(label_false);
        insert_IR(func, ir1);
        insert_IR(func, ir2);
    }

}

Operand Exp(Node *cur) {
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
        Operand op = Exp(son);
        Operand zero = newConstant(0);
        Operand result = newTemp();
        InterCode ir1 = newBinaryIR(result, zero, op, SUB);
        return result;
    }
    else if (son->type == NOT_NODE) {
        InterCode label1 = newLabelIR();
        InterCode label2 = newLabelIR();
        Operand result = newTemp();
        InterCode r0 = newAssignIR(result, newConstant(0));
        InterCode r1 = newAssignIR(result, newConstant(1));
        insert_IR(func, r0);
        Cond(cur, label1, label2);
        insert_IR(func, label1);
        insert_IR(func, r1);
        insert_IR(func, label2);
        return result;
    }
    else if (son->type == INT_NODE) {
        int value = son->val.ival;
        Operand constant = newConstant(value);
        Operand result = newTemp();
        InterCode ir1 = newAssignIR(result, constant);
        insert_IR(func, ir1);
        return result;
    }
    else if (son->type == Exp_NODE) {
        Node *lhs_node = son;
        son = son->succ;
        switch (son->type){
            case ASSIGNOP_NODE: { // lhs = rhs
                Operand lhs = LExp(lhs_node); 
                son = son->succ;
                Operand rhs = Exp(son);
                Operand result = newTemp();
                InterCode ir1 = newAssignIR(lhs, rhs);
                InterCode ir2 = newAssignIR(result, lhs);
                insert_IR(func, ir1);
                insert_IR(func, ir2);
                return lhs;
            }
            break;
            case AND_NODE: case OR_NODE: 
            case RELOP_NODE: {
                InterCode label1 = newLabelIR();
                InterCode label2 = newLabelIR();
                Operand result = newTemp();
                InterCode r0 = newAssignIR(result, newConstant(0));
                InterCode r1 = newAssignIR(result, newConstant(1));
                insert_IR(func, r0);
                Cond(cur, label1, label2);
                insert_IR(func, label1);
                insert_IR(func, r1);
                insert_IR(func, label2);
                return result;
            }
            break;
            case PLUS_NODE: case MINUS_NODE:
            case STAR_NODE: case DIV_NODE: {
                int kind = 0;
                if (son->type == PLUS_NODE) kind = ADD;
                if (son->type == MINUS_NODE) kind = SUB;
                if (son->type == STAR_NODE) kind = MUL;
                if (son->type == DIV_NODE) kind = DIV;
                Node *rhs_node = son->succ;
                Operand lhs = Exp(lhs_node);
                Operand rhs = Exp(rhs_node);
                Operand result = newTemp();
                InterCode ir1 = newBinaryIR(result, lhs, rhs, kind);
                insert_IR(func, ir1);
                return result;
            }
            break;
            // case LB_NODE: {
            //     son = son->succ;            
            //     if (lhs->kind == ARRAY) {
            //         Type index = Exp(son);
            //         if (is_intty(index)) {
            //             is_lexp = true;
            //             return lhs->u.array.elem;
            //         }
            //     }
            //     return NULL;
            // }
            // break;
            // case DOT_NODE: {
            //     Node *id = op->succ;
            //     if (lhs && lhs->kind == STRUCTURE) {
            //         Type field_type = get_field(lhs, id->val.id);
            //         if (field_type) {
            //             is_lexp = true;
            //             return field_type;
            //         }
            //     }
            //     return NULL;
            // }
            // break;
            default:
            break;
        }
    }
    else if (son->type == ID_NODE) {
        char *id = son->val.id;
        son = son->succ;
        if (son == NULL) {
            Operand var = table_lookup(id);
            Operand result = newTemp();
            InterCode ir1 = newAssignIR(result, var);
            insert_IR(func, ir1);
            return result;
        }
        else {
            Node *args_node = son->succ;
            Function callee = function_lookup(id);
            Operand result = newTemp();
            if (args_node->type == Args_NODE) { // ID LP Args RP
                InterCode ir1 = NULL;
                if (callee->name == "read") {
                    ir1 = newReadIR(result);
                }
                else {
                    ir1 = newCallIR(result, callee); 
                }
                insert_IR(func, ir1);
            }
            else { // ID LP RP
                ArgList arg_list = Args(args_node);
                if (callee->name == "write") {
                    Operand zero = newConstant(0);
                    InterCode ir1 = newWriteIR(arg_list->arg);
                    InterCode ir2 = newAssignIR(result, zero);
                    insert_IR(func, ir1);
                    insert_IR(func, ir2);
                }
                else {
                    while (arg_list) {
                        InterCode ir_arg = newArgIR(arg_list->arg);
                        insert_IR(func, ir_arg);
                        arg_list = arg_list->next;
                    }
                    InterCode ir1 = newCallIR(result, callee);
                    insert_IR(func, ir1);
                }
            }
            return result;
        }
    }
}

ArgList Args(Node *cur) {
    if (cur == NULL) return NULL;
    if (cur->type == Args_NODE) {
        Node *son = cur->son;
        Operand arg = Exp(son);
        ArgList succ_args = NULL; 
        son = son->succ;
        if (son) {
            son = son->succ;
            succ_args = Args(son);
        }
        return newArgList(arg, succ_args);
    }
    return NULL;
}