#ifndef PARSER_HEADER
#define PARSER_HEADER

#include "scope/scope.h"

#include "instruction/instruction.h"
typedef struct Module {
    ScopeVector scopes;
    Op_EXEC entrypoint;
} Module;


Module parse_module(TokenVector vec);

#endif
