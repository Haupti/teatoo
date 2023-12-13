#ifndef PARSER_ARGUMENT_UTILS_HEADER
#define PARSER_ARGUMENT_UTILS_HEADER
#include <stdlib.h>
#include "../byte/byte.h"

typedef struct Sequence {
    struct GenericOp * ops;
    size_t op_count;
} Sequence;

typedef struct Argument {
    Byte byte;
    int is_byte;
    Sequence sequence;
    int is_sequence;
    char * scope_name;
    int is_ref;
    int is_copy_ref;
} Argument;

typedef struct {
    Argument first;
    Argument second;
} ArgumentPair;


Argument new_sequence_argument(Sequence sequence);
Argument new_byte_argument(Byte byte);
Argument new_scope_ref_argument(char * scope_name);
Argument new_scope_copy_ref_argument(char * scope_name);

#endif
