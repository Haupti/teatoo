#ifndef INTERPRETER_RESULT_HEADER
#define INTERPRETER_RESULT_HEADER

#include "../../parser/byte/byte.h"

typedef struct Result {
    Byte byte;
    int is_byte;
    int is_null;
} Result;

Result null_result();
Result byte_result(Byte byte);

#endif
