#include "../../../lib/test/assert.h"
#include <stdio.h>
#include "../../../src/parser/find/find.h"
#include "../../../src/parser/slice/token_slice.h"

MODULAR_DESCRIBE(internal_find_matching_sequence_end_tests, {
    TEST("finds sequence end for empty sequence", {
        Token tokens[] = ARRAY(test_token(GRP_OPEN), test_token(GRP_CLOSE));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        int end = find_matching_sequence_end(slice);

        ASSERT_INT_EQUALS(end, 1);
    })
    TEST("finds end for sequence with stuff in it", {
        Token tokens[] = ARRAY(test_token(GRP_OPEN), test_token(TAKE), test_token(GRP_CLOSE));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        int end = find_matching_sequence_end(slice);

        ASSERT_INT_EQUALS(end, 2);
    })
    TEST("finds end for sequence with sequence in it", {
        Token tokens[] = ARRAY(test_token(GRP_OPEN), test_token(PUT), test_token(GRP_OPEN), test_token(TAKE), test_token(GRP_CLOSE), test_token(GRP_CLOSE));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        int end = find_matching_sequence_end(slice);

        ASSERT_INT_EQUALS(end, 5);
    })
    TEST("finds end for scope with sequence and terminators in it", {
        Token tokens[] = ARRAY(test_token(GRP_OPEN), test_token(PUT), test_token(GRP_OPEN), test_token(TAKE), test_token(TERM_SEM), test_token(GRP_CLOSE), test_token(TERM_NL),  test_token(GRP_CLOSE));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        int end = find_matching_sequence_end(slice);

        ASSERT_INT_EQUALS(end, 7);
    })
})
