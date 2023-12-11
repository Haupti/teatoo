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
    OT_OUT,
    OT_OUTCHAR,
    OT_OUTNUM
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

typedef struct Op_NOT {
    struct Argument first;
} Op_NOT;

typedef struct Op_TAKE {} Op_TAKE;

typedef struct Op_PUT {
    struct Argument first;
} Op_PUT;

typedef struct Op_PEEK {} Op_PEEK;

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
   struct Op_OUT op_out;
   struct Op_OUTCHAR op_outchar;
   struct Op_OUTNUM op_outnum;
};

typedef struct GenericOp {
    OpType type;
    union Op op;
} GenericOp;

typedef struct Statements {
    GenericOp * statements;
    int statements_len;
} Statements;

#endif
