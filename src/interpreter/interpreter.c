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

Result exec_arg(Module * module, ActiveScope * context, Argument arg);
Result execute_scope(Module * context, char * scope_name, int is_copy);
Result execute_whole_scope(Module * context, Scope scope);
int find_index_of_scope_by_name(Module * module, char * name);

Result get_top_of_stack(ByteVector bytes){
    if(bytes.len == 0){
        return null_result();
    }
    else {
        return byte_result(bytes.arr[bytes.len - 1]);
    }
}

void put_on_stack(ActiveScope * scope, Byte byte){
    scope->stack.len = scope->stack.len + 1;
    scope->stack.arr = checked_realloc(scope->stack.arr, sizeof(Byte) * (scope->stack.len));
    scope->stack.arr[scope->stack.len -1] = byte;
}

void put_on_stack_of(Scope * scope, Byte byte){
    scope->stack.len = scope->stack.len + 1;
    scope->stack.arr = checked_realloc(scope->stack.arr, sizeof(Byte) * (scope->stack.len));
    scope->stack.arr[scope->stack.len -1] = byte;
}

void remove_head_from_stack(ActiveScope * scope){
    if(scope->stack.len == 0){
        return;
    }
    if(scope->stack.len == 1){
        scope->stack.len = scope->stack.len - 1;
        free(scope->stack.arr);
        scope->stack.arr = NULL;
    }
    else if(scope->stack.len > 1){
        scope->stack.len = scope->stack.len - 1;
        scope->stack.arr = checked_realloc(scope->stack.arr, sizeof(Byte) * scope->stack.len);
    }
}

Result exec_op(Module * module, ActiveScope * context, GenericOp op){
    switch(op.type){
        case OT_PEEK:{
            return get_top_of_stack(context->stack);
            break;
        }
        case OT_IS_EMPTY:{
            Result res =  get_top_of_stack(context->stack);
            if(res.is_null){
                return byte_result(255);
            }
            else{
                return byte_result(0);
            }
            break;
        }
        case OT_TAKE:{
            Result result = get_top_of_stack(context->stack);
            remove_head_from_stack(context);
            return result;
        }
        case OT_PUT:{
            Result result = exec_arg(module, context, op.op.op_put.first);
            if(result.is_null){
                interpreter_err_in("NULL ARGUMENT ERROR ('PUT')", context->name);
            }
            else {
                put_on_stack(context, result.byte);
            }
            break;
        }
        case OT_AND:{
            Result first = exec_arg(module, context, op.op.op_and.first);
            Result second = exec_arg(module, context, op.op.op_and.second);
            if(first.is_null || second.is_null){
                interpreter_err_in("NULL ARGUMENT ERROR ('AND')", context->name);
            }
            Result result = byte_result(first.byte & second.byte);
            return result;
        }
        case OT_IF:{
            Result condition_result = exec_arg(module, context, op.op.op_if.condition);
            if(condition_result.is_null){
                interpreter_err_in("NULL ARGUMENT ERROR ('IF')", context->name);
            }
            if(condition_result.byte == 0xFF){
                return exec_arg(module, context, op.op.op_if.operation);
            }
            else {
                return null_result();
            }
            break;
        }
        case OT_EQ:{
            Result first = exec_arg(module, context, op.op.op_eq.first);
            Result second = exec_arg(module, context, op.op.op_eq.second);
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
            Result first = exec_arg(module, context, op.op.op_neq.first);
            Result second = exec_arg(module, context, op.op.op_neq.second);
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
        case OT_STACK:{
            Result first = exec_arg(module, context, op.op.op_stack.first);
            Result second = exec_arg(module, context, op.op.op_stack.second);

            if(first.is_null || second.is_null){
                interpreter_err_in("NULL ARGUMENT ERROR ('STACK')", context->name);
            }
            else if(!first.is_scope){
                interpreter_err_in("SCOPE EXPECTED AS FIRST ARGUMENT ERROR ('STACK')", context->name);
            }
            if(!second.is_byte){
                interpreter_err_in("BYTE EXPECTED AS SECOND ARGUMENT ERROR ('STACK')", context->name);
            }

            if(!first.is_copy){
                int scope_index = find_index_of_scope_by_name(module, first.scope_name);
                Scope * scope = &module->scopes.arr[scope_index];
                put_on_stack_of(scope, second.byte);
                Result result = scope_result(first.scope_name, first.is_copy);
                return result;
            }
            else{
                int scope_index = find_index_of_scope_by_name(module, first.scope_name);
                Scope scope = module->scopes.arr[scope_index];

                Byte * persistent_stack = malloc(sizeof(Byte) * scope.stack.len + 1);
                persistent_stack[scope.stack.len] = second.byte;
                ByteVector vec = {persistent_stack, scope.stack.len + 1};

                scope.stack = vec;
                Result result = whole_scope_result(scope);
                return result;
            }
        }
        case OT_OR:{
            Result first = exec_arg(module, context, op.op.op_or.first);
            Result second = exec_arg(module, context, op.op.op_or.second);
            if(first.is_null || second.is_null){
                interpreter_err_in("NULL ARGUMENT ERROR ('OR')", context->name);
            }
            else if(!first.is_byte || !second.is_byte){
                interpreter_err_in("BYTE ARGUMENTS EXPECTED ERROR ('OR')", context->name);
            }
            Result result = byte_result(first.byte | second.byte);
            return result;
        }
        case OT_XOR:{
            Result first = exec_arg(module, context, op.op.op_xor.first);
            Result second = exec_arg(module, context, op.op.op_xor.second);
            if(first.is_null || second.is_null){
                interpreter_err_in("NULL ARGUMENT ERROR ('XOR')", context->name);
            }
            else if(!first.is_byte || !second.is_byte){
                interpreter_err_in("BYTE ARGUMENTS EXPECTED ERROR ('XOR')", context->name);
            }
            Result result = byte_result(first.byte ^ second.byte);
            return result;
        }
        case OT_NOT:{
            Result first = exec_arg(module, context, op.op.op_not.first);
            if(first.is_null){
                interpreter_err_in("NULL ARGUMENT ERROR ('NOT')", context->name);
            }
            else if(!first.is_byte){
                interpreter_err_in("BYTE ARGUMENT EXPECTED ERROR ('NOT')", context->name);
            }
            Result result = byte_result(~first.byte);
            return result;
        }
        case OT_OUT:{
            Result first = exec_arg(module, context, op.op.op_out.first);
            if(first.is_null){
                interpreter_err_in("NULL ARGUMENT ERROR ('OUT')", context->name);
            }
            printf(BYTE_TO_BINARY_PATTERN" ", BYTE_TO_BINARY(first.byte));
            break;
        }
        case OT_OUTCHAR:{
            Result first = exec_arg(module, context, op.op.op_outchar.first);
            if(first.is_null){
                interpreter_err_in("NULL ARGUMENT ERROR ('OUTCHAR')", context->name);
            }
            printf("%c", first.byte);
            break;
        }
        case OT_OUTNUM:{
            Result first = exec_arg(module, context, op.op.op_outnum.first);
            if(first.is_null){
                interpreter_err_in("NULL ARGUMENT ERROR ('OUTNUM')", context->name);
            }
            printf("%d", first.byte);
            break;
        }
        case OT_RETURN:{
            Result first = exec_arg(module, context, op.op.op_return.first);
            if(first.is_null){
                interpreter_err_in("NULL ARGUMENT ERROR ('RETURN')", context->name);
            }
            context->return_result = first;
            context->is_returned = 1;
            break;
        }
        case OT_POW:{
            Result first = exec_arg(module, context, op.op.op_pow.first);
            if(first.is_null){
                interpreter_err_in("NULL ARGUMENT ERROR ('POW')", context->name);
            }
            else if(!first.is_byte){
                interpreter_err_in("EXPECTED AN BYTE ARGUMENT ERROR ('POW')", context->name);
            }

            Result result;
            if(first.byte == 0){
                result = byte_result(0);
            }
            else{
                result = byte_result(255);
            }
            context->return_result = result;
            context->is_returned = 1;
            return result;
            break;
        }
        case OT_IS_NULL:{
            Result first = exec_arg(module, context, op.op.op_is_null.first);
            Result result;
            if(first.is_null){
                result = byte_result(255);
            }
            else{
                result = byte_result(0);
            }
            context->return_result = result;
            context->is_returned = 1;
            return result;
            break;
        }
        case OT_EXEC:{
            Result first = exec_arg(module, context, op.op.op_exec.first);
            if(first.is_null){
                interpreter_err_in("NULL ARGUMENT ERROR ('EXEC')", context->name);
            }
            else if(!first.is_scope && !first.is_whole_scope){
                interpreter_err_in("EXECTED AN SCOPE REFERENCE, BUT WAS BYTE ('EXEC')", context->name);
            }

            if(first.is_scope){
                Result result = execute_scope(module, first.scope_name, first.is_copy);
                return result;
            }
            else if(first.is_whole_scope){
                Result result = execute_whole_scope(module, first.scope);
                return result;
            }
        };
    }
    return null_result();
}

Result exec_arg(Module * module, ActiveScope * context, Argument arg){
    if(arg.is_byte){
        Result result = byte_result(arg.byte);
        return result;
    }
    else if(arg.is_ref || arg.is_copy_ref){
        Result result = scope_result(arg.scope_name, arg.is_copy_ref);
        return result;
    }
    else if(arg.is_sequence){
        for(int i = 0; i<arg.sequence.op_count; i++){
            if(i == arg.sequence.op_count - 1){
                return exec_op(module, context, arg.sequence.ops[i]);
            }
            else {
                exec_op(module, context, arg.sequence.ops[i]);
            }
        }
    }
    else if(arg.is_null){
        return null_result();
    }
    return null_result();
}

ByteVector new_stack_mem(){
    ByteVector stack = {NULL, 0};
    return stack;
}

int find_index_of_scope_by_name(Module * module, char * name){
    for(int i = 0; i < module->scopes.len; i++){
        if(strcmp(name, module->scopes.arr[i].name) == 0){
            return i;
        }
    }
    printf("ERROR: referenced scope not in module '%s'\n",name);
    exit(EXIT_FAILURE);
}

Result execute_whole_scope(Module * context, Scope scope){
    ActiveScope active_scope = {scope.name, scope.stack, scope.statements, null_result(), 0};

    for(int i=0; i<active_scope.statements.statements_len; i++){
        GenericOp op = active_scope.statements.statements[i];
        exec_op(context, &active_scope, op);
        if(active_scope.is_returned){ break; }
    }

    if(active_scope.is_returned){
        return active_scope.return_result;
    }

    free(active_scope.stack.arr);
    active_scope.stack.arr = NULL;
    return null_result();
}

Result execute_scope(Module * context, char * scope_name, int is_copy){
    int scope_index = find_index_of_scope_by_name(context, scope_name);
    Scope scope = context->scopes.arr[scope_index];

    ActiveScope active_scope;

    if(is_copy){
        ActiveScope tmp = {scope.name, new_stack_mem(), scope.statements, null_result(), 0};
        active_scope = tmp;
    }
    else {
        ActiveScope tmp = {scope.name, scope.stack, scope.statements, null_result(), 0};
        active_scope = tmp;
    }

    for(int i=0; i<active_scope.statements.statements_len; i++){
        GenericOp op = active_scope.statements.statements[i];
        exec_op(context, &active_scope, op);
        if(active_scope.is_returned){ break; }
    }

    if(!is_copy){
        context->scopes.arr[scope_index] = (Scope) {active_scope.name, active_scope.stack, active_scope.statements};
    }
    else if(is_copy){
        free(active_scope.stack.arr);
    }

    if(active_scope.is_returned){
        return active_scope.return_result;
    }
    return null_result();
}


Result interpret_exec(Module * context, Op_EXEC exec){
    check_expect_scope(exec.first);
    return execute_scope(context, exec.first.scope_name, exec.first.is_copy_ref);
}

int interpret(Module * mod){
    if(mod->has_entrypoint){
        interpret_exec(mod, mod->entrypoint);
    }
    return EXIT_SUCCESS;
}
