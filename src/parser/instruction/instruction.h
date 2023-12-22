#ifndef INSTRUCTION_HEADER
#define INSTRUCTION_HEADER

#include "../byte/byte.h"
#include <stdlib.h>
#include "../argument/argument.h"

typedef enum {
    OT_IF,
    OT_EQ,
    OT_NEQ,
    OT_OR,
    OT_AND,
    OT_XOR,
    OT_NOT,
    OT_TAKE,
    OT_PUT,
    OT_PEEK,
    OT_RETURN,
    OT_EXEC,
    OT_STACK,
    OT_OUT,
    OT_OUTCHAR,
    OT_OUTNUM,
    OT_POW,
    OT_IS_EMPTY,
    OT_IS_NULL,
} OpType;


struct GenericOp;


typedef struct Op_IF {
    struct Argument condition;
    struct Argument operation;
} Op_IF;

typedef struct Op_EQ {
    struct Argument first;
    struct Argument second;
} Op_EQ;

typedef struct Op_NEQ {
    struct Argument first;
    struct Argument second;
} Op_NEQ;

typedef struct Op_OR {
    struct Argument first;
    struct Argument second;
} Op_OR;

typedef struct Op_AND {
    struct Argument first;
    struct Argument second;
} Op_AND;

typedef struct Op_XOR {
    struct Argument first;
    struct Argument second;
} Op_XOR;

typedef struct Op_STACK {
    struct Argument first;
    struct Argument second;
} Op_STACK;

typedef struct Op_NOT {
    struct Argument first;
} Op_NOT;

typedef struct Op_PUT {
    struct Argument first;
} Op_PUT;

typedef struct Op_RETURN {
    struct Argument first;
} Op_RETURN;

typedef struct Op_EXEC {
    struct Argument first;
} Op_EXEC;

typedef struct Op_OUT {
    struct Argument first;
} Op_OUT;

typedef struct Op_OUTCHAR {
    struct Argument first;
} Op_OUTCHAR;

typedef struct Op_OUTNUM {
    struct Argument first;
} Op_OUTNUM;

typedef struct Op_POW {
    struct Argument first;
} Op_POW;

typedef struct Op_IS_NULL {
    struct Argument first;
} Op_IS_NULL;

typedef struct Op_IS_EMPTY {} Op_IS_EMPTY;

typedef struct Op_TAKE {} Op_TAKE;

typedef struct Op_PEEK {} Op_PEEK;

union Op {
   struct Op_IF op_if;
   struct Op_EQ op_eq;
   struct Op_NEQ op_neq;
   struct Op_OR op_or;
   struct Op_AND op_and;
   struct Op_XOR op_xor;
   struct Op_NOT op_not;
   struct Op_TAKE op_take;
   struct Op_PUT op_put;
   struct Op_PEEK op_peek;
   struct Op_RETURN op_return;
   struct Op_EXEC op_exec;
   struct Op_STACK op_stack;
   struct Op_OUT op_out;
   struct Op_OUTCHAR op_outchar;
   struct Op_OUTNUM op_outnum;
   struct Op_POW op_pow;
   struct Op_IS_EMPTY op_is_empty;
   struct Op_IS_NULL op_is_null;
};

typedef struct GenericOp {
    OpType type;
    union Op op;
} GenericOp;

typedef struct Statements {
    GenericOp * statements;
    int statements_len;
} Statements;

Statements new_statements(GenericOp * statements, size_t len);
GenericOp new_peek();
GenericOp new_is_empty();
GenericOp new_put(Argument arg);
GenericOp new_take();
GenericOp new_and(Argument arg1, Argument arg2);
GenericOp new_or(Argument arg1, Argument arg2);
GenericOp new_xor(Argument arg1, Argument arg2);
GenericOp new_not(Argument arg);
GenericOp new_eq(Argument arg1, Argument arg2);
GenericOp new_neq(Argument arg1, Argument arg2);
GenericOp new_return(Argument arg);
GenericOp new_exec(Argument arg);
GenericOp new_stack(Argument arg1, Argument arg2);
GenericOp new_pow(Argument arg);
GenericOp new_is_null(Argument arg);
GenericOp new_out(Argument arg);
GenericOp new_outchar(Argument arg);
GenericOp new_outnum(Argument arg);
GenericOp new_if(Argument condition, Argument operation);

#endif
