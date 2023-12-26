#include "../../lib/test/assert.h"
#include <stdio.h>
#include <string.h>
#include "../../src/parser/instruction/instruction.h"
#include "interpreter_internal_header.h"

MODULAR_DESCRIBE(exec_op_tests, {
    Statements statements = ARRAY(NULL, 0);
    Module module = {};

    TEST("executes peek, returns null because stack is empty", {
        Byte empty_stack_vals[] = ARRAY();
        ByteVector empty_stack = ARRAY(empty_stack_vals, 0);
        ActiveScope active_scope_empty_stack = ARRAY("test_scope_empty_stack", empty_stack, statements, null_result(), 0);

        Result result = exec_op(&module, &active_scope_empty_stack, new_peek());

        ASSERT_INT_EQUALS(active_scope_empty_stack.stack.len, 0);
        ASSERT_INT_EQUALS(result.is_byte, 0);
        ASSERT_INT_EQUALS(result.is_null, 1);
    })
    TEST("executes EMPTY?, returns 255 because stack is empty", {
        Byte empty_stack_vals[] = ARRAY();
        ByteVector empty_stack = ARRAY(empty_stack_vals, 0);
        ActiveScope active_scope_empty_stack = ARRAY("test_scope_empty_stack", empty_stack, statements, null_result(), 0);

        Result result = exec_op(&module, &active_scope_empty_stack, new_is_empty());

        ASSERT_INT_EQUALS(active_scope_empty_stack.stack.len, 0);
        ASSERT_INT_EQUALS(result.is_byte, 1);
        ASSERT_INT_EQUALS(result.byte, 255);
    })
    TEST("executes EMPTY?, returns 0 because stack is not empty", {
        Byte stack_vals[] = ARRAY(123);
        ByteVector stack = ARRAY(stack_vals, LEN(stack_vals));
        ActiveScope active_scope = ARRAY("test_scope_empty_stack", stack, statements, null_result(), 0);

        Result result = exec_op(&module, &active_scope, new_is_empty());

        ASSERT_INT_EQUALS(active_scope.stack.len, 1);
        ASSERT_INT_EQUALS(result.is_byte, 1);
        ASSERT_INT_EQUALS(result.byte, 0);
    })
    TEST("executes peek, returns last of stack (top)", {
        Byte stack_vals[] = ARRAY('c', '\0', 255, 'x', 't');
        ByteVector stack = ARRAY(stack_vals, 5);
        ActiveScope active_scope = ARRAY("test_scope", stack, statements, null_result(), 0);

        Result result = exec_op(&module, &active_scope, new_peek());

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

        Result result = exec_op(&module, &active_scope, new_put(new_byte_argument('Y')));

        ASSERT_INT_EQUALS(active_scope.stack.len, 6);
        ASSERT_INT_EQUALS(result.is_byte, 0);
        ASSERT_INT_EQUALS(result.is_null, 1);
        ASSERT_INT_EQUALS(active_scope.stack.arr[5], 'Y');
    })
    TEST("executes NULL?, returns 255 because arg is null", {
        Byte stack_vals[] = ARRAY();
        ByteVector stack = ARRAY(NULL, 0);
        ActiveScope active_scope = ARRAY("test_scope", stack, statements, null_result(), 0);

        Result result = exec_op(&module, &active_scope, new_is_null(new_null_argument()));

        ASSERT_INT_EQUALS(result.is_byte, 1);
        ASSERT_INT_EQUALS(result.is_null, 0);
        ASSERT_INT_EQUALS(result.byte, 255);
    })
    TEST("executes take, returns top of stack and removes it", {
        Byte stack_vals[] = ARRAY('c', '\0', 255, 'x', 't');
        Byte * mem = malloc(sizeof(Byte) * (LEN(stack_vals)));
        memcpy(mem, stack_vals, sizeof(Byte) *(LEN(stack_vals)));
        ByteVector stack = ARRAY(mem, LEN(stack_vals));
        ActiveScope active_scope = ARRAY("test_scope", stack, statements, null_result(), 0);

        Result result = exec_op(&module, &active_scope, new_take());

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

        Result result = exec_op(&module, &active_scope, new_take());

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

        Result result1 = exec_op(&module, &active_scope, new_and(new_byte_argument(2), new_byte_argument(254)));

        ASSERT_INT_EQUALS(active_scope.stack.len, 5);
        ASSERT_INT_EQUALS(result1.is_byte, 1);
        ASSERT_INT_EQUALS(result1.is_null, 0);
        ASSERT_INT_EQUALS(result1.byte, 2);

        Result result2 = exec_op(&module, &active_scope, new_and(new_byte_argument(1), new_byte_argument(254)));

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


        Result result1 = exec_op(&module, &active_scope, new_or(new_byte_argument(2), new_byte_argument(254)));

        ASSERT_INT_EQUALS(active_scope.stack.len, 5);
        ASSERT_INT_EQUALS(result1.is_byte, 1);
        ASSERT_INT_EQUALS(result1.is_null, 0);
        ASSERT_INT_EQUALS(result1.byte, 254);

        Result result2 = exec_op(&module, &active_scope, new_or(new_byte_argument(1), new_byte_argument(254)));

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


        Result result1 = exec_op(&module, &active_scope, new_not(new_byte_argument(0)));

        ASSERT_INT_EQUALS(active_scope.stack.len, 5);
        ASSERT_INT_EQUALS(result1.is_byte, 1);
        ASSERT_INT_EQUALS(result1.is_null, 0);
        ASSERT_INT_EQUALS(result1.byte, 255);

        Result result2 = exec_op(&module, &active_scope, new_not(new_byte_argument(1)));

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


        Result result1 = exec_op(&module, &active_scope, new_xor(new_byte_argument(1), new_byte_argument(254)));

        ASSERT_INT_EQUALS(active_scope.stack.len, 5);
        ASSERT_INT_EQUALS(result1.is_byte, 1);
        ASSERT_INT_EQUALS(result1.is_null, 0);
        ASSERT_INT_EQUALS(result1.byte, 255);

        Result result2 = exec_op(&module, &active_scope, new_xor(new_byte_argument(1),new_byte_argument(255)));

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


        Result result1 = exec_op(&module, &active_scope, new_eq(new_byte_argument(1), new_byte_argument(254)));

        ASSERT_INT_EQUALS(active_scope.stack.len, 5);
        ASSERT_INT_EQUALS(result1.is_byte, 1);
        ASSERT_INT_EQUALS(result1.is_null, 0);
        ASSERT_INT_EQUALS(result1.byte, 0);

        Result result2 = exec_op(&module, &active_scope, new_eq(new_byte_argument(123),new_byte_argument(123)));

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


        Result result1 = exec_op(&module, &active_scope, new_neq(new_byte_argument(1), new_byte_argument(254)));

        ASSERT_INT_EQUALS(active_scope.stack.len, 5);
        ASSERT_INT_EQUALS(result1.is_byte, 1);
        ASSERT_INT_EQUALS(result1.is_null, 0);
        ASSERT_INT_EQUALS(result1.byte, 255);

        Result result2 = exec_op(&module, &active_scope, new_neq(new_byte_argument(123),new_byte_argument(123)));

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

        Result result = exec_op(&module, &active_scope, new_return(new_byte_argument(123)));

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

        GenericOp if_generic_ops[] = ARRAY(new_put(new_byte_argument(5)));
        Sequence if_ops = ARRAY(if_generic_ops, LEN(if_generic_ops));

        Result result = exec_op(&module, &active_scope, new_if(new_byte_argument(255), new_sequence_argument(if_ops)));

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


        GenericOp if_generic_ops[] = ARRAY(new_put(new_byte_argument(5)));
        Sequence if_ops = ARRAY(if_generic_ops, LEN(if_generic_ops));

        Result result = exec_op(&module, &active_scope, new_if(new_byte_argument(0), new_sequence_argument(if_ops)));

        ASSERT_INT_EQUALS(result.is_byte, 0);
        ASSERT_INT_EQUALS(result.is_null, 1);
        ASSERT_INT_EQUALS(active_scope.stack.len, 5);
        ASSERT_INT_EQUALS(active_scope.stack.arr[4], 't');
    })
    TEST("executes scope exec within scope", {
        ByteVector stack = ARRAY(NULL, 0);
        Scope scope1 = ARRAY("test_scope_1", stack, statements);
        ActiveScope active_scope = ARRAY("test_scope_1", stack, statements, null_result(), 0);

        GenericOp args2[] = ARRAY(new_return(new_byte_argument(80)));
        Statements statements2 = ARRAY(args2,1);
        Scope scope2 = ARRAY("test_scope_2", stack, statements2);

        Scope scopes[] = ARRAY(scope1, scope2);
        ScopeVector scope_vec = ARRAY(scopes, 2);
        Module module = ARRAY(scope_vec, (Op_EXEC){}, 0);

        Argument scope_arg = new_scope_copy_ref_argument("test_scope_2");
        Result result = exec_op(&module, &active_scope, new_exec(scope_arg));

        ASSERT_INT_EQUALS(result.is_byte, 1);
        ASSERT_INT_EQUALS(result.is_null, 0);
        ASSERT_INT_EQUALS(result.byte, 80);
        ASSERT_INT_EQUALS(active_scope.is_returned, 0);
    })
    TEST("executes STACK on given stack and returns it", {
        Byte stack_vals[] = ARRAY();
        Byte * mem = malloc(sizeof(Byte) * (LEN(stack_vals)));
        memcpy(mem, stack_vals, sizeof(Byte) *(LEN(stack_vals)));
        ByteVector stack = ARRAY(mem, LEN(stack_vals));
        Scope scope = ARRAY("test_scope", stack, statements);

        Byte stack_vals_active[] = ARRAY();
        Byte * mem_active = malloc(sizeof(Byte) * (LEN(stack_vals_active)));
        memcpy(mem_active, stack_vals_active, sizeof(Byte) *(LEN(stack_vals_active)));
        ByteVector stack_active = ARRAY(mem_active, LEN(stack_vals_active));
        ActiveScope active_scope = ARRAY("test_scope_active", stack_active, statements, null_result(), 0);

        GenericOp stack_gen_op = new_stack(new_scope_ref_argument("test_scope"), new_byte_argument(5));
        ScopeVector scopes = ARRAY(&scope, 1);
        Module test_module = ARRAY(scopes, (Op_EXEC) {});

        Result result = exec_op(&test_module, &active_scope, stack_gen_op);

        ASSERT_INT_EQUALS(result.is_scope, 1);
        ASSERT_STR_EQUALS(result.scope_name, "test_scope");
        ASSERT_INT_EQUALS(scope.stack.len, 1);
        ASSERT_INT_EQUALS(scope.stack.arr[0], 5)
    })
    TEST("executes MODULO, returns result", {
        Byte stack_vals[] = ARRAY('c', '\0', 255, 'x', 't');
        Byte * mem = malloc(sizeof(Byte) * (LEN(stack_vals)));
        memcpy(mem, stack_vals, sizeof(Byte) *(LEN(stack_vals)));
        ByteVector stack = ARRAY(mem, LEN(stack_vals));
        ActiveScope active_scope = ARRAY("test_scope", stack, statements, null_result(), 0);


        Result result1 = exec_op(&module, &active_scope, new_modulo(new_byte_argument(20), new_byte_argument(3)));

        ASSERT_INT_EQUALS(active_scope.stack.len, 5);
        ASSERT_INT_EQUALS(result1.is_byte, 1);
        ASSERT_INT_EQUALS(result1.is_null, 0);
        ASSERT_INT_EQUALS(result1.byte, 2);

        Result result2 = exec_op(&module, &active_scope, new_modulo(new_byte_argument(20),new_byte_argument(2)));

        ASSERT_INT_EQUALS(result2.is_byte, 1);
        ASSERT_INT_EQUALS(result2.is_null, 0);
        ASSERT_INT_EQUALS(result2.byte, 0);
    })
});

