#include "token.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

Token new_token(TokenType type, int line_nr){
    Token token = {type, line_nr, NULL};
    return token;
}

Token test_token(TokenType type){
    return new_token(type, 0);
}

Token new_identifier_token(char * to_copy, int line_nr){
    char * name = malloc(strlen(to_copy) * sizeof(char));
    strcpy(name, to_copy);
    Token token = {IDENTIFIER, line_nr, name};
    return token;
}
Token test_identifier_token(char * to_copy){
    return new_identifier_token(to_copy, 0);
}

void destroy_token(Token token){
    free(token.name);
    token.name = NULL;
}
