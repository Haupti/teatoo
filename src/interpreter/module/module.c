#include "../../parser/parser.h"

Module new_module(ScopeVector scopes, Op_EXEC entrypoint){
    Module module = {scopes, entrypoint, 1};
    return module;
}

Module new_no_entrypoint_module(ScopeVector scopes){
    Module module = {scopes, (Op_EXEC) {}, 0};
    return module;
}
