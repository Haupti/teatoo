#include "../../lib/test/assert.h"
#include <stdio.h>
#include "../../src/parser/parser.h"
#include "../../src/parser/byte/byte.h"
#include "../../src/parser/slice/token_slice.h"
#include "internal_functions_header.h"

MODULAR_DESCRIBE(internal_find_matching_sequence_end_tests, {
    TEST("finds sequence end for empty sequence", {
        Token tokens[] = ARRAY(new_token(GRP_OPEN), new_token(GRP_CLOSE));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        int end = find_matching_sequence_end(slice);

        ASSERT_INT_EQUALS(end, 1);
    })
    TEST("finds end for sequence with stuff in it", {
        Token tokens[] = ARRAY(new_token(GRP_OPEN), new_token(TAKE), new_token(GRP_CLOSE));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        int end = find_matching_sequence_end(slice);

        ASSERT_INT_EQUALS(end, 2);
    })
    TEST("finds end for sequence with sequence in it", {
        Token tokens[] = ARRAY(new_token(GRP_OPEN), new_token(PUT), new_token(GRP_OPEN), new_token(TAKE), new_token(GRP_CLOSE), new_token(GRP_CLOSE));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        int end = find_matching_sequence_end(slice);

        ASSERT_INT_EQUALS(end, 5);
    })
    TEST("finds end for scope with sequence and terminators in it", {
        Token tokens[] = ARRAY(new_token(GRP_OPEN), new_token(PUT), new_token(GRP_OPEN), new_token(TAKE), new_token(TERM_SEM), new_token(GRP_CLOSE), new_token(TERM_NL),  new_token(GRP_CLOSE));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        int end = find_matching_sequence_end(slice);

        ASSERT_INT_EQUALS(end, 7);
    })
})
