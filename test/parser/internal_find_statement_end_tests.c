
#include "../../lib/test/assert.h"
#include <iso646.h>
#include <stdio.h>
#include "../../src/parser/parser.h"
#include "../../src/parser/byte/byte.h"
#include "../../src/parser/slice/token_slice.h"
#include "internal_functions_header.h"

MODULAR_DESCRIBE(internal_find_statement_end_tests, {
    TEST("finds statement end by new line terminator", {
        Token tokens[] = ARRAY(new_token(IF), new_token(BYTE_START), new_token(BIT_OFF), new_token(BYTE_END), new_token(GRP_OPEN), new_token(TAKE), new_token(GRP_CLOSE) , new_token(TERM_NL), new_token(BIT_OFF));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        int end = find_statement_end(slice);

        ASSERT_INT_EQUALS(end, 6);
    })
    TEST("finds statement end by semicolon terminator", {
        Token tokens[] = ARRAY(new_token(IF), new_token(BYTE_START), new_token(BIT_OFF), new_token(BYTE_END), new_token(GRP_OPEN), new_token(TAKE), new_token(GRP_CLOSE) , new_token(TERM_SEM), new_token(BIT_OFF));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        int end = find_statement_end(slice);

        ASSERT_INT_EQUALS(end, 6);
    })
    TEST("finds statement end by sequence end", {
        Token tokens[] = ARRAY(new_token(IF), new_token(BYTE_START), new_token(BIT_OFF), new_token(BYTE_END), new_token(GRP_OPEN), new_token(TAKE), new_token(GRP_CLOSE) , new_token(GRP_CLOSE), new_token(BIT_OFF));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        int end = find_statement_end(slice);

        ASSERT_INT_EQUALS(end, 6);
    })
    TEST("finds statement end by scope end", {
        Token tokens[] = ARRAY(new_token(PUT), new_token(BYTE_START), new_token(BIT_OFF), new_token(BYTE_END), new_token(SCOPE_CLOSE));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        int end = find_statement_end(slice);

        ASSERT_INT_EQUALS(end, 3);
    })
});
