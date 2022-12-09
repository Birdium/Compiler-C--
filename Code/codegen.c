#include <assert.h>
#include "codegen.h"
#include "string.h"
#include "stdlib.h"

FILE *ostream;

char* regName[] = {
	"$zero",
	"$at",
	"$v0","$v1",
	"$a0","$a1","$a2","$a3",
	"$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7",
	"$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7",
	"$t8","$t9",
	"$k0","$k1",
	"$gp",
	"$sp",
	"$fp",
	"$ra"
};


void reg_alloc(Module module) {
    FunctionList flist = module->func_list;
    while (flist) {
        Function func = flist->func;
        int offset = 0;
        InterCode ir = func->entry;
        while (ir) {
            Operand lvalue = getLValue(ir);
            if (lvalue && lvalue->sp_offset == -1 && ir->kind != PARAM 
                       && lvalue->kind != ADDRESS && lvalue->kind != REFERENCE) {
                lvalue->sp_offset = offset;
                if (ir->kind == DEC) {
                    offset += ir->u.dec.size;
                }
                else {
                    offset += 4;
                }
            }
            ir = ir->next;
        }
        OpList params = func->params;
        int param_offset = 4;
        while (params) {
            Operand param = params->op;
            assert(param->kind == VARIABLE);
            param->sp_offset = offset + param_offset;
            param_offset += 4;
            // offset += 4;
            params = params->tail; 
        }
        func->stack_size = offset;
        assert(offset + 4 + param_offset <= 32767);
        flist = flist->next;
    }
}

int arg_cnt = 0;

void gencode_Module(Module module) {
    gencode_init();
    reg_alloc(module);
    FunctionList flist = module->func_list;
    while (flist) {
        gencode_Function(flist->func);
        flist = flist->next;
        fprintf(ostream, "\n");
    }
}

Function cur_func;

void gencode_Function(Function func) {
    cur_func = func;
    InterCode ir = func->entry;
    while (ir) {
        gencode_IR(ir);
        ir = ir->next;
    }
}

// only RValue
void mem2reg_unary(RegNum reg, Operand op){
    if (op->kind == CONSTANT) {
        fprintf(ostream, "\tli %s, %d\n", regName[reg], op->u.value);
    }
    else if (op->kind == ADDRESS) { // &t1
        fprintf(ostream, "\taddi %s, %s, %d\n", regName[reg], regName[SP], op->u.pt->sp_offset);
    }
    else if (op->kind == REFERENCE) { // *t1
        fprintf(ostream, "\tlw %s, %d(%s)\n", regName[reg], op->u.pt->sp_offset, regName[SP]);
        fprintf(ostream, "\tlw %s, %d(%s)\n", regName[reg], 0, regName[reg]);
    }
    else {
        fprintf(ostream, "\tlw %s, %d(%s)\n", regName[reg], op->sp_offset, regName[SP]);
    }
}

void mem2reg_binary(RegNum reg1, RegNum reg2, Operand op1, Operand op2){
    mem2reg_unary(reg1, op1);
    mem2reg_unary(reg2, op2);
}

void gencode_IR(InterCode ir){
    if (ir->kind == LABEL) {
        fprintf(ostream, "label%d:\n", ir->u.num);
    }
    else if (ir->kind == FUNCT) {
        fprintf(ostream, "%s:\n", ir->u.name);
        fprintf(ostream, "\taddi %s, %s, %d\n", regName[SP], regName[SP], -cur_func->stack_size);
    }
    else if (ir->kind == ASSIGN) {
        Operand left = ir->u.assign.left;
        Operand right = ir->u.assign.right;
        mem2reg_unary(T1, right);
        if (left->kind == REFERENCE) {
            fprintf(ostream, "\tlw %s, %d(%s)\n", regName[T2], left->u.pt->sp_offset, regName[SP]);
            fprintf(ostream, "\tsw %s, %d(%s)\n", regName[T1], 0, regName[T2]);
        }
        else {
            fprintf(ostream, "\tsw %s, %d(%s)\n", regName[T1], left->sp_offset, regName[SP]);
        }
    }
    else if (ir->kind == ADD) {
        Operand result = ir->u.binop.result;
        Operand op1 = ir->u.binop.op1;
        Operand op2 = ir->u.binop.op2;
        mem2reg_binary(T1, T2, op1, op2);
        fprintf(ostream, "\tadd %s, %s, %s\n", regName[T0], regName[T1], regName[T2]);
        fprintf(ostream, "\tsw %s, %d(%s)\n", regName[T0], result->sp_offset, regName[SP]);
    }
    else if (ir->kind == SUB) {
        Operand result = ir->u.binop.result;
        Operand op1 = ir->u.binop.op1;
        Operand op2 = ir->u.binop.op2;
        mem2reg_binary(T1, T2, op1, op2);
        fprintf(ostream, "\tsub %s, %s, %s\n", regName[T0], regName[T1], regName[T2]);
        fprintf(ostream, "\tsw %s, %d(%s)\n", regName[T0], result->sp_offset, regName[SP]);
    }
    else if (ir->kind == MUL) {
        Operand result = ir->u.binop.result;
        Operand op1 = ir->u.binop.op1;
        Operand op2 = ir->u.binop.op2;
        mem2reg_binary(T1, T2, op1, op2);
        fprintf(ostream, "\tmul %s, %s, %s\n", regName[T0], regName[T1], regName[T2]);
        fprintf(ostream, "\tsw %s, %d(%s)\n", regName[T0], result->sp_offset, regName[SP]);
    }
    else if (ir->kind == DIV) {
        Operand result = ir->u.binop.result;
        Operand op1 = ir->u.binop.op1;
        Operand op2 = ir->u.binop.op2;
        mem2reg_binary(T1, T2, op1, op2);
        fprintf(ostream, "\tdiv %s, %s\n", regName[T1], regName[T2]);
        fprintf(ostream, "\tmflo %s\n", regName[T0]);
        fprintf(ostream, "\tsw %s, %d(%s)\n", regName[T0], result->sp_offset, regName[SP]);
    }
    else if (ir->kind == JUMP) {
        fprintf(ostream, "\tj label%d\n", ir->u.dest->u.num);
    }
    else if (ir->kind == BRANCH) {
        Operand x = ir->u.branch.op1;
        Operand y = ir->u.branch.op2;
        mem2reg_binary(T1, T2, x, y);
        if (ir->u.branch.relop == EQ) {
            fprintf(ostream, "\tbeq ");
        }
        else if (ir->u.branch.relop == NEQ) {
            fprintf(ostream, "\tbne ");
        }
        else if (ir->u.branch.relop == GRT) {
            fprintf(ostream, "\tbgt ");
        }
        else if (ir->u.branch.relop == LSS) {
            fprintf(ostream, "\tblt ");
        }
        else if (ir->u.branch.relop == GEQ) {
            fprintf(ostream, "\tbge ");
        }
        else if (ir->u.branch.relop == LEQ) {
            fprintf(ostream, "\tble ");
        }
        fprintf(ostream, "%s, %s, ", regName[T1], regName[T2]);
        fprintf(ostream, "label%d\n", ir->u.branch.dest->u.num);
    }
    else if (ir->kind == RETURN) {
        Operand var = ir->u.var;
        mem2reg_unary(V0, var);
        fprintf(ostream, "\taddi %s, %s, %d\n", regName[SP], regName[SP], cur_func->stack_size);
        fprintf(ostream, "\tjr %s\n", regName[RA]);
    }
    else if (ir->kind == DEC) {
    }
    else if (ir->kind == ARG) {
        arg_cnt += 4;
        Operand var = ir->u.var;
        mem2reg_unary(T1, var);
        fprintf(ostream, "\tsw %s, %d($sp)\n", regName[T1], -arg_cnt);
    }
    else if (ir->kind == CALL) {
        Operand result = ir->u.call.result;
        fprintf(ostream, "\taddi $sp, $sp, %d\n", -4-arg_cnt);
        fprintf(ostream, "\tsw $ra, 0($sp)\n");
        fprintf(ostream, "\tjal %s\n", ir->u.call.callee->name);
        fprintf(ostream, "\tlw $ra, 0($sp)\n");
        fprintf(ostream, "\taddi $sp, $sp, %d\n", 4+arg_cnt);
        fprintf(ostream, "\tsw %s, %d(%s)\n", regName[V0], result->sp_offset, regName[SP]);
        arg_cnt = 0;
    }
    else if (ir->kind == PARAM) {
    }
    else if (ir->kind == READ) {
        Operand var = ir->u.var;
        fprintf(ostream, "\taddi $sp, $sp, -4\n");
        fprintf(ostream, "\tsw $ra, 0($sp)\n");
        fprintf(ostream, "\tjal read\n");
        fprintf(ostream, "\tlw $ra, 0($sp)\n");
        fprintf(ostream, "\taddi $sp, $sp, 4\n");
        fprintf(ostream, "\tsw %s, %d(%s)\n", regName[V0], var->sp_offset, regName[SP]);
    }
    else if (ir->kind == WRITE) {
        Operand var = ir->u.var;
        mem2reg_unary(A0, var);
        fprintf(ostream, "\taddi $sp, $sp, -4\n");
        fprintf(ostream, "\tsw $ra, 0($sp)\n");
        fprintf(ostream, "\tjal write\n");
        fprintf(ostream, "\tlw $ra, 0($sp)\n");
        fprintf(ostream, "\taddi $sp, $sp, 4\n");
    }
}

void gencode_init(){
    ostream = stdout;
    fprintf(ostream,".data\n");
    fprintf(ostream,"_prompt: .asciiz \"Enter an integer:\"\n");
    fprintf(ostream,"_ret: .asciiz \"\\n\"\n");
    fprintf(ostream,".globl main\n");
    fprintf(ostream,".text\n");
    fprintf(ostream,"read:\n");
    fprintf(ostream,"\tli $v0, 4\n");
    fprintf(ostream,"\tla $a0, _prompt\n");
    fprintf(ostream,"\tsyscall\n");
    fprintf(ostream,"\tli $v0, 5\n");
    fprintf(ostream,"\tsyscall\n");
    fprintf(ostream,"\tjr $ra\n");
    fprintf(ostream,"\n");
    fprintf(ostream,"write:\n");
    fprintf(ostream,"\tli $v0, 1\n");
    fprintf(ostream,"\tsyscall\n");
    fprintf(ostream,"\tli $v0, 4\n");
    fprintf(ostream,"\tla $a0, _ret\n");
    fprintf(ostream,"\tsyscall\n");
    fprintf(ostream,"\tmove $v0, $0\n");
    fprintf(ostream,"\tjr $ra\n\n");
}