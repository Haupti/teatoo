#include "token_slice.h"
#include "../../lexer/token.h"

TokenSlice new_token_slice(Token * tokens, size_t len){
    TokenSlice slice = {tokens, 0, len-1};
    return slice;
}

Token head_slice(TokenSlice slice){
    return slice.arr[slice.start];
}
