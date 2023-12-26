#include "parser.h"
#include "byte/byte.h"
#include "instruction/instruction.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utils/checked_alloc.h"
#include "scope/scope.h"
#include "slice/token_slice.h"
#include "error/error.h"
#include "byte/parse_byte.h"
#include "argument/argument.h"
#include "find/find.h"
#include "verify/verifyer.h"


GenericOp parse_op(TokenSlice slice);

Statements parse_section_ended_by(TokenSlice slice, TokenType ending_token) {
    GenericOp * ops = NULL;
    int ops_count = 0;


    int line_start = slice.start + 1; // next to section start
    TokenSlice rest;
    while(1){

        rest = (TokenSlice) {slice.arr, line_start, slice.end};
        // if 'start' is a newline (or a line only containing that) we skip it
        if(rest.arr[rest.start].type == TERM_NL || rest.arr[rest.start].type == TERM_SEM){
            line_start += 1;
            continue;
        }
        if(rest.arr[rest.start].type == ending_token){
            break;
        }

        int statement_end = find_statement_end(rest); // returns pos of last token of statement, not the terminator
        TokenSlice statement_slice = {slice.arr, line_start , statement_end};

        GenericOp op = parse_op(statement_slice);

        ops_count += 1;
        ops = checked_realloc(ops, sizeof(GenericOp) * (ops_count));
        ops[ops_count-1] = op;

        // stop loop if next token is the terminator of this current section (e.g. ')' or '}')
        if(slice.arr[statement_end + 1].type == ending_token){
            break;
        }

        line_start = statement_end + 2; // +1 to be at the terminator, another +1 to be at the next statement start
    }

    Statements statements = {ops, ops_count};
    return statements;
}

Scope parse_scope(char * name, TokenSlice slice){
    if(slice.arr[slice.start].type != SCOPE_OPEN){
        err_at("expected scope definition start, cannot parse scope from here", slice.start, head_slice(slice).line_nr);
    }

    if(slice.arr[slice.start + 1].type == SCOPE_CLOSE){
        Scope scope = new_scope(name, new_statements(NULL, 0));
        return scope;
    }
    else {

    Statements statements = parse_section_ended_by(slice, SCOPE_CLOSE);

    Scope scope = new_scope(name, statements);
    return scope;
    }
}

Sequence parse_sequence_arg(TokenSlice slice){
    if(slice.arr[slice.start].type != GRP_OPEN){
        err_at("cannot parse sequence if not at sequence start", slice.start, head_slice(slice).line_nr);
    }
    if(slice.arr[slice.start + 1].type == GRP_CLOSE){
        err_at("unexpected token, byte or seqeuence cannot be empty", slice.start, head_slice(slice).line_nr);
    }

    Statements statements = parse_section_ended_by(slice, GRP_CLOSE);

    Sequence seq = {statements.statements, statements.statements_len};
    return seq;
}

Argument collect_one_argument(TokenSlice slice){
    verify_has_only_one_argument(slice);
    if(slice.arr[slice.start].type == GRP_OPEN){
        int sequence_end = find_matching_sequence_end(slice);
        TokenSlice sequence_slice = {slice.arr, slice.start, sequence_end};
        Sequence sequence = parse_sequence_arg(sequence_slice);
        Argument arg = new_sequence_argument(sequence);
        return arg;
    }
    else if(slice.arr[slice.start].type == BYTE_START){
        Byte byte = parse_byte(slice);
        return new_byte_argument(byte);
    }
    else if(slice.arr[slice.start].type == COPY){
        if(slice.end <= slice.start){
            err_at("expected an scope identifier here", slice.start, head_slice(slice).line_nr);
        }
        return new_scope_copy_ref_argument(slice.arr[slice.start+1].name);
    }
    else if(slice.arr[slice.start].type == IDENTIFIER){
        return new_scope_ref_argument(slice.arr[slice.start].name);
    }
    else if(slice.arr[slice.start].type == COPY){
        return new_scope_copy_ref_argument(slice.arr[slice.start+1].name);
    }
    else {
        err_at("an argument cannot start with this token. I expected '(' or a byte definition", slice.start, head_slice(slice).line_nr);
        return (Argument) {};
    }
}

ArgumentPair collect_two_arguments(TokenSlice slice){
    verify_has_only_two_arguments(slice);
    int arg_start_pos = slice.start;
    Argument args[2];

    int arg1_end = find_argument_end(slice);
    TokenSlice first_arg_slice = {slice.arr, slice.start, arg1_end};
    TokenSlice second_arg_slice = {slice.arr, arg1_end + 1, slice.end};

    args[0] = collect_one_argument(first_arg_slice);
    args[1] = collect_one_argument(second_arg_slice);
    ArgumentPair pair = {args[0], args[1]};
    return pair;
}

GenericOp parse_op(TokenSlice slice){
    TokenSlice arguments_slice = {slice.arr, slice.start+1, slice.end};
    switch(slice.arr[slice.start].type){
        case EQ:{
            ArgumentPair pair = collect_two_arguments(arguments_slice);
            return new_eq(pair.first, pair.second);
        }
        case NEQ:{
            ArgumentPair pair = collect_two_arguments(arguments_slice);
            return new_neq(pair.first, pair.second);
        }
        case AND:{
            ArgumentPair pair = collect_two_arguments(arguments_slice);
            return new_and(pair.first, pair.second);
        }
        case OR:{
            ArgumentPair pair = collect_two_arguments(arguments_slice);
            return new_or(pair.first, pair.second);
        }
        case XOR:{
            ArgumentPair pair = collect_two_arguments(arguments_slice);
            return new_xor(pair.first, pair.second);
        }
        case IF:{
            ArgumentPair pair = collect_two_arguments(arguments_slice);
            return new_if(pair.first, pair.second);
        }
        case STACK:{
            ArgumentPair args = collect_two_arguments(arguments_slice);
            return new_stack(args.first, args.second);
        }
        case MULTIPLY:{
            ArgumentPair args = collect_two_arguments(arguments_slice);
            return new_multiply(args.first, args.second);
        }
        case DIVIDE:{
            ArgumentPair args = collect_two_arguments(arguments_slice);
            return new_divide(args.first, args.second);
        }
        case MINUS:{
            ArgumentPair args = collect_two_arguments(arguments_slice);
            return new_minus(args.first, args.second);
        }
        case PLUS:{
            ArgumentPair args = collect_two_arguments(arguments_slice);
            return new_plus(args.first, args.second);
        }
        case MODULO:{
            ArgumentPair args = collect_two_arguments(arguments_slice);
            return new_modulo(args.first, args.second);
        }
        case NOT:{
            Argument arg = collect_one_argument(arguments_slice);
            return new_not(arg);
        }
        case RETURN:{
            Argument arg = collect_one_argument(arguments_slice);
            return new_return(arg);
        }
        case EXEC:{
            Argument arg = collect_one_argument(arguments_slice);
            return new_exec(arg);
        }
        case PUT:{
            Argument arg = collect_one_argument(arguments_slice);
            return new_put(arg);
        }
        case POW:{
            Argument arg = collect_one_argument(arguments_slice);
            return new_pow(arg);
        }
        case IS_NULL:{
            Argument arg = collect_one_argument(arguments_slice);
            return new_is_null(arg);
        }
        case OUT:{
            Argument arg = collect_one_argument(arguments_slice);
            return new_out(arg);
        }
        case OUTCHAR:{
            Argument arg = collect_one_argument(arguments_slice);
            return new_outchar(arg);
        }
        case OUTNUM:{
            Argument arg = collect_one_argument(arguments_slice);
            return new_outnum(arg);
        }
        case TAKE:{
            return new_take();
        }
        case PEEK:{
            return new_peek();
        }
        case IS_EMPTY:{
            return new_is_empty();
        }
        default:{
            err_unexpected_token(arguments_slice.arr[arguments_slice.start], arguments_slice.start);
            GenericOp op = {};
            return op;
        }
    }
}

Module create_parse_module(TokenVector vec){

    if(vec.len == 0){
        ScopeVector scope_vec = {NULL, 0};
        Module module = {scope_vec, (Op_EXEC) {}, 0};
        return module;
    }

    TokenSlice slice = {vec.arr, 0, vec.len -1};

    Scope * scopes = NULL;
    int scope_count = 0;
    Op_EXEC entrypoint;
    int has_entrypoint = 0;

    // at this level only definitons of scopes and exec can occur
    Token current;
    int i;
    for(i = slice.start; i <= slice.end; i++){
        current = vec.arr[i];
        if(current.type == IDENTIFIER){
            if(get_token(vec, i+1).type != DEFINE){
                err_expected_token(new_token(DEFINE, 0), i+1, current.line_nr);
            }
            if(get_token(vec, i+2).type != SCOPE_OPEN){
                err_expected_token(new_token(SCOPE_OPEN, 0), i+2, current.line_nr);
            }
            TokenSlice slice = {vec.arr, i+2, vec.len-1};
            int scope_end = find_scope_end(slice);
            TokenSlice scope_slice = { vec.arr, i+2, scope_end};
            Scope scope = parse_scope(current.name, scope_slice);
            scope_count += 1;
            scopes = checked_realloc(scopes, sizeof(Scope) * scope_count);
            scopes[scope_count-1] = scope;
            i = scope_end;
        }
        else if(current.type == EXEC){
            TokenSlice exec_slice_part = {slice.arr, i, slice.end};
            int exec_end = find_statement_end(exec_slice_part);
            TokenSlice exec_slice = {slice.arr, i, exec_end};
            GenericOp op = parse_op(exec_slice);
            entrypoint = op.op.op_exec;
            has_entrypoint = 1;
            i = exec_end;
        }
        else if(current.type == TERM_NL || current.type == TERM_SEM){
            continue;
        }
        else {
            err_at("on module level only entrypoint (EXEC) and scope definitions are allowed", i, current.line_nr);
        }
    }

    ScopeVector scope_vec = {scopes, scope_count};
    Module module = {scope_vec, entrypoint, has_entrypoint};
    return module;
}
