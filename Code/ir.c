#include <stdio.h>
#include "ir.h"

static int var_no = 0;
static int tmp_no = 0;
static int lab_no = 0;


Operand newOperand();

Operand newConstant(int c) {
    Operand op = (Operand)malloc(sizeof(struct Operand_));
    op->kind = CONSTANT;
    op->u.value = c;
    return op;
}

Operand newVariable() {
    Operand op = (Operand)malloc(sizeof(struct Operand_));
    op->kind = VARIABLE;
    op->u.value = var_no++; 
    return op;
}

Operand newTemp() {
    Operand op = (Operand)malloc(sizeof(struct Operand_));
    op->kind = TEMPORARY;
    op->u.value = tmp_no++; 
    return op;
}

InterCode newLabelIR(){
    InterCode ir = (InterCode)malloc(sizeof(struct InterCode_));
    ir->kind = LABEL;
    ir->u.num = lab_no++;
    return ir;
}

InterCode newFunctionIR(char *name){
    InterCode ir = (InterCode)malloc(sizeof(struct InterCode_));
    ir->kind = FUNCT;
    ir->u.name = name;
    return ir;
}

InterCode newAssignIR(Operand left, Operand right) {
    InterCode ir = (InterCode)malloc(sizeof(struct InterCode_));
    ir->kind = ASSIGN;
    ir->u.assign.left = left;
    ir->u.assign.right = right;
    return ir;
}
InterCode newUnaryIR(Operand result, Operand op, int kind) {
    InterCode ir = (InterCode)malloc(sizeof(struct InterCode_));
    ir->kind = kind;
    ir->u.unop.op = op;
    ir->u.unop.result = result;
    return ir;
}

InterCode newBinaryIR(Operand result, Operand op1, Operand op2, int kind) {
    InterCode ir = (InterCode)malloc(sizeof(struct InterCode_));
    ir->kind = kind;
    ir->u.binop.op1 = op1;
    ir->u.binop.op2 = op2;
    ir->u.binop.result = result;
    return ir;
}

InterCode newJumpIR(InterCode dest) {
    InterCode ir = (InterCode)malloc(sizeof(struct InterCode_));
    ir->kind = JUMP;
    ir->u.dest = dest;
    return ir;
}

InterCode newBranchIR(InterCode dest, Operand op1, Operand op2, int relop) {
    InterCode ir = (InterCode)malloc(sizeof(struct InterCode_));
    ir->kind = BRANCH;
    ir->u.branch.dest = dest;
    ir->u.branch.op1 = op1;
    ir->u.branch.op2 = op2;
    ir->u.branch.relop = relop;
    return ir;
}

InterCode newReturnIR(Operand var) {
    InterCode ir = (InterCode)malloc(sizeof(struct InterCode_));
    ir->kind = RETURN;
    ir->u.var = var;
    return ir;
}

InterCode newArgIR(Operand var) {
    InterCode ir = (InterCode)malloc(sizeof(struct InterCode_));
    ir->kind = ARG;
    ir->u.var = var;
    return ir;
}

InterCode newCallIR(Operand result, InterCode callee) {
    InterCode ir = (InterCode)malloc(sizeof(struct InterCode_));
    ir->kind = CALL;
    ir->u.call.result = result;
    ir->u.call.callee = callee;
    return ir;
}

InterCode newParamIR(Operand var) {
    InterCode ir = (InterCode)malloc(sizeof(struct InterCode_));
    ir->kind = PARAM;
    ir->u.var = var;
    return ir;
}

InterCode newReadIR(Operand var) {
    InterCode ir = (InterCode)malloc(sizeof(struct InterCode_));
    ir->kind = READ;
    ir->u.var = var;
    return ir;
}

InterCode newWriteIR(Operand var) {
    InterCode ir = (InterCode)malloc(sizeof(struct InterCode_));
    ir->kind = WRITE;
    ir->u.var = var;
    return ir;
}

Module newModule() {
    Module module = (Module)malloc(sizeof(struct Module_));
    module->func_list = NULL;
    module->func_tail = NULL;
    return module;
}


Function newFunction(char *name, OpList params) {
    Function func = (Function)malloc(sizeof(struct Function_));
    func->name = name;
    func->entry = func->tail = newFunctionIR(name);
    while (params) {
        Operand param = params->op;
        InterCode ir = newParamIR(param);
        insert_IR(func, ir);
        params = params->tail;
    }
    return func;
}

void insert_IR(Function func, InterCode ir) {
    func->tail->next = ir;
    ir->prev = func->tail;
    func->tail = ir;
}

OpList newOpList(Operand arg, OpList succ_list) {
    OpList op_list = (OpList)malloc(sizeof(struct OpList_));
    op_list->op = arg;
    op_list->tail = succ_list;
    return op_list;
}

void print_Operand(Operand op) {
    if (op->kind == CONSTANT) {
        printf("#%d", op->u.value);
    }
    if (op->kind == VARIABLE) {
        printf("v%d", op->u.value);
    }
    if (op->kind == TEMPORARY) {
        printf("t%d", op->u.value);
    }
}

void print_IR(InterCode cur) {
    if (cur->kind == LABEL) {
        printf("LABEL label%d :", cur->u.num);
    }
    else if (cur->kind == ASSIGN) {
        print_Operand(cur->u.assign.left);
        printf(" := ");
        print_Operand(cur->u.assign.right);
    }
    else if (cur->kind == ADD) {
        print_Operand(cur->u.binop.result);
        printf(" := ");
        print_Operand(cur->u.binop.op1);
        printf(" + ");
        print_Operand(cur->u.binop.op2);
    }
    else if (cur->kind == SUB) {
        print_Operand(cur->u.binop.result);
        printf(" := ");
        print_Operand(cur->u.binop.op1);
        printf(" - ");
        print_Operand(cur->u.binop.op2);
    }
    else if (cur->kind == MUL) {
        print_Operand(cur->u.binop.result);
        printf(" := ");
        print_Operand(cur->u.binop.op1);
        printf(" * ");
        print_Operand(cur->u.binop.op2);
    }
    else if (cur->kind == DIV) {
        print_Operand(cur->u.binop.result);
        printf(" / ");
        print_Operand(cur->u.binop.op1);
        printf(" + ");
        print_Operand(cur->u.binop.op2);
    }
    else if (cur->kind == JUMP) {
        printf("GOTO label%d", cur->u.dest->u.num);
    }
    else if (cur->kind == BRANCH) {
        printf("IF ");
        print_Operand(cur->u.branch.op1);
        switch (cur->u.branch.relop) {
            case LSS : printf(" < ");  break;
            case GRT : printf(" > ");  break;
            case LEQ : printf(" <= "); break;
            case GEQ : printf(" >= "); break;
            case EQ  : printf(" == "); break;
            case NEQ : printf(" != "); break;
            default  : break;
        }
        print_Operand(cur->u.branch.op2);
        printf(" GOTO label%d", cur->u.branch.dest->u.num);
    }
    else if (cur->kind == ARG) {
        printf("ARG ");
        print_Operand(cur->u.var);
    }
    else if (cur->kind == CALL) {
        print_Operand(cur->u.call.result);
        printf(" := CALL %s", cur->u.call.callee->name);
    }
    else if (cur->kind == PARAM) {
        printf("PARAM ");
        print_Operand(cur->u.var);
    }
    else if (cur->kind == READ) {
        printf("READ ");
        print_Operand(cur->u.var);
    }
    else if (cur->kind == WRITE) {
        printf("WRITE ");
        print_Operand(cur->u.var);
    }
    printf("\n");
}

void print_Function(Function func) {
    InterCode ir = func->entry;
    while (ir) {
        print_IR(ir);
    }
}

void print_Module(Module module) {
    FunctionList flist = module->func_list;
    while (flist) {
        print_Function(flist->func);
        flist = flist->next;
        printf("\n");
    }
}