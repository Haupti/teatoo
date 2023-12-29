#include "../../lib/test/assert.h"
#include <stdio.h>
#include <string.h>
#include "../../src/parser/parser.h"
#include "../../src/parser/byte/byte.h"
#include "../../src/parser/slice/token_slice.h"
#include "internal_functions_header.h"

MODULAR_DESCRIBE(internal_parse_op_tests, {
    TEST("parses TAKE", {
        Token tokens[] = ARRAY(test_token(TAKE), test_token(TERM_NL));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        GenericOp op = parse_op(slice);

        ASSERT_EQUALS(op.type, OT_TAKE);
    });
    TEST("parses PUT", {
        Token tokens[] = ARRAY(test_token(PUT), test_token(BYTE_START), test_token(BIT_ON), test_token(BYTE_END), test_token(TERM_NL));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        GenericOp op = parse_op(slice);

        ASSERT_EQUALS(op.type, OT_PUT);
        ASSERT_INT_EQUALS(op.op.op_put.first.is_byte, 1);
        ASSERT_INT_EQUALS(op.op.op_put.first.byte, 0xFF);
    });
    TEST("parses IF", {
        Token tokens[] = ARRAY(test_token(IF), test_token(BYTE_START), test_token(BIT_ON), test_token(BYTE_END), test_token(GRP_OPEN), test_token(TAKE), test_token(GRP_CLOSE), test_token(TERM_NL));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        GenericOp op = parse_op(slice);

        ASSERT_EQUALS(op.type, OT_IF);
        ASSERT_INT_EQUALS(op.op.op_if.condition.is_byte, 1);
        ASSERT_INT_EQUALS(op.op.op_if.condition.byte, 0xFF);
        ASSERT_INT_EQUALS(op.op.op_if.operation.is_sequence, 1);
        ASSERT_INT_EQUALS((int) op.op.op_if.operation.sequence.op_count, 1);
        ASSERT_EQUALS(op.op.op_if.operation.sequence.ops[0].type, OT_TAKE);
    });
    TEST("parses STACK", {
        Token tokens[] = ARRAY(test_token(STACK), test_token(COPY), test_identifier_token("jörg"), test_token(BYTE_START), test_token(BIT_ON), test_token(BYTE_END));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        GenericOp op = parse_op(slice);

        ASSERT_EQUALS(op.type, OT_STACK);
        ASSERT_INT_EQUALS(op.op.op_stack.first.is_copy_ref, 1);
        ASSERT_STR_EQUALS(op.op.op_stack.first.scope_name, "jörg");
        ASSERT_INT_EQUALS(op.op.op_stack.second.is_byte, 1);
        ASSERT_INT_EQUALS(op.op.op_stack.second.byte, 0xFF);
    });
    TEST("parses MODULO", {
        Token tokens[] = ARRAY(test_token(MODULO), test_token(BYTE_START), test_token(BIT_ON), test_token(BYTE_END), test_token(BYTE_START), test_token(BIT_ON), test_token(BYTE_END));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        GenericOp op = parse_op(slice);

        ASSERT_EQUALS(op.type, OT_MODULO);
        ASSERT_INT_EQUALS(op.op.op_modulo.first.is_byte, 1);
        ASSERT_INT_EQUALS(op.op.op_modulo.first.byte, 0xFF);
        ASSERT_INT_EQUALS(op.op.op_modulo.second.is_byte, 1);
        ASSERT_INT_EQUALS(op.op.op_modulo.second.byte, 0xFF);
    });
    TEST("parses complex op", {
        Token tokens[] = ARRAY(test_token(PUT), test_token(GRP_OPEN), test_token(EQ), test_token(GRP_OPEN),test_token(MODULO), test_token(GRP_OPEN), test_token(PEEK), test_token(GRP_CLOSE), test_token(BYTE_START), test_token(BIT_OFF), test_token(BYTE_END), test_token(GRP_CLOSE), test_token(BYTE_START), test_token(BIT_OFF), test_token(BYTE_END), test_token(GRP_CLOSE));

        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        GenericOp op = parse_op(slice);

        ASSERT_EQUALS(op.type, OT_PUT);
    })
});
