#ifndef SCOPE_HEADER
#define SCOPE_HEADER

#include "../../lexer/token_vector.h"
#include "../../lexer/token.h"
#include "../instruction/instruction.h"
#include "../byte/byte.h"

typedef struct ByteVector {
    Byte * arr;
    int len;
} ByteVector;

struct Scope;

typedef struct ScopeVector {
    struct Scope * arr;
    int len;
} ScopeVector;

typedef struct Scope {
    char * name;
    ByteVector stack;
    Statements statements;
} Scope;

Scope new_scope(char * name, Statements statements);
Scope new_scope_copy(char * name, Statements statements, ByteVector stack);

#endif
