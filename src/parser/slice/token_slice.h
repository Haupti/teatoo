#ifndef TOKEN_SLICE_HEADER
#define TOKEN_SLICE_HEADER

#include <stdlib.h>
#include "../../lexer/token.h"

typedef struct {
    Token * arr;
    int start;
    int end;
} TokenSlice;

TokenSlice new_token_slice(Token * tokens, size_t len);
Token head_slice(TokenSlice slice);
#endif
