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

GenericOp new_put(Byte byte){
    Op_PUT put = {new_byte_argument(byte)};
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

GenericOp new_and(Byte b1, Byte b2){
    Op_AND and = {new_byte_argument(b1), new_byte_argument(b2)};
    union Op op;
    op.op_and = and;
    GenericOp gen_op = {OT_AND, op};
    return gen_op;
}

GenericOp new_or(Byte b1, Byte b2){
    Op_OR or = {new_byte_argument(b1), new_byte_argument(b2)};
    union Op op;
    op.op_or = or;
    GenericOp gen_op = {OT_OR, op};
    return gen_op;
}

GenericOp new_xor(Byte b1, Byte b2){
    Op_XOR xor = {new_byte_argument(b1), new_byte_argument(b2)};
    union Op op;
    op.op_xor = xor;
    GenericOp gen_op = {OT_XOR, op};
    return gen_op;
}

GenericOp new_not(Byte byte){
    Op_NOT not = {new_byte_argument(byte)};
    union Op op;
    op.op_not = not;
    GenericOp gen_op = {OT_NOT, op};
    return gen_op;
}

GenericOp new_eq(Byte b1, Byte b2){
    Op_EQ eq = {new_byte_argument(b1), new_byte_argument(b2)};
    union Op op;
    op.op_eq = eq;
    GenericOp gen_op = {OT_EQ, op};
    return gen_op;
}

GenericOp new_neq(Byte b1, Byte b2){
    Op_NEQ neq = {new_byte_argument(b1), new_byte_argument(b2)};
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

GenericOp new_if(Argument condition, Argument operation){
    Op_IF iff = {condition, operation};
    union Op op;
    op.op_if = iff;
    GenericOp gen_op = {OT_IF, op};
    return gen_op;
}
