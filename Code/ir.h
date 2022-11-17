#ifndef IR_H
#define IR_HH

typedef struct Operand_* Operand;
struct Operand_{
    enum {VARIABLE, CONSTANT, TEMPORARY, ADDRESS} kind;
    union {
        int value;
    } u;
};

typedef struct InterCode_* InterCode;
struct InterCode_ {
    enum {ASSIGN, ADD, SUB, MUL, DIV, 
          LABEL, JUMP, BRANCH, 
          RETURN, DEC, ARG, CALL, PARAM,
          READ, WRITE} kind;
    union {
        struct {Operand right, left;} assign;
        struct {Operand result, op1, op2;} binop;
        struct {Operand result, op;} unop;
        int num;
        InterCode dest;
        Operand var;
        struct {InterCode dest; Operand op1, op2; enum {LSS, GRT, LEQ, GEQ, EQ, NEQ} relop;} branch;
        struct {Operand result; Function callee;} call;
    } u;
    InterCode prev, next;
};

typedef struct Function_* Function;
struct Function_ {
    char *name;
    ParamList params;
    InterCode entry;
    InterCode tail;
};

typedef struct Module_* Module;
struct Module_ {
    Function func;
    Module next;
};

typedef struct ArgList_* ArgList;
struct ArgList_ {
    Operand arg;
    ArgList next;
};

typedef struct ParamList_* ParamList;
struct ParamList_ {
    Operand param;
    ArgList next;
};

Operand newConstant(int x);
Operand newTemporary();
InterCode newIR();
InterCode newLabelIR();
InterCode newFunctionIR(char *name);
InterCode newAssignIR(Operand left, Operand right);
InterCode newUnaryIR(Operand result, Operand op, int kind);
InterCode newBinaryIR(Operand result, Operand op1, Operand op2, int kind);
InterCode newJumpIR(InterCode dest);
InterCode newBranchIR(InterCode dest, Operand op1, Operand op2, int relop);
InterCode newReturnIR(Operand var);
InterCode newReadIR(Operand var);
InterCode newWriteIR(Operand var);

Function newFunction(char *name);
void insert_IR(Function func, InterCode ir);

ArgList newArgList(Operand arg, ArgList succ_list);

#endif