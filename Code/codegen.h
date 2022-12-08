#ifndef CODEGEN_H
#define CODEGEN_H

#include <stdio.h>
#include <assert.h>
#include "ir.h"

void gencode_Module(Module module);
void gencode_Function(Function func);
void gencode_IR(InterCode ir);
void gencode_init();



#endif