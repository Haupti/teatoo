#include <stdio.h>
#include <stdlib.h>

void lex_err(char * msg, char c, int pos){
    printf("ERROR WHILE PARSING: unexpected character %c at %d", c, pos);
    exit(EXIT_FAILURE);
}
