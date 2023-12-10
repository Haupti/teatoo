#include "token.h"
#include "../utils/free.h"
#include <string.h>
#include <stdio.h>

Token new_token(TokenType type){
    Token token = {type, NULL};
    return token;
}

Token new_identifier_token(char * to_copy){
    char * name = malloc(strlen(to_copy) * sizeof(char));
    strcpy(name, to_copy);
    Token token = {IDENTIFIER, name};
    return token;
}

void destroy_token(Token token){
    free_s(token.name);
}
