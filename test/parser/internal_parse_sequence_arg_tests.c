
#include "../../lib/test/assert.h"
#include <stdio.h>
#include <string.h>
#include "../../src/parser/parser.h"
#include "../../src/parser/byte/byte.h"
#include "../../src/parser/slice/token_slice.h"
#include "internal_functions_header.h"

MODULAR_DESCRIBE(internal_parse_sequence_arg_tests, {
    TEST("parses the args of a sequence with one statement", {
        Token tokens[] = ARRAY(test_token(GRP_OPEN), test_token(PUT), test_token(BYTE_START), test_token(BIT_OFF), test_token(BYTE_END), test_token(GRP_CLOSE), test_token(TERM_NL));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        Sequence sequence = parse_sequence_arg(slice);

        ASSERT_INT_EQUALS((int) sequence.op_count, 1);
        ASSERT_EQUALS(sequence.ops[0].type, OT_PUT);
        ASSERT_INT_EQUALS(sequence.ops[0].op.op_put.first.is_byte, 1);
        ASSERT_INT_EQUALS(sequence.ops[0].op.op_put.first.byte, 0);
    });
    TEST("parses the args of a sequence with two statement and new line terminator", {
        Token tokens[] = ARRAY(test_token(GRP_OPEN), test_token(PUT), test_token(BYTE_START), test_token(BIT_OFF), test_token(BYTE_END), test_token(TERM_NL), test_token(TAKE), test_token(GRP_CLOSE));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        Sequence sequence = parse_sequence_arg(slice);

        ASSERT_INT_EQUALS((int) sequence.op_count, 2);
        ASSERT_EQUALS(sequence.ops[0].type, OT_PUT);
        ASSERT_INT_EQUALS(sequence.ops[0].op.op_put.first.is_byte, 1);
        ASSERT_INT_EQUALS(sequence.ops[0].op.op_put.first.byte, 0);
        ASSERT_INT_EQUALS((int) sequence.op_count, 2);
        ASSERT_EQUALS(sequence.ops[1].type, OT_TAKE);
    });
    TEST("parses the args of a sequence with two statement and semicolon terminator", {
        Token tokens[] = ARRAY(test_token(GRP_OPEN), test_token(PUT), test_token(BYTE_START), test_token(BIT_OFF), test_token(BYTE_END), test_token(TERM_SEM), test_token(TAKE), test_token(GRP_CLOSE));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        Sequence sequence = parse_sequence_arg(slice);

        ASSERT_INT_EQUALS((int) sequence.op_count, 2);
        ASSERT_EQUALS(sequence.ops[0].type, OT_PUT);
        ASSERT_INT_EQUALS(sequence.ops[0].op.op_put.first.is_byte, 1);
        ASSERT_INT_EQUALS(sequence.ops[0].op.op_put.first.byte, 0);
        ASSERT_INT_EQUALS((int) sequence.op_count, 2);
        ASSERT_EQUALS(sequence.ops[1].type, OT_TAKE);
    });

    TEST("parses the args of a sequence with two statement where one has an sequence argument", {
        Token tokens[] = ARRAY(test_token(GRP_OPEN), test_token(PUT), test_token(GRP_OPEN), test_token(TAKE), test_token(GRP_CLOSE), test_token(TERM_SEM), test_token(TAKE), test_token(GRP_CLOSE));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        Sequence sequence = parse_sequence_arg(slice);

        ASSERT_INT_EQUALS((int) sequence.op_count, 2);
        ASSERT_EQUALS(sequence.ops[0].type, OT_PUT);
        ASSERT_INT_EQUALS(sequence.ops[0].op.op_put.first.is_sequence, 1);
        ASSERT_INT_EQUALS((int) sequence.ops[0].op.op_put.first.sequence.op_count, 1);
        ASSERT_INT_EQUALS(sequence.ops[0].op.op_put.first.sequence.ops[0].type, OT_TAKE);
        ASSERT_INT_EQUALS((int) sequence.op_count, 2);
        ASSERT_EQUALS(sequence.ops[1].type, OT_TAKE);
    });
});
