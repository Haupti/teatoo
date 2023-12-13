#include "../parser/parser.h"
#include <stdlib.h>
#include <stdio.h>

void error_on_byte_expect_sequence(Argument arg){
    if(arg.is_byte){
        printf("ERROR: expected a sequence, but received a byte");
        exit(EXIT_FAILURE);
    }
    // EXEC expects a scope, and only a scope
    // that means a argument must then be a scope ref or a scope copy ref i.e. e.g. "a: {}; EXEC a" or "a:{}; EXEC $a"
}


int interpret_exec(Op_EXEC exec){
    error_on_byte_expect_sequence(exec.first);
    return EXIT_FAILURE;
}

int interpret(Module mod){
    if(mod.has_entrypoint){
        interpret_exec(mod.entrypoint);
    }
    return EXIT_SUCCESS;
}
