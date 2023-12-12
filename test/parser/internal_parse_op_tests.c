#include "../../lib/test/assert.h"
#include <stdio.h>
#include <string.h>
#include "../../src/parser/parser.h"
#include "../../src/parser/byte/byte.h"
#include "../../src/parser/slice/token_slice.h"
#include "internal_functions_header.h"

MODULAR_DESCRIBE(internal_parse_op_tests, {
    TEST("parses TAKE", {
        Token tokens[] = ARRAY(new_token(TAKE), new_token(TERM_NL));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        GenericOp op = parse_op(slice);

        ASSERT_EQUALS(op.type, OT_TAKE);
    });
    TEST("parses PUT", {
        Token tokens[] = ARRAY(new_token(PUT), new_token(BYTE_START), new_token(BIT_ON), new_token(BYTE_END), new_token(TERM_NL));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        GenericOp op = parse_op(slice);

        ASSERT_EQUALS(op.type, OT_PUT);
        ASSERT_INT_EQUALS(op.op.op_put.first.is_byte, 1);
        ASSERT_INT_EQUALS(op.op.op_put.first.byte, 127);
    });
    TEST("parses IF", {
        Token tokens[] = ARRAY(new_token(IF), new_token(BYTE_START), new_token(BIT_ON), new_token(BYTE_END), new_token(GRP_OPEN), new_token(TAKE), new_token(GRP_CLOSE), new_token(TERM_NL));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        GenericOp op = parse_op(slice);

        ASSERT_EQUALS(op.type, OT_IF);
        ASSERT_INT_EQUALS(op.op.op_if.condition.is_byte, 1);
        ASSERT_INT_EQUALS(op.op.op_if.condition.byte, 127);
        ASSERT_INT_EQUALS(op.op.op_if.operation.is_sequence, 1);
        ASSERT_INT_EQUALS((int) op.op.op_if.operation.sequence.op_count, 1);
        ASSERT_EQUALS(op.op.op_if.operation.sequence.ops[0].type, OT_TAKE);
    });
});
