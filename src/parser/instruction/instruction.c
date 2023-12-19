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
