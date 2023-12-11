
#include "../../lib/test/assert.h"
#include <stdio.h>
#include <string.h>
#include "../../src/parser/parser.h"
#include "../../src/parser/byte/byte.h"
#include "../../src/parser/slice/token_slice.h"
#include "internal_functions_header.h"

MODULAR_DESCRIBE(internal_parse_sequence_arg_tests, {
    TEST("parses the args of a empty sequence", {
        Token tokens[] = ARRAY(new_token(GRP_OPEN), new_token(GRP_CLOSE));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        Sequence sequence = parse_sequence_arg(slice);

        ASSERT_INT_EQUALS((int) sequence.op_count, 0);
        ASSERT_EQUALS(sequence.ops, NULL);
    });
    TEST("parses the args of a sequence with one statement", {
        Token tokens[] = ARRAY(new_token(GRP_OPEN), new_token(PUT), new_token(BYTE_START), new_token(BIT_OFF), new_token(BYTE_END), new_token(GRP_CLOSE), new_token(TERM_NL));
        TokenSlice slice = new_token_slice(tokens, LEN(tokens));

        Sequence sequence = parse_sequence_arg(slice);

        ASSERT_INT_EQUALS((int) sequence.op_count, 1);
        ASSERT_EQUALS(sequence.ops[0].type, OT_PUT);
        ASSERT_INT_EQUALS(sequence.ops[0].op.op_put.first.is_byte, 1);
        ASSERT_INT_EQUALS(sequence.ops[0].op.op_put.first.byte, 0);
    });
    TEST("parses the args of a sequence with two statement and new line terminator", {
        Token tokens[] = ARRAY(new_token(GRP_OPEN), new_token(PUT), new_token(BYTE_START), new_token(BIT_OFF), new_token(BYTE_END), new_token(TERM_NL), new_token(TAKE), new_token(GRP_CLOSE));
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
        Token tokens[] = ARRAY(new_token(GRP_OPEN), new_token(PUT), new_token(BYTE_START), new_token(BIT_OFF), new_token(BYTE_END), new_token(TERM_SEM), new_token(TAKE), new_token(GRP_CLOSE));
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
        Token tokens[] = ARRAY(new_token(GRP_OPEN), new_token(PUT), new_token(GRP_OPEN), new_token(TAKE), new_token(GRP_CLOSE), new_token(TERM_SEM), new_token(TAKE), new_token(GRP_CLOSE));
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
