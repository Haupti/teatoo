#include "../../lib/test/assert.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "../../src/lexer/lexer.h"
#include "../../src/lexer/token.h"
#include "../../src/lexer/token_vector.h"

MODULAR_DESCRIBE(lexer_tests, {
    TEST("reads scope definition", {
        char prog[] = "a:{}";
        TokenVector vec = create_tokens(prog, strlen(prog));
        Token expected[4] = ARRAY(new_identifier_token("a"), new_token(DEFINE), new_token(SCOPE_OPEN), new_token(SCOPE_CLOSE));
        ASSERT_ARRS_EQUAL(vec.arr, vec.len, expected, LEN(expected), BY(type));
    });
    TEST("reads complex scope", {
        char prog[] = "a:{TAKE;PUT [0]\nRETURN TAKE}\nb:$a\nEXEC b";
        TokenVector vec = create_tokens(prog, strlen(prog));
        Token expected[21] = ARRAY(new_identifier_token("a"), new_token(DEFINE), new_token(SCOPE_OPEN),new_token(TAKE), new_token(TERM_SEM), new_token(PUT),new_token(BYTE_START),
                                  new_token(BIT_OFF),new_token(BYTE_END), new_token(TERM_NL),new_token(RETURN),new_token(TAKE), new_token(SCOPE_CLOSE),
                                  new_token(TERM_NL), new_identifier_token("b"),new_token(DEFINE),new_token(COPY), new_identifier_token("a"),new_token(TERM_NL),new_token(EXEC),
                                  new_identifier_token("b"));
        ASSERT_ARRS_EQUAL(vec.arr, vec.len, expected, LEN(expected), BY(type));
    });
    TEST("reads various keywords", {
        char prog[] = "a:{}[10];\nPUT TAKE RETURN EQ NEQ IF EXEC $ XOR | & !! OUT OUTCHAR OUTNUM PEEK ()";
        TokenVector vec = create_tokens(prog, strlen(prog));
        Token expected[29] = ARRAY(new_identifier_token("a"), new_token(DEFINE), new_token(SCOPE_OPEN), new_token(SCOPE_CLOSE),
                                   new_token(BYTE_START), new_token(BIT_ON), new_token(BIT_OFF),new_token(BYTE_END),
                                   new_token(TERM_SEM),new_token(TERM_NL), new_token(PUT),new_token(TAKE),new_token(RETURN),
                                   new_token(EQ), new_token(NEQ),new_token(IF),new_token(EXEC), new_token(COPY),new_token(XOR),new_token(OR),
                                   new_token(AND), new_token(NOT), new_token(NOT), new_token(OUT), new_token(OUTCHAR), new_token(OUTNUM), new_token(PEEK), new_token(GRP_OPEN), new_token(GRP_CLOSE));
        ASSERT_ARRS_EQUAL(vec.arr, vec.len, expected, LEN(expected), BY(type));
    })
    TEST("reads new keywords", {
        char prog[] = "EMTPY? NULL? POW";
        TokenVector vec = create_tokens(prog, strlen(prog));
        Token expected[] = ARRAY(new_token(IS_EMPTY), new_token(IS_NULL), new_token(POW));
        ASSERT_ARRS_EQUAL(vec.arr, vec.len, expected, LEN(expected), BY(type));
    })
    TEST("empty program has no tokens", {
        char prog[] = "";
        TokenVector vec = create_tokens(prog, strlen(prog));
        ASSERT_EQUALS(vec.len, 0);
    })
});
