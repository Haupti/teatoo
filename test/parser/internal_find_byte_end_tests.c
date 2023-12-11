#include "../../lib/test/assert.h"
#include <stdio.h>
#include "../../src/parser/parser.h"
#include "../../src/parser/byte/byte.h"
#include "../../src/parser/slice/token_slice.h"
#include "internal_functions_header.h"

MODULAR_DESCRIBE(internal_find_byte_end_tests, {
    TEST("finds byte end for shorthand def [0]", {
        Token tokens[] = ARRAY(new_token(BYTE_START), new_token(BIT_OFF), new_token(BYTE_END));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        int end = find_byte_end(slice);

        ASSERT_INT_EQUALS(end, 2);
    })
    TEST("finds byte end for regular def [01010101]", {
        Token tokens[] = ARRAY(new_token(BYTE_START), new_token(BIT_OFF), new_token(BIT_ON),new_token(BIT_OFF), new_token(BIT_ON),new_token(BIT_OFF), new_token(BIT_ON),new_token(BIT_OFF), new_token(BIT_ON), new_token(BYTE_END));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        int end = find_byte_end(slice);

        ASSERT_INT_EQUALS(end, 9);
    })
});
