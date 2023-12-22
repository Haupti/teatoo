#include "instruction.h"
#include <stdlib.h>

Statements new_statements(GenericOp * statements, size_t len){
    Statements s = {statements, len};
    return s;
}

GenericOp new_peek(){
    Op_PEEK peek = {};
    union Op op;
    op.op_peek = peek;
    GenericOp gen_op = {OT_PEEK, op};
    return gen_op;
}

GenericOp new_put(Argument arg){
    Op_PUT put = {arg};
    union Op op;
    op.op_put = put;
    GenericOp gen_op = {OT_PUT, op};
    return gen_op;
}

GenericOp new_take(){
    Op_TAKE take = {};
    union Op op;
    op.op_take = take;
    GenericOp gen_op = {OT_TAKE, op};
    return gen_op;
}

GenericOp new_is_empty(){
    Op_IS_EMPTY is_empty = {};
    union Op op;
    op.op_is_empty = is_empty;
    GenericOp gen_op = {OT_IS_EMPTY, op};
    return gen_op;
}

GenericOp new_and(Argument arg1, Argument arg2){
    Op_AND and = {arg1, arg2};
    union Op op;
    op.op_and = and;
    GenericOp gen_op = {OT_AND, op};
    return gen_op;
}

GenericOp new_or(Argument arg1, Argument arg2){
    Op_OR or = {arg1, arg2};
    union Op op;
    op.op_or = or;
    GenericOp gen_op = {OT_OR, op};
    return gen_op;
}

GenericOp new_xor(Argument arg1, Argument arg2){
    Op_XOR xor = {arg1, arg2};
    union Op op;
    op.op_xor = xor;
    GenericOp gen_op = {OT_XOR, op};
    return gen_op;
}

GenericOp new_not(Argument arg){
    Op_NOT not = {arg};
    union Op op;
    op.op_not = not;
    GenericOp gen_op = {OT_NOT, op};
    return gen_op;
}

GenericOp new_eq(Argument arg1, Argument arg2){
    Op_EQ eq = {arg1, arg2};
    union Op op;
    op.op_eq = eq;
    GenericOp gen_op = {OT_EQ, op};
    return gen_op;
}

GenericOp new_neq(Argument arg1, Argument arg2){
    Op_NEQ neq = {arg1, arg2};
    union Op op;
    op.op_neq = neq;
    GenericOp gen_op = {OT_NEQ, op};
    return gen_op;
}

GenericOp new_return(Argument arg){
    Op_RETURN ret = {arg};
    union Op op;
    op.op_return = ret;
    GenericOp gen_op = {OT_RETURN, op};
    return gen_op;
}

GenericOp new_exec(Argument arg){
    Op_EXEC exec = {arg};
    union Op op;
    op.op_exec = exec;
    GenericOp gen_op = {OT_EXEC, op};
    return gen_op;
}

GenericOp new_stack(Argument arg1, Argument arg2){
    Op_STACK stack = {arg1, arg2};
    union Op op;
    op.op_stack = stack;
    GenericOp gen_op = {OT_STACK, op};
    return gen_op;
}

GenericOp new_pow(Argument arg){
    Op_POW pow = {arg};
    union Op op;
    op.op_pow = pow;
    GenericOp gen_op = {OT_POW, op};
    return gen_op;
}

GenericOp new_out(Argument arg){
    Op_OUT out = {arg};
    union Op op;
    op.op_out = out;
    GenericOp gen_op = {OT_OUT, op};
    return gen_op;
}

GenericOp new_outnum(Argument arg){
    Op_OUTNUM outnum = {arg};
    union Op op;
    op.op_outnum = outnum;
    GenericOp gen_op = {OT_OUTNUM, op};
    return gen_op;
}

GenericOp new_outchar(Argument arg){
    Op_OUTCHAR outchar = {arg};
    union Op op;
    op.op_outchar = outchar;
    GenericOp gen_op = {OT_OUTCHAR, op};
    return gen_op;
}

GenericOp new_is_null(Argument arg){
    Op_IS_NULL is_null = {arg};
    union Op op;
    op.op_is_null = is_null;
    GenericOp gen_op = {OT_IS_NULL, op};
    return gen_op;
}

GenericOp new_if(Argument condition, Argument operation){
    Op_IF iff = {condition, operation};
    union Op op;
    op.op_if = iff;
    GenericOp gen_op = {OT_IF, op};
    return gen_op;
}
