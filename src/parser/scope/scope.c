#include "scope.h"

Scope new_scope(char * name, Statements statements){
    ByteVector bytes = {NULL, 0};
    Scope scope = {name, bytes, statements};
    return scope;
}
