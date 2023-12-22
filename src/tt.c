#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpreter/interpreter.h"
#include "lexer/lexer.h"
#include "lexer/token.h"
#include "lexer/token_vector.h"
#include "parser/parser.h"



char tt(char * program){
    TokenVector tokens = create_tokens(program, strlen(program));
    Module module = create_parse_module(tokens);
    free(tokens.arr);
    tokens.arr = NULL;

    interpret(&module);

    return '\0';
}
