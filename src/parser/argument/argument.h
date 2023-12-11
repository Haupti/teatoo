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
} Argument;

typedef struct {
    Argument first;
    Argument second;
} ArgumentPair;


Argument new_sequence_argument(Sequence sequence);
Argument new_byte_argument(Byte byte);

#endif
