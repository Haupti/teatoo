#include "../parser/parser.h"
#include <stdlib.h>
#include <stdio.h>

void error_on_byte_expect_sequence(Argument arg){
    if(arg.is_byte){
        printf("ERROR: expected a sequence, but received a byte");
        exit(EXIT_FAILURE);
    }
}

int iterpret_exec(Op_EXEC exec){
    error_on_byte_expect_sequence(exec.first);
    return EXIT_FAILURE;
}

int interpret(Module mod){
    if(mod.has_entrypoint){
        interpret_exec(mod.entrypoint);
    }
    return EXIT_SUCCESS;
}
