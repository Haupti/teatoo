#include "../../../lib/test/assert.h"
#include <stdio.h>
#include "../../../src/parser/find/find.h"
#include "../../../src/parser/slice/token_slice.h"

MODULAR_DESCRIBE(internal_find_statement_end_tests, {
    TEST("finds statement end by new line terminator", {
        Token tokens[] = ARRAY(test_token(IF), test_token(BYTE_START), test_token(BIT_OFF), test_token(BYTE_END), test_token(GRP_OPEN), test_token(TAKE), test_token(GRP_CLOSE) , test_token(TERM_NL), test_token(BIT_OFF));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        int end = find_statement_end(slice);

        ASSERT_INT_EQUALS(end, 6);
    })
    TEST("finds statement end by semicolon terminator", {
        Token tokens[] = ARRAY(test_token(IF), test_token(BYTE_START), test_token(BIT_OFF), test_token(BYTE_END), test_token(GRP_OPEN), test_token(TAKE), test_token(GRP_CLOSE) , test_token(TERM_SEM), test_token(BIT_OFF));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        int end = find_statement_end(slice);

        ASSERT_INT_EQUALS(end, 6);
    })
    TEST("finds statement end by sequence end", {
        Token tokens[] = ARRAY(test_token(IF), test_token(BYTE_START), test_token(BIT_OFF), test_token(BYTE_END), test_token(GRP_OPEN), test_token(TAKE), test_token(GRP_CLOSE) , test_token(GRP_CLOSE), test_token(BIT_OFF));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        int end = find_statement_end(slice);

        ASSERT_INT_EQUALS(end, 6);
    })
    TEST("finds statement end by scope end", {
        Token tokens[] = ARRAY(test_token(PUT), test_token(BYTE_START), test_token(BIT_OFF), test_token(BYTE_END), test_token(SCOPE_CLOSE));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        int end = find_statement_end(slice);

        ASSERT_INT_EQUALS(end, 3);
    })
});
