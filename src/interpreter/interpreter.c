#include "../parser/parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utils/checked_alloc.h"

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

typedef struct Result {
    Byte byte;
    int is_byte;
    int is_null;
} Result;

Result null_result(){
    Result result = {0,0,1};
    return result;
}

Result byte_result(Byte byte){
    Result result = {byte, 1, 0};
    return result;
}

Result exec_peek(Scope scope){
    if(scope.stack.len == 0){
        return null_result();
    }
    return byte_result(scope.stack.arr[scope.stack.len -1]);
}

Result exec_arg(Scope * context, Argument arg);

Result exec_and(Scope * scope, Op_AND op){
    Argument first_arg = op.first;
    Argument second_arg = op.second;
    Result first_result = exec_arg(scope, first_arg);
    Result second_result = exec_arg(scope, second_arg);
    if(first_result.is_byte && second_result.is_byte){
        char and_result = first_result.byte & second_result.byte;
        return byte_result(and_result);
    }
    else {
        interpreter_err_in("NULL ARGUMENT ERROR ('AND')", scope.name);
        return null_result();
    }
}

Result get_top_of_stack(ByteVector bytes){
    if(bytes.len == 0){
        return null_result();
    }
    else {
        return byte_result(bytes.arr[bytes.len -1]);
    }
}

Result exec_op(Scope * context, GenericOp op){
    switch(op.type){
        case PEEK:{
            return get_top_of_stack(context->stack);
            break;
        }
        case TAKE:{
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
        case PUT:{
            Result result = exec_arg(context, op.op.op_put.first);
            if(result.is_null){
                interpreter_err_in("NULL ARGUMENT ERROR ('PUT')", context->name);
            }
            else {
                context->stack.len = context->stack.len + 1;
                context->stack.arr = checked_realloc(context->stack.arr, sizeof(Byte) * (context->stack.len));
            }
        }
    }
}

Result exec_arg(Scope * context, Argument arg){
    if(arg.is_byte){
        Result result = byte_result(arg.byte);
        return result
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
}

int execute_scope(Module context, Scope scope, int is_copy){
    ByteVector stack = scope.stack;
    int stack_top = stack.len - 1;
    Statements statements = scope.statements;

    for(int i=0; i<scope.statements.statements_len; i++){
        GenericOp op = statements.statements[i];
        switch(op.type){
            case OT_PEEK:
                exec_peek(scope);
                break;
            case OT_AND:
                exec_and(&scope, op.op.op_and);
                break;
            case OT_EQ:
                break;

        }
    }
}


int interpret_exec(Module context, Op_EXEC exec){
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
    return EXIT_FAILURE;
}

int interpret(Module mod){
    if(mod.has_entrypoint){
        interpret_exec(mod.entrypoint);
    }
    return EXIT_SUCCESS;
}
