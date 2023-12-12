#include <stdio.h>
#include "token_vector.h"
#include <stdlib.h>

void print_tknvec(TokenVector vec){
    printf("VECTOR LENGTH: %d\n", vec.len);
    for(int i =0; i<vec.len; i++){
        printf("%s\n", TKN_STR(vec.arr[i]));
    }
}

Token get_token(TokenVector vec, int index){
    if(index >= vec.len){
        printf("ERROR WHILE PARSING: missing a token at %d\n", index);
        exit(EXIT_FAILURE);
    }
    Token token = vec.arr[index];
    return token;
}

TokenVector new_token_vector(Token * arr, size_t len){
    TokenVector vec = {arr, len};
    return vec;
}
