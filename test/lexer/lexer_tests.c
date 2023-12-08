#include "../../lib/test/assert.h"
#include <stdio.h>
#include <string.h>
#include "../../src/lexer/lexer.h"
#include "../../src/lexer/token.h"

MODULAR_DESCRIBE(lexer_tests, {
    TEST("reads scope definition", {
        char prog[] = "a:{}";
        TokenVec vec = create_tokens(prog, strlen(prog));
        Token expected[4] = ARRAY(new_identifier_token("a"), new_token(DEFINE), new_token(SCOPE_OPEN), new_token(SCOPE_CLOSE));
        ASSERT_ARRS_EQUAL(vec.arr, vec.len, expected, LEN(expected), BY(type));
    });
    TEST("reads complex scope", {
        char prog[] = "a:{TAKE;PUT [0]\nRETURN TAKE}\nb:$a\nEXEC b";
        TokenVec vec = create_tokens(prog, strlen(prog));
        Token expected[21] = ARRAY(new_identifier_token("a"), new_token(DEFINE), new_token(SCOPE_OPEN),new_token(TAKE), new_token(TERM_SEM), new_token(PUT),new_token(BYTE_START),
                                  new_token(BIT_OFF),new_token(BYTE_END), new_token(TERM_NL),new_token(RETURN),new_token(TAKE), new_token(SCOPE_CLOSE),
                                  new_token(TERM_NL), new_identifier_token("b"),new_token(DEFINE),new_token(COPY), new_identifier_token("a"),new_token(TERM_NL),new_token(EXEC),
                                  new_identifier_token("b"));
        ASSERT_ARRS_EQUAL(vec.arr, vec.len, expected, LEN(expected), BY(type));
    });
});
