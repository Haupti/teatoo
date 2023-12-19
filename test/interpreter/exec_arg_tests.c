#include "../../lib/test/assert.h"
#include <stdio.h>
#include <string.h>
#include "../../src/parser/instruction/instruction.h"
#include "interpreter_internal_header.h"

MODULAR_DESCRIBE(exec_arg_tests, {
    Statements statements = ARRAY(NULL, 0);

    TEST("executes an argument byte, returns the byte result", {
        Byte stack_vals[] = ARRAY('c', '\0', 255, 'x', 't');
        Byte * mem = malloc(sizeof(Byte) * (LEN(stack_vals)));
        memcpy(mem, stack_vals, sizeof(Byte) *(LEN(stack_vals)));
        ByteVector stack = ARRAY(mem, LEN(stack_vals));
        ActiveScope active_scope = ARRAY("test_scope", stack, statements, null_result(), 0);

        Result result = exec_arg(&active_scope, new_byte_argument('G'));

        ASSERT_INT_EQUALS(result.is_byte, 1);
        ASSERT_INT_EQUALS(result.is_null, 0);
        ASSERT_INT_EQUALS(result.byte, 'G');
        ASSERT_INT_EQUALS(active_scope.stack.len, 5);
    })

    TEST("executes an argument sequence, returns the result", {
        Byte stack_vals[] = ARRAY('c', '\0', 255, 'x', 't');
        Byte * mem = malloc(sizeof(Byte) * (LEN(stack_vals)));
        memcpy(mem, stack_vals, sizeof(Byte) *(LEN(stack_vals)));
        ByteVector stack = ARRAY(mem, LEN(stack_vals));
        ActiveScope active_scope = ARRAY("test_scope", stack, statements, null_result(), 0);

        GenericOp ops[] = ARRAY(new_peek(), new_take());
        Sequence seq = ARRAY(ops, LEN(ops));

        Result result = exec_arg(&active_scope, new_sequence_argument(seq));

        ASSERT_INT_EQUALS(result.is_byte, 1);
        ASSERT_INT_EQUALS(result.is_null, 0);
        ASSERT_INT_EQUALS(result.byte, 't');
        ASSERT_INT_EQUALS(active_scope.stack.len, 4);
    })
})
