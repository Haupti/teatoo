#include "../parser/parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utils/checked_alloc.h"
#include "result/result.h"
#include "active_scope.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0')

void interpreter_err(char * msg){
    printf("ERROR:");
    printf("%s\n", msg);
    exit(EXIT_FAILURE);
}

void interpreter_err_in(char * msg, char * scope_name){
    printf("ERROR in execution of scope %s:", scope_name);
    printf("%s\n", msg);
    exit(EXIT_FAILURE);
}

void check_expect_scope(Argument arg){
    if(arg.is_byte){
        interpreter_err("expected a scope argument, found byte");
    }
    else if(arg.is_sequence){
        interpreter_err("expected a scope argument, found sequence");
    }
}


Result exec_arg(ActiveScope * context, Argument arg);

Result get_top_of_stack(ByteVector bytes){
    if(bytes.len == 0){
        return null_result();
    }
    else {
        return byte_result(bytes.arr[bytes.len -1]);
    }
}

Result exec_op(ActiveScope * context, GenericOp op){
    switch(op.type){
        case OT_PEEK:{
            return get_top_of_stack(context->stack);
            break;
        }
        case OT_TAKE:{
            Result result = get_top_of_stack(context->stack);

            if(context->stack.len == 0){
                return result;
            }
            else if(context->stack.len >= 1){
                context->stack.len = context->stack.len - 1;
                context->stack.arr = checked_realloc(context->stack.arr, sizeof(Byte) * context->stack.len);
                return result;
            }
        }
        case OT_PUT:{
            Result result = exec_arg(context, op.op.op_put.first);
            if(result.is_null){
                interpreter_err_in("NULL ARGUMENT ERROR ('PUT')", context->name);
            }
            else {
                context->stack.len = context->stack.len + 1;
                context->stack.arr = checked_realloc(context->stack.arr, sizeof(Byte) * (context->stack.len));
            }
            break;
        }
        case OT_AND:{
            Result first = exec_arg(context, op.op.op_and.first);
            Result second = exec_arg(context, op.op.op_and.second);
            if(first.is_null || second.is_null){
                interpreter_err_in("NULL ARGUMENT ERROR ('AND')", context->name);
            }
            Result result = byte_result(first.byte & second.byte);
            return result;
        }
        case OT_IF:{
            Result condition_result = exec_arg(context, op.op.op_if.condition);
            if(condition_result.is_null){
                interpreter_err_in("NULL ARGUMENT ERROR ('IF')", context->name);
            }
            if(condition_result.byte == 0xFF){
                return exec_arg(context, op.op.op_if.operation);
            }
            else {
                return null_result();
            }
            break;
        }
        case OT_EQ:{
            Result first = exec_arg(context, op.op.op_eq.first);
            Result second = exec_arg(context, op.op.op_eq.second);
            if(first.is_null || second.is_null){
                interpreter_err_in("NULL ARGUMENT ERROR ('EQ')", context->name);
            }
            Result result;
            if(first.byte == second.byte){
                result = byte_result(0xFF);
            }
            else {
                result = byte_result(0x0);
            }
            return result;
        }
        case OT_NEQ:{
            Result first = exec_arg(context, op.op.op_neq.first);
            Result second = exec_arg(context, op.op.op_neq.second);
            if(first.is_null || second.is_null){
                interpreter_err_in("NULL ARGUMENT ERROR ('NEQ')", context->name);
            }
            Result result;
            if(first.byte == second.byte){
                result = byte_result(0x0);
            }
            else {
                result = byte_result(0xFF);
            }
            return result;
        }
        case OT_OR:{
            Result first = exec_arg(context, op.op.op_or.first);
            Result second = exec_arg(context, op.op.op_or.second);
            if(first.is_null || second.is_null){
                interpreter_err_in("NULL ARGUMENT ERROR ('OR')", context->name);
            }
            Result result = byte_result(first.byte | second.byte);
            return result;
        }
        case OT_XOR:{
            Result first = exec_arg(context, op.op.op_xor.first);
            Result second = exec_arg(context, op.op.op_xor.second);
            if(first.is_null || second.is_null){
                interpreter_err_in("NULL ARGUMENT ERROR ('XOR')", context->name);
            }
            Result result = byte_result(first.byte ^ second.byte);
            return result;
        }
        case OT_NOT:{
            Result first = exec_arg(context, op.op.op_not.first);
            if(first.is_null){
                interpreter_err_in("NULL ARGUMENT ERROR ('NOT')", context->name);
            }
            Result result = byte_result(~first.byte);
            return result;
        }
        case OT_OUT:{
            Result first = exec_arg(context, op.op.op_out.first);
            if(first.is_null){
                interpreter_err_in("NULL ARGUMENT ERROR ('OUT')", context->name);
            }
            printf(BYTE_TO_BINARY_PATTERN" ", BYTE_TO_BINARY(first.byte));
            break;
        }
        case OT_OUTCHAR:{
            Result first = exec_arg(context, op.op.op_outchar.first);
            if(first.is_null){
                interpreter_err_in("NULL ARGUMENT ERROR ('OUTCHAR')", context->name);
            }
            printf("%c", first.byte);
            break;
        }
        case OT_OUTNUM:{
            Result first = exec_arg(context, op.op.op_outnum.first);
            if(first.is_null){
                interpreter_err_in("NULL ARGUMENT ERROR ('OUTNUM')", context->name);
            }
            printf("%d", first.byte);
            break;
        }
        case OT_RETURN:{
            Result first = exec_arg(context, op.op.op_return.first);
            if(first.is_null){
                interpreter_err_in("NULL ARGUMENT ERROR ('RETURN')", context->name);
            }
            return first;
            break;
        }
        case OT_EXEC:{
            interpreter_err_in("EXEX is not allowed inside a scope", context->name);
        };
    }
    return null_result();
}

Result exec_arg(ActiveScope * context, Argument arg){
    if(arg.is_byte){
        Result result = byte_result(arg.byte);
        return result;
    }
    else if(arg.is_sequence){
        for(int i = 0; i<arg.sequence.op_count; i++){
            if(i == arg.sequence.op_count -1){
                return exec_op(context, arg.sequence.ops[i]);
            }
            else {
                exec_op(context, arg.sequence.ops[i]);
                return null_result();
            }
        }
    }
    return null_result();
}

Result execute_scope(Module context, Scope scope, int is_copy){
    ByteVector stack = scope.stack;
    int stack_top = stack.len - 1;
    Statements statements = scope.statements;
    ActiveScope active_scope = {scope.name, scope.stack, scope.statements, null_result(), 0};

    for(int i=0; i<scope.statements.statements_len; i++){
        GenericOp op = statements.statements[i];
        if(active_scope.is_returned){
            return active_scope.return_result;
        }
        exec_op(&active_scope, op);
    }
    return null_result();
}


Result interpret_exec(Module context, Op_EXEC exec){
    check_expect_scope(exec.first);
    int is_copy;
    if(exec.first.is_copy_ref){
        is_copy = 1;
    }
    else if(exec.first.is_ref){
        is_copy = 0;
    }
    for(int i = 0; i < context.scopes.len; i++){
        if(strcmp(exec.first.scope_name, context.scopes.arr[i].name) == 0){
            Scope match = context.scopes.arr[i];
            return execute_scope(context, match, is_copy);
        }
    }
    return null_result();
}

int interpret(Module mod){
    if(mod.has_entrypoint){
        interpret_exec(mod, mod.entrypoint);
    }
    return EXIT_SUCCESS;
}
