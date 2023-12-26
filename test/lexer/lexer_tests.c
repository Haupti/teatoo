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
        Token expected[4] = ARRAY(test_identifier_token("a"), test_token(DEFINE), test_token(SCOPE_OPEN), test_token(SCOPE_CLOSE));
        ASSERT_ARRS_EQUAL(vec.arr, vec.len, expected, LEN(expected), BY(type));
    });
    TEST("reads complex scope", {
        char prog[] = "a:{TAKE;PUT [0]\nRETURN TAKE}\nb:$a\nEXEC b";
        TokenVector vec = create_tokens(prog, strlen(prog));
        Token expected[21] = ARRAY(test_identifier_token("a"), test_token(DEFINE), test_token(SCOPE_OPEN),test_token(TAKE), test_token(TERM_SEM), test_token(PUT),test_token(BYTE_START),
                                  test_token(BIT_OFF),test_token(BYTE_END), test_token(TERM_NL),test_token(RETURN),test_token(TAKE), test_token(SCOPE_CLOSE),
                                  test_token(TERM_NL), test_identifier_token("b"),test_token(DEFINE),test_token(COPY), test_identifier_token("a"),test_token(TERM_NL),test_token(EXEC),
                                  test_identifier_token("b"));
        ASSERT_ARRS_EQUAL(vec.arr, vec.len, expected, LEN(expected), BY(type));
    });
    TEST("reads various keywords", {
        char prog[] = "a:{}[10];\nPUT TAKE RETURN EQ NEQ IF EXEC $ XOR | & !! OUT OUTCHAR OUTNUM PEEK ()";
        TokenVector vec = create_tokens(prog, strlen(prog));
        Token expected[29] = ARRAY(test_identifier_token("a"), test_token(DEFINE), test_token(SCOPE_OPEN), test_token(SCOPE_CLOSE),
                                   test_token(BYTE_START), test_token(BIT_ON), test_token(BIT_OFF),test_token(BYTE_END),
                                   test_token(TERM_SEM),test_token(TERM_NL), test_token(PUT),test_token(TAKE),test_token(RETURN),
                                   test_token(EQ), test_token(NEQ),test_token(IF),test_token(EXEC), test_token(COPY),test_token(XOR),test_token(OR),
                                   test_token(AND), test_token(NOT), test_token(NOT), test_token(OUT), test_token(OUTCHAR), test_token(OUTNUM), test_token(PEEK), test_token(GRP_OPEN), test_token(GRP_CLOSE));
        ASSERT_ARRS_EQUAL(vec.arr, vec.len, expected, LEN(expected), BY(type));
    })
    TEST("reads new keywords: EMPTY? NULL? POW", {
        char prog[] = "EMPTY? NULL? POW";
        TokenVector vec = create_tokens(prog, strlen(prog));
        Token expected[] = ARRAY(test_token(IS_EMPTY), test_token(IS_NULL), test_token(POW));
        ASSERT_ARRS_EQUAL(vec.arr, vec.len, expected, LEN(expected), BY(type));
    })
    TEST("reads new keywords: STACK APPLY", {
        char prog[] = "STACK APPLY";
        TokenVector vec = create_tokens(prog, strlen(prog));
        Token expected[] = ARRAY(test_token(STACK), test_token(APPLY));
        ASSERT_ARRS_EQUAL(vec.arr, vec.len, expected, LEN(expected), BY(type));
    })
    TEST("reads new keywords: MINUS PLUS MODULO MULTIPLY DIVIDE", {
        char prog[] = "MINUS PLUS MODULO MULTIPLY DIVIDE";
        TokenVector vec = create_tokens(prog, strlen(prog));
        Token expected[] = ARRAY(test_token(MINUS), test_token(PLUS), test_token(MODULO), test_token(MULTIPLY), test_token(DIVIDE));
        ASSERT_ARRS_EQUAL(vec.arr, vec.len, expected, LEN(expected), BY(type));
    })
    TEST("empty program has no tokens", {
        char prog[] = "";
        TokenVector vec = create_tokens(prog, strlen(prog));
        ASSERT_EQUALS(vec.len, 0);
    })
});
