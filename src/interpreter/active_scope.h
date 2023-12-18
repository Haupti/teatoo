#ifndef INTERPRETER_ACTIVE_SCOPE_HEADER
#define INTERPRETER_ACTIVE_SCOPE_HEADER

#include "result/result.h"
#include "../parser/scope/scope.h"

typedef struct ActiveScope {
    char * name;
    ByteVector stack;
    Statements statements;
    Result return_result;
    int is_returned;
} ActiveScope;

#endif
