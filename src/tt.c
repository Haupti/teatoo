#include <stdio.h>
#include <string.h>
#include "lexer/lexer.h"
#include "lexer/token.h"



char tt(char * program){
    TokenVector tokens = create_tokens(program, strlen(program));

    return '\0';
}
