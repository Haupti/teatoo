#include "../lib/test/assert.h"
#include <stdlib.h>
#include <stdio.h>
#include "lexer/lexer_tests.h"
#include "parser/parse_byte_tests.h"
#include "parser/parse_module_tests.h"
#include "parser/find/internal_find_byte_end_tests.h"
#include "parser/find/internal_find_matching_sequence_end_tests.h"
#include "parser/find/internal_find_scope_end_tests.h"
#include "parser/find/internal_find_statement_end_tests.h"
#include "parser/find/find_argument_end_tests.h"
#include "parser/internal_parse_scope_tests.h"
#include "parser/internal_parse_sequence_arg_tests.h"
#include "parser/internal_parse_op_tests.h"
#include "parser/internal_collect_one_argument_tests.h"
#include "parser/verify/verify_tests.h"
#include "interpreter/exec_op_tests.h"
#include "interpreter/exec_arg_tests.h"
#include "interpreter/exec_scope_tests.h"
int main(){

    RUN_DESCRIBE_MODULE(lexer_tests);

    RUN_DESCRIBE_MODULE(internal_find_byte_end_tests);
    RUN_DESCRIBE_MODULE(internal_find_scope_end_tests);
    RUN_DESCRIBE_MODULE(internal_find_matching_sequence_end_tests);
    RUN_DESCRIBE_MODULE(internal_find_statement_end_tests);
    RUN_DESCRIBE_MODULE(find_argument_end_tests);

    RUN_DESCRIBE_MODULE(parse_byte_tests);
    RUN_DESCRIBE_MODULE(internal_collect_one_argument_tests);
    RUN_DESCRIBE_MODULE(internal_parse_sequence_arg_tests);
    RUN_DESCRIBE_MODULE(internal_parse_op_tests);
    RUN_DESCRIBE_MODULE(internal_parse_scope_tests);
    RUN_DESCRIBE_MODULE(parse_module_tests);
    RUN_DESCRIBE_MODULE(verify_tests);

    RUN_DESCRIBE_MODULE(exec_op_tests);
    RUN_DESCRIBE_MODULE(exec_arg_tests);
    RUN_DESCRIBE_MODULE(exec_scope_tests);

    return EXIT_SUCCESS;
}
