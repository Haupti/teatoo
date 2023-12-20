#ifndef INTERPRETER_RESULT_HEADER
#define INTERPRETER_RESULT_HEADER

#include "../../parser/byte/byte.h"

typedef struct Result {
    Byte byte;
    int is_byte;
    char * scope_name;
    int is_copy;
    int is_scope;
    int is_null;
} Result;

Result null_result();
Result byte_result(Byte byte);
Result scope_result(char * name, int is_copy);

#endif
