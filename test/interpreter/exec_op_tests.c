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
        memcpy(mem, stack_vals, sizeof(Byte) * (LEN(stack_vals)));
        ByteVector stack = ARRAY(mem, LEN(stack_vals));
        ActiveScope active_scope = ARRAY("test_scope", stack, statements, null_result(), 0);

        Result result = exec_op(&active_scope, new_put('Y'));

        ASSERT_INT_EQUALS(active_scope.stack.len, 6);
        ASSERT_INT_EQUALS(result.is_byte, 0);
        ASSERT_INT_EQUALS(result.is_null, 1);
        ASSERT_INT_EQUALS(active_scope.stack.arr[5], 'Y');
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
    TEST("executes take, returns top of stack and removes it, leaving it empty", {
        Byte stack_vals[] = ARRAY('t');
        Byte * mem = malloc(sizeof(Byte) * (LEN(stack_vals)));
        memcpy(mem, stack_vals, sizeof(Byte) *(LEN(stack_vals)));
        ByteVector stack = ARRAY(mem, LEN(stack_vals));
        ActiveScope active_scope = ARRAY("test_scope", stack, statements, null_result(), 0);

        Result result = exec_op(&active_scope, new_take());

        ASSERT_INT_EQUALS(active_scope.stack.len, 0);
        ASSERT_INT_EQUALS(result.is_byte, 1);
        ASSERT_INT_EQUALS(result.is_null, 0);
        ASSERT_INT_EQUALS(result.byte, 't');
    })
    TEST("executes and, returns result", {
        Byte stack_vals[] = ARRAY('c', '\0', 255, 'x', 't');
        Byte * mem = malloc(sizeof(Byte) * (LEN(stack_vals)));
        memcpy(mem, stack_vals, sizeof(Byte) *(LEN(stack_vals)));
        ByteVector stack = ARRAY(mem, LEN(stack_vals));
        ActiveScope active_scope = ARRAY("test_scope", stack, statements, null_result(), 0);

        Result result1 = exec_op(&active_scope, new_and(2, 254));

        ASSERT_INT_EQUALS(active_scope.stack.len, 5);
        ASSERT_INT_EQUALS(result1.is_byte, 1);
        ASSERT_INT_EQUALS(result1.is_null, 0);
        ASSERT_INT_EQUALS(result1.byte, 2);

        Result result2 = exec_op(&active_scope, new_and(1, 254));

        ASSERT_INT_EQUALS(result2.is_byte, 1);
        ASSERT_INT_EQUALS(result2.is_null, 0);
        ASSERT_INT_EQUALS(result2.byte, 0);
    })
    TEST("executes or, returns result", {
        Byte stack_vals[] = ARRAY('c', '\0', 255, 'x', 't');
        Byte * mem = malloc(sizeof(Byte) * (LEN(stack_vals)));
        memcpy(mem, stack_vals, sizeof(Byte) *(LEN(stack_vals)));
        ByteVector stack = ARRAY(mem, LEN(stack_vals));
        ActiveScope active_scope = ARRAY("test_scope", stack, statements, null_result(), 0);


        Result result1 = exec_op(&active_scope, new_or(2, 254));

        ASSERT_INT_EQUALS(active_scope.stack.len, 5);
        ASSERT_INT_EQUALS(result1.is_byte, 1);
        ASSERT_INT_EQUALS(result1.is_null, 0);
        ASSERT_INT_EQUALS(result1.byte, 254);

        Result result2 = exec_op(&active_scope, new_or(1, 254));

        ASSERT_INT_EQUALS(result2.is_byte, 1);
        ASSERT_INT_EQUALS(result2.is_null, 0);
        ASSERT_INT_EQUALS(result2.byte, 255);
    })
    TEST("executes not, returns result", {
        Byte stack_vals[] = ARRAY('c', '\0', 255, 'x', 't');
        Byte * mem = malloc(sizeof(Byte) * (LEN(stack_vals)));
        memcpy(mem, stack_vals, sizeof(Byte) *(LEN(stack_vals)));
        ByteVector stack = ARRAY(mem, LEN(stack_vals));
        ActiveScope active_scope = ARRAY("test_scope", stack, statements, null_result(), 0);


        Result result1 = exec_op(&active_scope, new_not(0));

        ASSERT_INT_EQUALS(active_scope.stack.len, 5);
        ASSERT_INT_EQUALS(result1.is_byte, 1);
        ASSERT_INT_EQUALS(result1.is_null, 0);
        ASSERT_INT_EQUALS(result1.byte, 255);

        Result result2 = exec_op(&active_scope, new_not(1));

        ASSERT_INT_EQUALS(result2.is_byte, 1);
        ASSERT_INT_EQUALS(result2.is_null, 0);
        ASSERT_INT_EQUALS(result2.byte, 254);
    })
    TEST("executes xor, returns result", {
        Byte stack_vals[] = ARRAY('c', '\0', 255, 'x', 't');
        Byte * mem = malloc(sizeof(Byte) * (LEN(stack_vals)));
        memcpy(mem, stack_vals, sizeof(Byte) *(LEN(stack_vals)));
        ByteVector stack = ARRAY(mem, LEN(stack_vals));
        ActiveScope active_scope = ARRAY("test_scope", stack, statements, null_result(), 0);


        Result result1 = exec_op(&active_scope, new_xor(1, 254));

        ASSERT_INT_EQUALS(active_scope.stack.len, 5);
        ASSERT_INT_EQUALS(result1.is_byte, 1);
        ASSERT_INT_EQUALS(result1.is_null, 0);
        ASSERT_INT_EQUALS(result1.byte, 255);

        Result result2 = exec_op(&active_scope, new_xor(1,255));

        ASSERT_INT_EQUALS(result2.is_byte, 1);
        ASSERT_INT_EQUALS(result2.is_null, 0);
        ASSERT_INT_EQUALS(result2.byte, 254);
    })
    TEST("executes eq, returns result", {
        Byte stack_vals[] = ARRAY('c', '\0', 255, 'x', 't');
        Byte * mem = malloc(sizeof(Byte) * (LEN(stack_vals)));
        memcpy(mem, stack_vals, sizeof(Byte) *(LEN(stack_vals)));
        ByteVector stack = ARRAY(mem, LEN(stack_vals));
        ActiveScope active_scope = ARRAY("test_scope", stack, statements, null_result(), 0);


        Result result1 = exec_op(&active_scope, new_eq(1, 254));

        ASSERT_INT_EQUALS(active_scope.stack.len, 5);
        ASSERT_INT_EQUALS(result1.is_byte, 1);
        ASSERT_INT_EQUALS(result1.is_null, 0);
        ASSERT_INT_EQUALS(result1.byte, 0);

        Result result2 = exec_op(&active_scope, new_eq(123,123));

        ASSERT_INT_EQUALS(result2.is_byte, 1);
        ASSERT_INT_EQUALS(result2.is_null, 0);
        ASSERT_INT_EQUALS(result2.byte, 255);
    })
    TEST("executes neq, returns result", {
        Byte stack_vals[] = ARRAY('c', '\0', 255, 'x', 't');
        Byte * mem = malloc(sizeof(Byte) * (LEN(stack_vals)));
        memcpy(mem, stack_vals, sizeof(Byte) *(LEN(stack_vals)));
        ByteVector stack = ARRAY(mem, LEN(stack_vals));
        ActiveScope active_scope = ARRAY("test_scope", stack, statements, null_result(), 0);


        Result result1 = exec_op(&active_scope, new_neq(1, 254));

        ASSERT_INT_EQUALS(active_scope.stack.len, 5);
        ASSERT_INT_EQUALS(result1.is_byte, 1);
        ASSERT_INT_EQUALS(result1.is_null, 0);
        ASSERT_INT_EQUALS(result1.byte, 255);

        Result result2 = exec_op(&active_scope, new_neq(123,123));

        ASSERT_INT_EQUALS(result2.is_byte, 1);
        ASSERT_INT_EQUALS(result2.is_null, 0);
        ASSERT_INT_EQUALS(result2.byte, 0);
    })
    TEST("executes return, sets scope return result and is_returned", {
        Byte stack_vals[] = ARRAY('c', '\0', 255, 'x', 't');
        Byte * mem = malloc(sizeof(Byte) * (LEN(stack_vals)));
        memcpy(mem, stack_vals, sizeof(Byte) *(LEN(stack_vals)));
        ByteVector stack = ARRAY(mem, LEN(stack_vals));
        ActiveScope active_scope = ARRAY("test_scope", stack, statements, null_result(), 0);

        Result result = exec_op(&active_scope, new_return(new_byte_argument(123)));

        ASSERT_INT_EQUALS(result.is_byte, 0);
        ASSERT_INT_EQUALS(result.is_null, 1);
        ASSERT_INT_EQUALS(active_scope.is_returned, 1);
        ASSERT_INT_EQUALS(active_scope.return_result.is_byte, 1);
        ASSERT_INT_EQUALS(active_scope.return_result.byte, 123);
    })
    TEST("executes an if, puts on stack bacause condition is [1]", {
        Byte stack_vals[] = ARRAY('c', '\0', 255, 'x', 't');
        Byte * mem = malloc(sizeof(Byte) * (LEN(stack_vals)));
        memcpy(mem, stack_vals, sizeof(Byte) *(LEN(stack_vals)));
        ByteVector stack = ARRAY(mem, LEN(stack_vals));
        ActiveScope active_scope = ARRAY("test_scope", stack, statements, null_result(), 0);

        GenericOp if_generic_ops[] = ARRAY(new_put(5));
        Sequence if_ops = ARRAY(if_generic_ops, LEN(if_generic_ops));

        Result result = exec_op(&active_scope, new_if(new_byte_argument(255), new_sequence_argument(if_ops)));

        ASSERT_INT_EQUALS(result.is_byte, 0);
        ASSERT_INT_EQUALS(result.is_null, 1);
        ASSERT_INT_EQUALS(active_scope.stack.len, 6);
        ASSERT_INT_EQUALS(active_scope.stack.arr[5], 5)
    })

    TEST("executes an if, does nothing because [0]", {
        Byte stack_vals[] = ARRAY('c', '\0', 255, 'x', 't');
        Byte * mem = malloc(sizeof(Byte) * (LEN(stack_vals)));
        memcpy(mem, stack_vals, sizeof(Byte) *(LEN(stack_vals)));
        ByteVector stack = ARRAY(mem, LEN(stack_vals));
        ActiveScope active_scope = ARRAY("test_scope", stack, statements, null_result(), 0);


        GenericOp if_generic_ops[] = ARRAY(new_put(5));
        Sequence if_ops = ARRAY(if_generic_ops, LEN(if_generic_ops));

        Result result = exec_op(&active_scope, new_if(new_byte_argument(0), new_sequence_argument(if_ops)));

        ASSERT_INT_EQUALS(result.is_byte, 0);
        ASSERT_INT_EQUALS(result.is_null, 1);
        ASSERT_INT_EQUALS(active_scope.stack.len, 5);
        ASSERT_INT_EQUALS(active_scope.stack.arr[4], 't');
    })
});

