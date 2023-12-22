#ifndef INTERPRETER_RESULT_HEADER
#define INTERPRETER_RESULT_HEADER

#include "../../parser/scope/scope.h"
#include "../../parser/byte/byte.h"

typedef struct Result {
    Byte byte;
    int is_byte;
    char * scope_name;
    int is_copy;
    int is_scope;
    int is_null;
    int is_whole_scope;
    Scope scope;
} Result;

Result null_result();
Result byte_result(Byte byte);
Result scope_result(char * name, int is_copy);
Result whole_scope_result(Scope scope);

#endif
