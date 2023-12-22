#include "../../../lib/test/assert.h"
#include <stdio.h>
#include "../../../src/parser/find/find.h"
#include "../../../src/parser/byte/byte.h"
#include "../../../src/parser/slice/token_slice.h"

MODULAR_DESCRIBE(internal_find_byte_end_tests, {
    TEST("finds byte end for shorthand def [0]", {
        Token tokens[] = ARRAY(test_token(BYTE_START), test_token(BIT_OFF), test_token(BYTE_END));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        int end = find_byte_end(slice);

        ASSERT_INT_EQUALS(end, 2);
    })
    TEST("finds byte end for regular def [01010101]", {
        Token tokens[] = ARRAY(test_token(BYTE_START), test_token(BIT_OFF), test_token(BIT_ON),test_token(BIT_OFF), test_token(BIT_ON),test_token(BIT_OFF), test_token(BIT_ON),test_token(BIT_OFF), test_token(BIT_ON), test_token(BYTE_END));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        int end = find_byte_end(slice);

        ASSERT_INT_EQUALS(end, 9);
    })
});
