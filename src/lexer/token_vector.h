#ifndef TOKEN_VECTOR_HEADER
#define TOKEN_VECTOR_HEADER

#include "token.h"
#include <stdlib.h>

typedef struct {
    Token * arr;
    int len;
} TokenVector;

void print_tknvec(TokenVector vec);
Token get_token(TokenVector vec, int index);
TokenVector new_token_vector(Token * arr, size_t len);

#endif
