#include "scope.h"

Scope new_scope(char * name, Statements statements){
    ByteVector bytes = {NULL, 0};
    Scope scope = {name, bytes, statements};
    return scope;
}

Scope new_scope_copy(char * name, Statements statements, ByteVector stack){
    Scope scope = {name, stack, statements};
    return scope;
}
