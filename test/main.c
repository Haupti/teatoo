#include "../lib/test/assert.h"
#include <stdlib.h>
#include <stdio.h>
#include "lexer/lexer_tests.h"
#include "parser/parse_byte_tests.h"

int main(){
    RUN_DESCRIBE_MODULE(lexer_tests);
    RUN_DESCRIBE_MODULE(parse_byte_tests);
    return EXIT_SUCCESS;
}
