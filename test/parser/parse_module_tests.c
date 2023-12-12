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
        Token tokens[] = ARRAY(new_token(EXEC), new_token(GRP_OPEN), new_token(OUTCHAR), new_token(BYTE_START), new_token(BIT_OFF), new_token(BIT_OFF), new_token(BIT_OFF), new_token(BIT_OFF), new_token(BIT_OFF), new_token(BIT_OFF), new_token(BIT_OFF), new_token(BIT_OFF), new_token(BYTE_END), new_token(GRP_CLOSE), new_token(TERM_NL));
        TokenVector vec = new_token_vector(tokens, LEN(tokens));

        Module module = create_parse_module(vec);

        ASSERT_INT_EQUALS(module.scopes.len, 0);
        ASSERT_INT_EQUALS(module.has_entrypoint,  1);
    });
    TEST("parse module with only scope", {
        Token tokens[] = ARRAY(new_identifier_token("perl"), new_token(DEFINE), new_token(SCOPE_OPEN), new_token(OUTCHAR), new_token(BYTE_START), new_token(BIT_ON), new_token(BYTE_END), new_token(SCOPE_CLOSE), new_token(TERM_NL));
        TokenVector vec = new_token_vector(tokens, LEN(tokens));

        Module module = create_parse_module(vec);

        ASSERT_INT_EQUALS(module.scopes.len, 1);
        ASSERT_INT_EQUALS(module.has_entrypoint, 0);
    });
    TEST("parse module with only scope", {
        Token tokens[] = ARRAY(new_identifier_token("perl"), new_token(DEFINE), new_token(SCOPE_OPEN), new_token(OUTCHAR), new_token(BYTE_START), new_token(BIT_ON), new_token(BYTE_END), new_token(SCOPE_CLOSE), new_token(TERM_NL), new_token(EXEC), new_token(GRP_OPEN), new_token(OUTCHAR), new_token(BYTE_START), new_token(BIT_OFF), new_token(BIT_OFF), new_token(BIT_OFF), new_token(BIT_OFF), new_token(BIT_OFF), new_token(BIT_OFF), new_token(BIT_OFF), new_token(BIT_OFF), new_token(BYTE_END), new_token(GRP_CLOSE), new_token(TERM_NL));
        TokenVector vec = new_token_vector(tokens, LEN(tokens));

        Module module = create_parse_module(vec);

        ASSERT_INT_EQUALS(module.scopes.len, 1);
        ASSERT_INT_EQUALS(module.has_entrypoint, 1);
    });
});
