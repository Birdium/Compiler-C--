#ifndef IR_H
#define IR_H

typedef struct Operand_* Operand;
struct Operand_{
    enum {VARIABLE, CONSTANT, TEMPORARY, ADDRESS} kind;
    union {
        int value;
    } u;
};

typedef struct InterCode_* InterCode;
typedef struct Function_* Function;
typedef struct OpList_* OpList;


struct InterCode_ {
    enum {ASSIGN, ADD, SUB, MUL, DIV, 
          LABEL, FUNCT, JUMP, BRANCH, 
          RETURN, DEC, ARG, CALL, PARAM,
          READ, WRITE} kind;
    union {
        struct {Operand right, left;} assign;
        struct {Operand result, op1, op2;} binop;
        struct {Operand result, op;} unop;
        int num;
        char *name;
        InterCode dest;
        Operand var;
        struct {InterCode dest; Operand op1, op2; enum {LSS, GRT, LEQ, GEQ, EQ, NEQ} relop;} branch;
        struct {Operand result; Function callee;} call;
    } u;
    InterCode prev, next;
};

struct Function_ {
    char *name;
    OpList params;
    InterCode entry;
    InterCode tail;
};

typedef struct FunctionList_* FunctionList;
struct FunctionList_{
    Function func;
    FunctionList next;
}; 

typedef struct Module_* Module;
struct Module_ {
    FunctionList func_list;
    FunctionList func_tail;
};
struct OpList_ {
    Operand op;
    OpList tail;
};

Operand newConstant(int x);
Operand newVariable();
Operand newTemp();
OpList newOpList(Operand arg, OpList succ_list);
InterCode newIR();
InterCode newFunctionIR(char *name);
InterCode newLabelIR();
InterCode newAssignIR(Operand left, Operand right);
InterCode newUnaryIR(Operand result, Operand op, int kind);
InterCode newBinaryIR(Operand result, Operand op1, Operand op2, int kind);
InterCode newJumpIR(InterCode dest);
InterCode newBranchIR(InterCode dest, Operand op1, Operand op2, int relop);
InterCode newReturnIR(Operand var);
InterCode newArgIR(Operand var);
InterCode newCallIR(Operand result, InterCode callee);
InterCode newParamIR(Operand var);
InterCode newReadIR(Operand var);
InterCode newWriteIR(Operand var);

Module newModule();
Function newFunction(char *name, OpList params);
FunctionList newFunctionList(Function func, FunctionList next);
void insert_IR(Function func, InterCode ir);
void insert_fun(Module module, Function func);

void print_Module(Module);
void print_Function(Function);
void print_IR(InterCode);
void print_Operand(Operand);

#endif