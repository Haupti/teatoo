#include <stdio.h>
#include <stdlib.h>
#include "../../lexer/token.h"

void parse_err_at(char * msg, int pos, int line){
    printf("ERROR WHILE PARSING: in line %d at token %d: ", line, pos);
    printf("%s\n", msg);
    exit(EXIT_FAILURE);
}

void err_expected_token(Token token_expected, int index, int line){
    printf("ERROR WHILE PARSING: expected token %s in line %d at %d\n", TKN_STR(token_expected), line, index);
    exit(EXIT_FAILURE);
}

void err_unexpected_token(Token token_unexpected, int index){
    printf("ERROR WHILE PARSING: unexpected token %s in line %d at %d\n", TKN_STR(token_unexpected), token_unexpected.line_nr, index);
    exit(EXIT_FAILURE);
}

void err_at(char * msg, int index, int line){
    printf("ERROR WHILE PARSING: in line %d at %d: ", line, index);
    printf("%s\n",msg);
    exit(EXIT_FAILURE);
}
