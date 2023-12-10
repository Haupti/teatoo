#ifndef TOKEN_VECTOR_HEADER
#define TOKEN_VECTOR_HEADER
#include "token.h"

typedef struct {
    Token * arr;
    int len;
} TokenVector;

void print_tknvec(TokenVector vec);

#endif
