#include "../../lib/test/assert.h"
#include <stdio.h>
#include <string.h>
#include "../../src/parser/parser.h"
#include "../../src/parser/byte/byte.h"
#include "../../src/parser/slice/token_slice.h"
#include "internal_functions_header.h"

MODULAR_DESCRIBE(internal_parse_scope_tests, {
    TEST("parses empty scope", {
        Token tokens[] = ARRAY(test_token(SCOPE_OPEN), test_token(SCOPE_CLOSE));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        Scope scope = parse_scope("steve", slice);

        ASSERT_STR_EQUALS(scope.name, "steve");
        ASSERT_INT_EQUALS(scope.stack.len, 0);
        ASSERT_INT_EQUALS(scope.statements.statements_len, 0);
        ASSERT_EQUALS(scope.statements.statements, NULL);
    });
    TEST("parses scope with only one statement", {
        Token tokens[] = ARRAY(test_token(SCOPE_OPEN), test_token(TAKE), test_token(SCOPE_CLOSE));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        Scope scope = parse_scope("steve", slice);

        ASSERT_STR_EQUALS(scope.name, "steve");
        ASSERT_INT_EQUALS(scope.stack.len, 0);
        ASSERT_INT_EQUALS(scope.statements.statements_len, 1);
        ASSERT_EQUALS(scope.statements.statements[0].type, OT_TAKE);
    });
    TEST("parses scope with two statements and if", {
        Token tokens[] = ARRAY(test_token(SCOPE_OPEN), test_token(IF), test_token(BYTE_START), test_token(BIT_ON), test_token(BYTE_END), test_token(GRP_OPEN) ,test_token(TAKE), test_token(GRP_CLOSE), test_token(TERM_NL), test_token(RETURN), test_token(GRP_OPEN), test_token(TAKE), test_token(GRP_CLOSE),test_token(SCOPE_CLOSE));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        Scope scope = parse_scope("steve", slice);

        ASSERT_STR_EQUALS(scope.name, "steve");
        ASSERT_INT_EQUALS(scope.stack.len, 0);
        ASSERT_INT_EQUALS(scope.statements.statements_len, 2);
        ASSERT_EQUALS(scope.statements.statements[0].type, OT_IF);
        ASSERT_EQUALS(scope.statements.statements[1].type, OT_RETURN);

        // check if
        Op_IF opif = scope.statements.statements[0].op.op_if;
        ASSERT_INT_EQUALS(opif.condition.is_byte, 1);
        ASSERT_INT_EQUALS(opif.operation.is_sequence, 1);
        ASSERT_INT_EQUALS((int) opif.operation.sequence.op_count, 1);
        ASSERT_EQUALS(opif.operation.sequence.ops[0].type, OT_TAKE);

        // check return
        Op_RETURN opreturn = scope.statements.statements[1].op.op_return;
        ASSERT_INT_EQUALS(opreturn.first.is_sequence, 1);
        ASSERT_INT_EQUALS((int) opif.operation.sequence.op_count, 1);
        ASSERT_EQUALS(opif.operation.sequence.ops[0].type, OT_TAKE);
    });
});
