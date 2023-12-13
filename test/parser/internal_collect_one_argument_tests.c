#include "../../lib/test/assert.h"
#include <stdio.h>
#include "../../src/parser/parser.h"
#include "../../src/parser/byte/byte.h"
#include "../../src/parser/slice/token_slice.h"
#include "internal_functions_header.h"

MODULAR_DESCRIBE(internal_collect_one_argument_tests, {
    TEST("collects one argument which is a byte", {
        Token tokens[] = ARRAY(new_token(BYTE_START), new_token(BIT_OFF), new_token(BYTE_END), new_token(TERM_NL));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        Argument arg = collect_one_argument(slice);

        ASSERT_INT_EQUALS(arg.is_byte, 1)
    })
    TEST("collects one argument which is a sequence", {
        Token tokens[] = ARRAY(new_token(GRP_OPEN), new_token(PUT), new_token(BYTE_START), new_token(BIT_OFF), new_token(BYTE_END), new_token(GRP_CLOSE));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        Argument arg = collect_one_argument(slice);

        ASSERT_INT_EQUALS(arg.is_sequence, 1)
    })
    TEST("collects one argument which is a scope ref", {
        Token tokens[] = ARRAY(new_identifier_token("peter"));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        Argument arg = collect_one_argument(slice);

        ASSERT_INT_EQUALS(arg.is_ref, 1)
    })
    TEST("collects one argument which is a scope copy ref", {
        Token tokens[] = ARRAY( new_token(COPY), new_identifier_token("peter"));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        Argument arg = collect_one_argument(slice);

        ASSERT_INT_EQUALS(arg.is_copy_ref, 1)
    })
});

