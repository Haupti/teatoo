#include "../../../lib/test/assert.h"
#include <stdio.h>
#include "../../../src/parser/slice/token_slice.h"
#include "../../../src/parser/find/find.h"

MODULAR_DESCRIBE(find_argument_end_tests, {
    TEST("finds argument end in two args", {
        Token tokens[] = ARRAY(test_token(BYTE_START), test_token(BIT_OFF), test_token(BYTE_END), test_token(GRP_OPEN), test_token(TAKE), test_token(GRP_CLOSE) , test_token(TERM_NL), test_token(BIT_OFF));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        int result = find_argument_end(slice);

        ASSERT_INT_EQUALS(result, 2);
    })
    TEST("finds statement end in one arg", {
        Token tokens[] = ARRAY( test_token(BYTE_START), test_token(BIT_OFF), test_token(BYTE_END), test_token(TERM_NL));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        int result = find_argument_end(slice);

        ASSERT_INT_EQUALS(result, 2);
    })
    TEST("finds argument end in two args, starting at grp open", {
        Token tokens[] = ARRAY(test_token(GRP_OPEN), test_token(TAKE), test_token(GRP_CLOSE), test_token(BYTE_START), test_token(BIT_OFF), test_token(BYTE_END));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        int result = find_argument_end(slice);

        ASSERT_INT_EQUALS(result, 2);
    })
    TEST("finds argument end in two args, starting at identifier", {
        Token tokens[] = ARRAY(test_identifier_token("earl"), test_token(GRP_CLOSE), test_token(BYTE_START), test_token(BIT_OFF), test_token(BYTE_END));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        int result = find_argument_end(slice);

        ASSERT_INT_EQUALS(result, 0);
    })
    TEST("finds argument end in two args, starting at copy identifier", {
        Token tokens[] = ARRAY(test_token(COPY), test_identifier_token("earl"), test_token(GRP_CLOSE), test_token(BYTE_START), test_token(BIT_OFF), test_token(BYTE_END));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        int result = find_argument_end(slice);

        ASSERT_INT_EQUALS(result, 1);
    })
})
