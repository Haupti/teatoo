#include "../../lib/test/assert.h"
#include <stdio.h>
#include "../../src/parser/parser.h"
#include "../../src/parser/byte/byte.h"
#include "../../src/parser/slice/token_slice.h"
#include "../../src/lexer/token_vector.h"


MODULAR_DESCRIBE(parse_module_tests, {
    TEST("parse empty module", {
        Token tokens[] = ARRAY();
        TokenVector vec = new_token_vector(tokens, LEN(tokens));

        Module module = create_parse_module(vec);

        ASSERT_INT_EQUALS(module.scopes.len, 0);
        ASSERT_INT_EQUALS(module.has_entrypoint,  0);
    });
    TEST("parse module with only exec", {
        Token tokens[] = ARRAY(test_token(EXEC), test_token(GRP_OPEN), test_token(OUTCHAR), test_token(BYTE_START), test_token(BIT_OFF), test_token(BIT_OFF), test_token(BIT_OFF), test_token(BIT_OFF), test_token(BIT_OFF), test_token(BIT_OFF), test_token(BIT_OFF), test_token(BIT_OFF), test_token(BYTE_END), test_token(GRP_CLOSE), test_token(TERM_NL));
        TokenVector vec = new_token_vector(tokens, LEN(tokens));

        Module module = create_parse_module(vec);

        ASSERT_INT_EQUALS(module.scopes.len, 0);
        ASSERT_INT_EQUALS(module.has_entrypoint,  1);
    });
    TEST("parse module with only scope", {
        Token tokens[] = ARRAY(test_identifier_token("perl"), test_token(DEFINE), test_token(SCOPE_OPEN), test_token(OUTCHAR), test_token(BYTE_START), test_token(BIT_ON), test_token(BYTE_END), test_token(SCOPE_CLOSE), test_token(TERM_NL));
        TokenVector vec = new_token_vector(tokens, LEN(tokens));

        Module module = create_parse_module(vec);

        ASSERT_INT_EQUALS(module.scopes.len, 1);
        ASSERT_INT_EQUALS(module.has_entrypoint, 0);
    });
    TEST("parse module with only scope", {
        Token tokens[] = ARRAY(test_identifier_token("perl"), test_token(DEFINE), test_token(SCOPE_OPEN), test_token(OUTCHAR), test_token(BYTE_START), test_token(BIT_ON), test_token(BYTE_END), test_token(SCOPE_CLOSE), test_token(TERM_NL), test_token(EXEC), test_token(GRP_OPEN), test_token(OUTCHAR), test_token(BYTE_START), test_token(BIT_OFF), test_token(BIT_OFF), test_token(BIT_OFF), test_token(BIT_OFF), test_token(BIT_OFF), test_token(BIT_OFF), test_token(BIT_OFF), test_token(BIT_OFF), test_token(BYTE_END), test_token(GRP_CLOSE), test_token(TERM_NL));
        TokenVector vec = new_token_vector(tokens, LEN(tokens));

        Module module = create_parse_module(vec);

        ASSERT_INT_EQUALS(module.scopes.len, 1);
        ASSERT_INT_EQUALS(module.has_entrypoint, 1);
    });
});
