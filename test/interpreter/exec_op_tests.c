#include "../../lib/test/assert.h"
#include <stdio.h>
#include <string.h>
#include "../../src/parser/instruction/instruction.h"
#include "interpreter_internal_header.h"

MODULAR_DESCRIBE(exec_op_tests, {
    Statements statements = ARRAY(NULL, 0);

    TEST("executes peek, returns null because stack is empty", {
        Byte empty_stack_vals[] = ARRAY();
        ByteVector empty_stack = ARRAY(empty_stack_vals, 0);
        ActiveScope active_scope_empty_stack = ARRAY("test_scope_empty_stack", empty_stack, statements, null_result(), 0);

        Result result = exec_op(&active_scope_empty_stack, new_peek());

        ASSERT_INT_EQUALS(active_scope_empty_stack.stack.len, 0);
        ASSERT_INT_EQUALS(result.is_byte, 0);
        ASSERT_INT_EQUALS(result.is_null, 1);
    })
    TEST("executes peek, returns last of stack (top)", {
        Byte stack_vals[] = ARRAY('c', '\0', 255, 'x', 't');
        ByteVector stack = ARRAY(stack_vals, 5);
        ActiveScope active_scope = ARRAY("test_scope", stack, statements, null_result(), 0);

        Result result = exec_op(&active_scope, new_peek());

        ASSERT_INT_EQUALS(active_scope.stack.len, 5);
        ASSERT_INT_EQUALS(result.is_byte, 1);
        ASSERT_INT_EQUALS(result.is_null, 0);
        ASSERT_CHAR_EQUALS(result.byte, 't');
    })
    TEST("executes put, returns null", {
        Byte stack_vals[] = ARRAY('c', '\0', 255, 'x', 't');
        Byte * mem = malloc(sizeof(Byte) * (LEN(stack_vals)));
        memcpy(mem, stack_vals, sizeof(Byte) *(LEN(stack_vals)));
        ByteVector stack = ARRAY(mem, LEN(stack_vals));
        ActiveScope active_scope = ARRAY("test_scope", stack, statements, null_result(), 0);

        Result result = exec_op(&active_scope, new_put('Y'));

        ASSERT_INT_EQUALS(active_scope.stack.len, 6);
        ASSERT_INT_EQUALS(result.is_byte, 0);
        ASSERT_INT_EQUALS(result.is_null, 1);
        ASSERT_INT_EQUALS(stack.arr[5], 'Y');
    })
    TEST("executes take, returns top of stack and removes it", {
        Byte stack_vals[] = ARRAY('c', '\0', 255, 'x', 't');
        Byte * mem = malloc(sizeof(Byte) * (LEN(stack_vals)));
        memcpy(mem, stack_vals, sizeof(Byte) *(LEN(stack_vals)));
        ByteVector stack = ARRAY(mem, LEN(stack_vals));
        ActiveScope active_scope = ARRAY("test_scope", stack, statements, null_result(), 0);

        Result result = exec_op(&active_scope, new_take());

        ASSERT_INT_EQUALS(active_scope.stack.len, 4);
        ASSERT_INT_EQUALS(result.is_byte, 1);
        ASSERT_INT_EQUALS(result.is_null, 0);
        ASSERT_INT_EQUALS(result.byte, 't');
    })
});

