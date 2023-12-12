#include "../../lib/test/assert.h"
#include <stdio.h>
#include "../../src/parser/parser.h"
#include "../../src/parser/byte/parse_byte.h"
#include "../../src/parser/byte/byte.h"

MODULAR_DESCRIBE(parse_byte_tests, {
    TEST("parse byte parses byte [1] as 127", {
        Token tokens[] = ARRAY(new_token(BYTE_START), new_token(BIT_ON), new_token(BYTE_END));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        Byte byte = parse_byte(slice);

        ASSERT_EQUALS(byte, 127);
    });
    TEST("parse byte parses byte [0] as 0", {
        Token tokens[] = ARRAY(new_token(BYTE_START), new_token(BIT_OFF), new_token(BYTE_END));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        Byte byte = parse_byte(slice);

        ASSERT_EQUALS(byte, '\0');
    });
    TEST("parse byte parses byte [01010101] as 85", {
        Token tokens[] = ARRAY(new_token(BYTE_START), new_token(BIT_OFF), new_token(BIT_ON),new_token(BIT_OFF), new_token(BIT_ON),new_token(BIT_OFF), new_token(BIT_ON),new_token(BIT_OFF), new_token(BIT_ON), new_token(BYTE_END));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        Byte byte = parse_byte(slice);

        ASSERT_INT_EQUALS(byte, 85);
    });
});
