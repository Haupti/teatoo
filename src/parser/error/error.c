#include <stdio.h>
#include <stdlib.h>
#include "../../lexer/token.h"

void parse_err_at(char * msg, int pos){
    printf("ERROR WHILE PARSING: at token %d: ", pos);
    printf("%s\n", msg);
    exit(EXIT_FAILURE);
}

void err_expected_token(Token token_expected, int index){
    printf("ERROR WHILE PARSING: expected token %s at %d\n", TKN_STR(token_expected), index);
    exit(EXIT_FAILURE);
}

void err_unexpected_token(Token token_unexpected, int index){
    printf("ERROR WHILE PARSING: unexpected token %s at %d\n", TKN_STR(token_unexpected), index);
    exit(EXIT_FAILURE);
}

void err_at(char * msg, int index){
    printf("ERROR WHILE PARSING: at %d: ", index);
    printf("%s\n",msg);
    exit(EXIT_FAILURE);
}
