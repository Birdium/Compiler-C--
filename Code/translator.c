#include <stdio.h>
#include <assert.h>
#include "translator.h"
#include "ast.h"
#include "table.h"
#include "ir.h"
#include "codegen.h"

Module module;
Function func;

bool is_ParamDec;

void init_translator() {
    module = newModule();
}

void translate_Program(Node *cur) {
    init_translator();
    translate_ExtDefList(cur->son);
	// print_Module(module);
    gencode_Module(module);
}

void translate_ExtDefList(Node *cur) {
    if (cur->type == ExtDefList_NODE) {
        Node *ext_def = cur->son;
        translate_ExtDef(ext_def);
        Node *ext_def_list = ext_def->succ;
        translate_ExtDefList(ext_def_list);
    }
}

void translate_ExtDef(Node *cur) {
    if (cur == NULL) return;
    if (cur->type == ExtDef_NODE) {
        Node *son = cur->son;
        Node *node = son->succ;
        if (node->type == SEMI_NODE) {
            return;
        }
        else if (node->type == FunDec_NODE) {
            Node *funDec = node;
            node = node->succ;
            if (node->type == CompSt_NODE) {
                func = translate_FunDec(funDec);
				insert_fun(module, func);
                translate_CompSt(node);
            }
            else assert(0);
        }
    }
}

Operand translate_VarDec(Node *cur) {
    if (cur == NULL) return NULL;
    Node *son = cur->son;
    if (son->type == ID_NODE) {
        char *id = son->val.id;
		Type type = table_lookup(id);
        Operand var = newVariable(type);
        if (type->kind != BASIC && is_ParamDec) {
            var = makeReference(var);
        }
        if (type->kind != BASIC && !is_ParamDec) {
            int size = get_type_size(type);
            Operand op = newConstant(size);
            InterCode ir = newDecIR(var, size);
            insert_IR(func, ir);
        }
		table_update(id, var);
        return var;
    }
    else if (son->type == VarDec_NODE) {
        Node *varDec = son;
        return translate_VarDec(varDec); 
    }
}

Function translate_FunDec(Node *cur) {
    if (cur == NULL) return NULL;
    if (cur->type == FunDec_NODE) {
        Node *son = cur->son;
        char *id = son->val.id;
        son = son->succ; son = son->succ;
        OpList params = NULL;
        if (son->type == VarList_NODE) {
            params = translate_VarList(son);
        }
        func = newFunction(id, params);
        function_update(id, func, true, 0);
        return func;
    }
    return NULL;
}

OpList translate_VarList(Node *cur) {
    if (cur == NULL) return NULL;
    if (cur->type == VarList_NODE) {
        Node *son = cur->son;
        OpList param = translate_ParamDec(son);
        son = son->succ;
        if (son) {
            son = son->succ;
            param->tail = translate_VarList(son);
        }
        return param;
    }
    return NULL;
}

OpList translate_ParamDec(Node *cur) {
    if (cur == NULL) return NULL;
    if (cur->type == ParamDec_NODE) {
        Node *son = cur->son;
        son = son->succ;
        is_ParamDec = true;
        Operand op = translate_VarDec(son);
        is_ParamDec = false;
        return newOpList(op, NULL);
    }
}

void translate_CompSt(Node *cur) {
    if (cur == NULL) return;
    if (cur->type == CompSt_NODE) {
        Node *son = cur->son; son = son->succ;
        translate_DefList(son);
        son = son->succ;
        translate_StmtList(son);
    }
}

void translate_StmtList(Node *cur) {
    if (cur == NULL) return;
    if (cur->type == StmtList_NODE) {
        Node *son = cur->son;
        translate_Stmt(son);
        son = son->succ;
        translate_StmtList(son);
    }
}

void translate_Stmt(Node *cur) {
    if (cur == NULL) return ;
    if (cur->type == Stmt_NODE) {
        Node *son = cur->son;
        assert(son);
        switch (son->type) {
        case Exp_NODE: translate_Exp(son); break;
        case CompSt_NODE: translate_CompSt(son); break;         
        case RETURN_NODE: {
            son = son->succ;
            Operand value = translate_Exp(son);
            InterCode ir1 = newReturnIR(value);
            insert_IR(func, ir1);
        }
        break;
        case IF_NODE: {
            InterCode label1 = newLabelIR();
            InterCode label2 = newLabelIR();
            son = son->succ; son = son->succ;
            translate_Cond(son, label1, label2);
            insert_IR(func, label1);
            son = son->succ; son = son->succ;
            translate_Stmt(son);
            son = son->succ;
            if (son) {
                InterCode label3 = newLabelIR();
                InterCode goto3 = newJumpIR(label3);
                insert_IR(func, goto3);
                insert_IR(func, label2);
                son = son->succ;
                translate_Stmt(son);
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
            translate_Cond(son, label2, label3);
            insert_IR(func, label2);
            son = son->succ; son = son->succ;
            translate_Stmt(son);
            InterCode goto1 = newJumpIR(label1);
            insert_IR(func, goto1);
            insert_IR(func, label3);
        }
        break;
        default: assert(0); break;
        }
    }
}

void translate_DefList(Node *cur) {
    if (cur == NULL) return;
    if (cur->type == DefList_NODE) {
        Node *son = cur->son;
        translate_Def(son);
        son = son->succ;
        translate_DefList(son);
    }
}

void translate_Def(Node *cur) {
    if (cur == NULL) return;
    if (cur->type == Def_NODE) {
        Node *son = cur->son;
        son = son->succ;
        translate_DecList(son); 
    }
}

void translate_DecList(Node *cur) {
    if (cur == NULL) return;
    if (cur->type == DecList_NODE) {
        Node *son = cur->son;
        translate_Dec(son);
        son = son->succ;
        if (son) translate_DecList(son->succ);
    }
}

void translate_Dec(Node *cur) {
    if (cur == NULL) return;
    if (cur->type == Dec_NODE) {
        Node *son = cur->son;
        Operand var = translate_VarDec(son);
        son = son->succ;
        if (son) {
            son = son->succ;
            Operand exp = translate_Exp(son);
			InterCode ir1 = newAssignIR(var, exp);
			insert_IR(func, ir1);
        }
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

void translate_Cond(Node *cur, InterCode label_true, InterCode label_false) {
    Node *son = cur->son;
    if (son->type == NOT_NODE) {
        son = son->succ;
        translate_Cond(son, label_false, label_true);
    }
    else if (cur->type == Exp_NODE) {
        Node *lhs_node = son;
        son = son->succ;
        if (son && son->type == AND_NODE) {
            Node *rhs_node = son->succ;
            InterCode label1 = newLabelIR();
            translate_Cond(lhs_node, label1, label_false);
            insert_IR(func, label1);
            translate_Cond(rhs_node, label_true, label_false);
        }
        else if (son && son->type == OR_NODE) {
            Node *rhs_node = son->succ;
            InterCode label1 = newLabelIR();
            translate_Cond(lhs_node, label_true, label1);
            insert_IR(func, label1);
            translate_Cond(rhs_node, label_true, label_false);
        }
        else if (son && son->type == RELOP_NODE) {
            int op = get_relop(son->val.id);
            Node *rhs_node = son->succ;
            Operand lhs = translate_Exp(lhs_node);
            Operand rhs = translate_Exp(rhs_node);
            InterCode ir1 = newBranchIR(label_true, lhs, rhs, op);
            InterCode ir2 = newJumpIR(label_false);
            insert_IR(func, ir1);
            insert_IR(func, ir2);
        }
        else {
            Operand t1 = translate_Exp(cur);
            Operand zero = newConstant(0);
            InterCode ir1 = newBranchIR(label_true, t1, zero, NEQ);
            InterCode ir2 = newJumpIR(label_false);
            insert_IR(func, ir1);
            insert_IR(func, ir2);
        }
    }
}

// dangerous!!!
Type left_type;

Operand translate_LExp(Node *cur) {
	Node *son = cur->son;
    if (son->type == ID_NODE) {
        char *id = son->val.id;
        son = son->succ;
        if (!son) {
            Type type = table_lookup(id);
            Operand op = table_getop(id);
            if (type->kind == ARRAY || type->kind == STRUCTURE) left_type = type;
            if (type->kind != BASIC && op->kind != ADDRESS) {
                return makeAddress(op); // for vars like DEC v1
            }
            return op;
        }
    }
    else if (son->type == Exp_NODE){
        Node *lhs_node = son;
        son = son->succ;
        if (son->type == DOT_NODE) {
            Operand base = translate_LExp(lhs_node); 
            Operand addr = newTemp();
            son = son->succ;
            char *id = son->val.id;
            int offset = table_getoffset(id);
            left_type = get_field(left_type, id);
            Operand op = newConstant(offset);
            InterCode ir1 = newBinaryIR(addr, base, op, ADD);
            insert_IR(func, ir1);
            return addr;
        }
        else if (son->type == LB_NODE) {
            Operand base = translate_LExp(lhs_node);
            Operand addr = newTemp();
            son = son->succ;
            Operand index = translate_Exp(son);
            left_type = left_type->u.array.elem;
            int size = get_type_size(left_type);
            Operand op = newConstant(size);
            Operand offset = newTemp();
            InterCode ir1 = newBinaryIR(offset, index, op, MUL);
            InterCode ir2 = newBinaryIR(addr, base, offset, ADD);
            insert_IR(func, ir1);
            insert_IR(func, ir2);
            return addr;
        }
        else {
            assert(0);
        }
    }
    return NULL;
}

Operand translate_Exp(Node *cur) {
    if (cur == NULL) return NULL;
    if (cur->type == Null_NODE) return NULL;
    Node *son = cur->son;
    if (son == NULL) return NULL;
    if (son->type == LP_NODE) {
        son = son->succ;
        return translate_Exp(son);
    }
    else if (son->type == MINUS_NODE) {
        son = son->succ;
        Operand op = translate_Exp(son);
        Operand zero = newConstant(0);
        Operand result = newTemp();
        InterCode ir1 = newBinaryIR(result, zero, op, SUB);
		insert_IR(func, ir1);
        return result;
    }
    else if (son->type == NOT_NODE) {
        InterCode label1 = newLabelIR();
        InterCode label2 = newLabelIR();
        Operand result = newTemp();
        InterCode r0 = newAssignIR(result, newConstant(0));
        InterCode r1 = newAssignIR(result, newConstant(1));
        insert_IR(func, r0);
        translate_Cond(cur, label1, label2);
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
                Operand lhs = translate_LExp(lhs_node); 
                son = son->succ;
                if (lhs->kind != VARIABLE) 
                    lhs = makeReference(lhs);
                Operand rhs = translate_Exp(son);
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
                translate_Cond(cur, label1, label2);
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
                Operand lhs = translate_Exp(lhs_node);
                Operand rhs = translate_Exp(rhs_node);
                Operand result = newTemp();
                InterCode ir1 = newBinaryIR(result, lhs, rhs, kind);
                insert_IR(func, ir1);
                return result;
            }
            break;
            case LB_NODE: {
                Operand base = translate_LExp(lhs_node);
                Operand addr = newTemp();
                Operand offset = newTemp();
                son = son->succ;
                left_type = left_type->u.array.elem;
                Type tmp_type = left_type;
                int size = get_type_size(left_type);
                Operand op = newConstant(size);
                Operand index = translate_Exp(son);
                left_type = tmp_type;
                InterCode ir1 = newBinaryIR(offset, index, op, MUL);
                InterCode ir2 = newBinaryIR(addr, base, offset, ADD);
                if (left_type->kind != BASIC) {
                    insert_IR(func, ir1);
                    insert_IR(func, ir2);
                    return addr;
                }
                else {
                    Operand result = newTemp();
                    addr = makeReference(addr);
                    InterCode ir3 = newAssignIR(result, addr);
                    insert_IR(func, ir1);
                    insert_IR(func, ir2);
                    insert_IR(func, ir3);
                    return result;
                }
            }
            break;
            case DOT_NODE: {
                Operand base = translate_LExp(lhs_node); 
                Operand addr = newTemp();
                son = son->succ;
                char *id = son->val.id;
                int offset = table_getoffset(id);
                Operand op = newConstant(offset);
                InterCode ir1 = newBinaryIR(addr, base, op, ADD);
                left_type = get_field(left_type, id);
                if (left_type->kind != BASIC) {
                    insert_IR(func, ir1);
                    return addr;
                }
                else {
                    Operand result = newTemp();
                    addr = makeReference(addr);
                    InterCode ir2 = newAssignIR(result, addr);
                    insert_IR(func, ir1);
                    insert_IR(func, ir2);
                    return result;
                }
            }
            break;
            default:
            break;
        }
    }
    else if (son->type == ID_NODE) {
        char *id = son->val.id;
        son = son->succ;
        if (son == NULL) {
            Type type = table_lookup(id);
            Operand var = table_getop(id);
            if (type->kind != BASIC) {
                var = makeAddress(var);
            }
            Operand result = newTemp();
            InterCode ir1 = newAssignIR(result, var);
            insert_IR(func, ir1);
            return result;
        }
        else {
            Node *args_node = son->succ;
            Function callee = function_getfunc(id);
            Operand result = newTemp();
            if (args_node->type == Args_NODE) { // ID LP Args RP
                OpList arg_list = translate_Args(args_node);
                if (streq(id, "write")) {
                    Operand zero = newConstant(0);
                    InterCode ir1 = newWriteIR(arg_list->op);
                    InterCode ir2 = newAssignIR(result, zero);
                    insert_IR(func, ir1);
                    insert_IR(func, ir2);
                }
                else {
                    insert_ArgList(func, arg_list);
                    InterCode ir1 = newCallIR(result, callee);
                    insert_IR(func, ir1);
                }
            }
            else { // ID LP RP
                InterCode ir1 = NULL;
                if (streq(id, "read")) {
                    ir1 = newReadIR(result);
                }
                else {
                    ir1 = newCallIR(result, callee); 
                }
                insert_IR(func, ir1);
            }
            return result;
        }
    }
}

OpList translate_Args(Node *cur) {
    if (cur == NULL) return NULL;
    if (cur->type == Args_NODE) {
        Node *son = cur->son;
        Operand arg = translate_Exp(son);
        OpList succ_args = NULL; 
        son = son->succ;
        if (son) {
            son = son->succ;
            succ_args = translate_Args(son);
        }
        return newOpList(arg, succ_args);
    }
    return NULL;
}