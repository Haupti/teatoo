#include "../../lib/test/assert.h"
#include <stdio.h>
#include <string.h>
#include "../../src/parser/instruction/instruction.h"
#include "../../src/parser/parser.h"
#include "../../src/interpreter/module/module.h"
#include "../../src/interpreter/result/result.h"
#include "interpreter_internal_header.h"

MODULAR_DESCRIBE(exec_scope_tests, {
    Statements statements = ARRAY(NULL, 0);

    TEST("executes scope, copies it and thus does not modifie the original one", {
        GenericOp ops[] = ARRAY(new_put(1), new_return(new_byte_argument(2)));
        Statements statements = new_statements(ops, LEN(ops));

        Scope scope = new_scope("steve", statements);
        Scope scopes[] = ARRAY(scope);
        ScopeVector scope_vec = ARRAY(scopes , LEN(scopes));

        Argument arg = new_scope_copy_ref_argument("steve");
        Op_EXEC exec = new_exec(arg).op.op_exec;
        Module module = new_module(scope_vec, exec);

        Result result = execute_scope(&module, "steve", 1);

        ASSERT_INT_EQUALS(result.is_byte, 1);
        ASSERT_INT_EQUALS(result.is_null, 0);
        ASSERT_INT_EQUALS(result.byte, 2);

        ASSERT_INT_EQUALS(module.scopes.arr[0].stack.len, 0);
    })

    TEST("executes scope, modifies it (persistent)", {
        GenericOp ops[] = ARRAY(new_put(1), new_return(new_byte_argument(2)));
        Statements statements = new_statements(ops, LEN(ops));

        Scope scope = new_scope("steve", statements);
        Scope scopes[] = ARRAY(scope);
        ScopeVector scope_vec = ARRAY(scopes , LEN(scopes));

        Argument arg = new_scope_ref_argument("steve");
        Op_EXEC exec = new_exec(arg).op.op_exec;
        Module module = new_module(scope_vec, exec);

        Result result = execute_scope(&module, "steve", 0);

        ASSERT_INT_EQUALS(result.is_byte, 1);
        ASSERT_INT_EQUALS(result.is_null, 0);
        ASSERT_INT_EQUALS(result.byte, 2);

        ASSERT_INT_EQUALS(module.scopes.arr[0].stack.len, 1);
        ASSERT_INT_EQUALS(module.scopes.arr[0].stack.arr[0], 1);
    })
})
