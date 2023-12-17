#include "../../../lib/test/assert.h"
#include <stdio.h>
#include "../../../src/parser/slice/token_slice.h"
#include "../../../src/parser/find/find.h"

MODULAR_DESCRIBE(find_argument_end_tests, {
    TEST("finds argument end in two args", {
        Token tokens[] = ARRAY(new_token(BYTE_START), new_token(BIT_OFF), new_token(BYTE_END), new_token(GRP_OPEN), new_token(TAKE), new_token(GRP_CLOSE) , new_token(TERM_NL), new_token(BIT_OFF));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        int result = find_argument_end(slice);

        ASSERT_INT_EQUALS(result, 2);
    })
    TEST("finds statement end in one arg", {
        Token tokens[] = ARRAY( new_token(BYTE_START), new_token(BIT_OFF), new_token(BYTE_END), new_token(TERM_NL));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        int result = find_argument_end(slice);

        ASSERT_INT_EQUALS(result, 2);
    })
    TEST("finds argument end in two args, starting at grp open", {
        Token tokens[] = ARRAY(new_token(GRP_OPEN), new_token(TAKE), new_token(GRP_CLOSE), new_token(BYTE_START), new_token(BIT_OFF), new_token(BYTE_END));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        int result = find_argument_end(slice);

        ASSERT_INT_EQUALS(result, 2);
    })
    TEST("finds argument end in two args, starting at identifier", {
        Token tokens[] = ARRAY(new_identifier_token("earl"), new_token(GRP_CLOSE), new_token(BYTE_START), new_token(BIT_OFF), new_token(BYTE_END));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        int result = find_argument_end(slice);

        ASSERT_INT_EQUALS(result, 0);
    })
    TEST("finds argument end in two args, starting at copy identifier", {
        Token tokens[] = ARRAY(new_token(COPY), new_identifier_token("earl"), new_token(GRP_CLOSE), new_token(BYTE_START), new_token(BIT_OFF), new_token(BYTE_END));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        int result = find_argument_end(slice);

        ASSERT_INT_EQUALS(result, 1);
    })
})
