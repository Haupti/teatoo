#include <stdio.h>
#include <string.h>
#include "lexer/lexer.h"
#include "lexer/token.h"
#include "parser/parser.h"
#include "utils/free.h"



char tt(char * program){
    TokenVector tokens = create_tokens(program, strlen(program));
    Module module = create_parse_module(tokens);
    free_s(tokens.arr);

    return '\0';
}
