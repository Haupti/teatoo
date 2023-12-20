#ifndef INTERPRETER_MODULE_HEADER
#define INTERPRETER_MODULE_HEADER

#include "../../parser/parser.h"

Module new_module(ScopeVector scopes, Op_EXEC entrypoint);
Module new_no_entrypoint_module(ScopeVector scopes);

#endif
