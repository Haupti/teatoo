#include <stdio.h>
#include <stdlib.h>

void lex_err(char * msg, char c, int pos, int line){
    printf("ERROR WHILE PARSING: unexpected character %c in line %d at %d", c, line, pos);
    exit(EXIT_FAILURE);
}
