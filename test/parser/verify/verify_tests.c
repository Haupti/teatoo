#include "../../../lib/test/assert.h"
#include <stdio.h>
#include "../../../src/parser/parser.h"
#include "../../../src/parser/byte/byte.h"
#include "../../../src/parser/slice/token_slice.h"
#include "internal_verify_header.h"

MODULAR_DESCRIBE(verify_tests, {
    TEST("verifies correct termination", {
        Token tokens[] = ARRAY(new_token(BYTE_START), new_token(BIT_OFF), new_token(BYTE_END), new_token(TERM_NL));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        int result = is_terminated(slice, 3);

        ASSERT_INT_EQUALS(result, 1)
    })
    TEST("verifies incorrect termination", {
        Token tokens[] = ARRAY(new_token(BYTE_START), new_token(BIT_OFF), new_token(BYTE_END), new_token(TERM_NL));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        int result = is_terminated(slice, 1);

        ASSERT_INT_EQUALS(result, 0)
    })
    TEST("verifies slice contains only one argument", {
        Token tokens[] = ARRAY(new_token(BYTE_START), new_token(BIT_OFF), new_token(BYTE_END), new_token(TERM_NL));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        int result = has_only_one_argument(slice);

        ASSERT_INT_EQUALS(result, 1)
    })
    TEST("finds error in one argument slice", {
        Token tokens[] = ARRAY(new_token(BYTE_START), new_token(BIT_OFF), new_token(BYTE_END), new_identifier_token("peter"), new_token(TERM_NL));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        int result = has_only_one_argument(slice);

        ASSERT_INT_EQUALS(result, 0)
    })
    TEST("finds problem: has two args", {
        Token tokens[] = ARRAY(new_token(BYTE_START), new_token(BIT_OFF), new_token(BYTE_END), new_token(GRP_OPEN), new_token(TAKE), new_token(GRP_CLOSE), new_token(TERM_NL));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        int result = has_only_one_argument(slice);

        ASSERT_INT_EQUALS(result, 0)
    })
    TEST("verifies slice contains only two argument, termination is end of slice", {
        Token tokens[] = ARRAY(new_token(BYTE_START), new_token(BIT_OFF), new_token(BYTE_END), new_token(GRP_OPEN), new_token(TAKE), new_token(GRP_CLOSE));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        int result = has_only_two_arguments(slice);

        ASSERT_INT_EQUALS(result, 1)
    })
    TEST("verifies slices contains only two argumetns, termination is scope close", {
        Token tokens[] = ARRAY(new_token(BYTE_START), new_token(BIT_OFF), new_token(BYTE_END), new_token(GRP_OPEN), new_token(TAKE), new_token(GRP_CLOSE), new_token(SCOPE_CLOSE));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        int result = has_only_two_arguments(slice);

        ASSERT_INT_EQUALS(result, 1)
    })
    TEST("finds problem: has three arguments", {
        Token tokens[] = ARRAY(new_token(BYTE_START), new_token(BIT_OFF), new_token(BYTE_END), new_token(GRP_OPEN), new_token(TAKE), new_token(GRP_CLOSE), new_token(GRP_OPEN), new_token(PUT), new_token(BYTE_START), new_token(BIT_OFF), new_token(BYTE_END), new_token(GRP_CLOSE));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        int result = has_only_two_arguments(slice);

        ASSERT_INT_EQUALS(result, 0)
    })
})
