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


int find_byte_end(TokenSlice slice){
    int start = slice.start;
    if(slice.arr[start].type != BYTE_START){
        err_at("not at byte start", start);
    }
    Token current;
    for(int i = slice.start; i <= slice.end; i++){
        current = slice.arr[i];
        if(current.type == BYTE_END){
            return i;
        }
    }
    err_at("expected an end of the byte starting here", slice.start);
    return -1;
}

int find_scope_end(TokenSlice slice){
    TokenType start_type = slice.arr[slice.start].type;
    if(start_type == SCOPE_OPEN){
        int bracket_counter = 0;
        int i;
        for(i = slice.start; i <= slice.end; i++){
            switch(slice.arr[i].type){
                case SCOPE_CLOSE:
                    bracket_counter -= 1;
                    break;
                case SCOPE_OPEN:
                    bracket_counter += 1;
                    break;
                default: break;
            }
            if(bracket_counter == 0){
                return i;
            }
        }
        err_at("could not find matching sequence closing starting here", slice.start);
        return -1;
    }
    err_at("not at scope start, cannot search for end", slice.start);
    return -1;
}

int find_statement_end(TokenSlice slice){
    int sequence_counter = 0;
    Token current;
    for(int i = slice.start; i <= slice.end; i++){
        current = slice.arr[i];
        // statement can be ended by ')' but not if there was a '(' before within the same statement
        // e.g. (IF [0] (TAKE))
        // searching will start at IF but the ')' matching the '(' before the IF will also terminate the statement
        if(sequence_counter == 0 && (current.type == TERM_NL || current.type == TERM_SEM || current.type == SCOPE_CLOSE || current.type == GRP_CLOSE)){
            return i-1;
        }
        else if(current.type == GRP_OPEN){
            sequence_counter += 1;
        }
        else if(current.type == GRP_CLOSE){
            sequence_counter -= 1;
        }
    }
    parse_err_at("statement did not end before end of current slice", slice.start);
    return -1;
}


int find_matching_sequence_end(TokenSlice slice){
    TokenType start_type = slice.arr[slice.start].type;
    if(start_type == GRP_OPEN){
        int grp_counter = 0;
        int i;
        for(i = slice.start; i <= slice.end; i++){
            switch(slice.arr[i].type){
                case GRP_CLOSE:
                    grp_counter -= 1;
                    break;
                case GRP_OPEN:
                    grp_counter += 1;
                    break;
                default: break;
            }
            if(grp_counter == 0){
                return i;
            }
        }
        err_at("could not find matching sequence closing starting here", slice.start);
        return -1;
    }
    err_at("not at sequence start, cannot search for end", slice.start);
    return -1;
}

GenericOp parse_op(TokenSlice slice);

int find_statement_end(TokenSlice slice);

Statements parse_section_ended_by(TokenSlice slice, TokenType ending_token) {
    GenericOp * ops = NULL;
    int ops_count = 0;

    int line_start = slice.start + 1; // next to scope start
    TokenSlice rest;
    while(1){
        // stop loop if on end
        if(slice.arr[line_start].type == ending_token){
            break;
        }

        rest = (TokenSlice) {slice.arr, line_start, slice.end};

        int statement_end = find_statement_end(rest);
        TokenSlice statement_slice = {slice.arr, line_start, statement_end};
        GenericOp op = parse_op(statement_slice);

        ops_count += 1;
        ops = checked_realloc(ops, sizeof(GenericOp) * (ops_count));
        ops[ops_count-1] = op;

        line_start = statement_end + 1;

    }

    Statements statements = {ops, ops_count};
    return statements;
}

Scope parse_scope(char * name, TokenSlice slice){
    if(slice.arr[slice.start].type != SCOPE_OPEN){
        err_at("expected scope definition start, cannot parse scope from here", slice.start);
    }

    Statements statements = parse_section_ended_by(slice, SCOPE_CLOSE);

    Scope scope = new_scope(name, statements);
    return scope;
}

Sequence parse_sequence_arg(TokenSlice slice){
    if(slice.arr[slice.start].type != GRP_OPEN){
        err_at("cannot parse sequence if not at sequence start", slice.start);
    }

    Statements statements = parse_section_ended_by(slice, GRP_CLOSE);

    Sequence seq = {statements.statements, statements.statements_len};
    return seq;
}

Argument collect_one_argument(TokenSlice slice){
    if(slice.arr[slice.start].type == GRP_OPEN){
        int sequence_end = find_matching_sequence_end(slice);
        TokenSlice sequence_slice = {slice.arr, slice.start, sequence_end};
        Sequence sequence = parse_sequence_arg(sequence_slice);
        Argument arg = {'\0', 0, sequence, 1};
        return arg;
    }
    else if(slice.arr[slice.start].type == BYTE_START){
        Byte byte = parse_byte(slice);
        return new_byte_argument(byte);
    }
    else {
        err_at("expected '(' or a byte definition", slice.start);
        return (Argument) {};
    }
}

ArgumentPair collect_two_arguments(TokenSlice slice){
    int arg_start_pos = slice.start;
    Argument args[2];
    int i = 0;
    while(i < 2){
        if(slice.arr[arg_start_pos].type == GRP_OPEN){
            int sequence_end = find_matching_sequence_end(slice);
            TokenSlice sequence_slice = {slice.arr, arg_start_pos, sequence_end};
            Sequence sequence = parse_sequence_arg(sequence_slice);
            Argument arg = {'\0', 0, sequence, 1};
            args[i] = arg;
            arg_start_pos = sequence_end + 1;
        }
        else if(slice.arr[arg_start_pos].type == BYTE_START){
            Byte byte = parse_byte(slice);
            int byte_end = find_byte_end(slice);
            TokenSlice byte_slice = {slice.arr, arg_start_pos, byte_end};
            args[i] = new_byte_argument(byte);
            arg_start_pos = byte_end + 1;
        }
        else {
            err_at("expected '(' or a byte definition", slice.start);
        }
        i += 1;
    }
    ArgumentPair pair = {args[0], args[1]};
    return pair;
}

GenericOp parse_op(TokenSlice slice){
    TokenSlice after_token = {slice.arr, slice.start + 1, slice.end};
    switch(after_token.arr[after_token.start].type){
        case EQ:{
            ArgumentPair pair = collect_two_arguments(after_token);

            Op_EQ op_eq = {pair.first, pair.second};
            union Op op;
            op.op_eq = op_eq;
            GenericOp gen_op = {OT_EQ, op};
            return gen_op;
        }
        case NEQ:{
            ArgumentPair pair = collect_two_arguments(after_token);

            Op_NEQ op_neq = {pair.first, pair.second};
            union Op op;
            op.op_neq = op_neq;
            GenericOp gen_op = {OT_NEQ, op};
            return gen_op;
        }
        case AND:{
            ArgumentPair pair = collect_two_arguments(after_token);

            Op_AND op_and = {pair.first, pair.second};
            union Op op;
            op.op_and = op_and;
            GenericOp gen_op = {OT_AND, op};
            return gen_op;
        }
        case OR:{
            ArgumentPair pair = collect_two_arguments(after_token);

            Op_OR op_or = {pair.first, pair.second};
            union Op op;
            op.op_or = op_or;
            GenericOp gen_op = {OT_OR, op};
            return gen_op;
        }
        case XOR:{
            ArgumentPair pair = collect_two_arguments(after_token);

            Op_XOR op_xor = {pair.first, pair.second};
            union Op op;
            op.op_xor = op_xor;
            GenericOp gen_op = {OT_XOR, op};
            return gen_op;
        }
        case IF:{
            ArgumentPair pair = collect_two_arguments(after_token);

            Op_IF op_if = {pair.first, pair.second};
            union Op op;
            op.op_if = op_if;
            GenericOp gen_op = {OT_IF, op};
            return gen_op;
        }
        case NOT:{
            Op_NOT op_not = {collect_one_argument(after_token)};
            union Op op;
            op.op_not = op_not;
            GenericOp gen_op = {OT_NOT, op};
            return gen_op;
        }
        case RETURN:{
            Op_RETURN op_return = {collect_one_argument(after_token)};
            union Op op;
            op.op_return = op_return;
            GenericOp gen_op = {OT_RETURN, op};
            return gen_op;
        }
        case EXEC:{
            Op_EXEC op_exec = {collect_one_argument(after_token)};
            union Op op;
            op.op_exec = op_exec;
            GenericOp gen_op = {OT_EXEC, op};
            return gen_op;
        }
        case PUT:{
            Op_PUT op_put = {collect_one_argument(after_token)};
            union Op op;
            op.op_put = op_put;
            GenericOp gen_op = {OT_PUT, op};
            return gen_op;
        }
        case OUT:{
            Op_OUT op_out = {collect_one_argument(after_token)};
            union Op op;
            op.op_out = op_out;
            GenericOp gen_op = {OT_OUT, op};
            return gen_op;
        }
        case OUTCHAR:{
            Op_OUTCHAR op_outchar = {collect_one_argument(after_token)};
            union Op op;
            op.op_outchar = op_outchar;
            GenericOp gen_op = {OT_OUTCHAR, op};
            return gen_op;
        }
        case OUTNUM:{
            Op_OUTNUM op_outnum = {collect_one_argument(after_token)};
            union Op op;
            op.op_outnum = op_outnum;
            GenericOp gen_op = {OT_OUTNUM, op};
            return gen_op;
        }
        case TAKE:{
            Op_TAKE op_take = {};
            union Op op;
            op.op_take = op_take;
            GenericOp gen_op = {OT_TAKE, op};
            return gen_op;
        }
        case PEEK:{
            Op_PEEK op_peek = {};
            union Op op;
            op.op_peek = op_peek;
            GenericOp gen_op = {OT_PEEK, op};
            return gen_op;
        }
        default:{
            err_unexpected_token(after_token.arr[after_token.start], after_token.start);
            GenericOp op = {};
            return op;
        }
    }
}



Module parse_module(TokenVector vec){

    TokenSlice slice = {vec.arr, 0, vec.len -1};



    Scope * scopes;
    int scope_count;
    Op_EXEC entrypoint;

    // at this level only definitons of scopes and exec can occur
    Token current;
    int i;
    for(i = slice.start; i <= slice.end; i++){
        current = vec.arr[i];
        if(current.type == IDENTIFIER){
            if(get_token(vec, i+1).type != DEFINE){
                err_expected_token(new_token(DEFINE), i+1);
            }
            if(get_token(vec, i+2).type != SCOPE_OPEN){
                err_expected_token(new_token(SCOPE_OPEN), i+2);
            }
            TokenSlice slice = {vec.arr, i+2, vec.len-1};
            int scope_end = find_scope_end(slice);
            TokenSlice scope_slice = { vec.arr, i+2, scope_end};
            Scope scope = parse_scope(current.name, scope_slice);
            scope_count += 1;
            scopes = checked_realloc(scopes, sizeof(Scope) * scope_count);
            scopes[scope_count-1] = scope;
            continue;
        }
        else if(current.type == EXEC){
            TokenSlice exec_slice_part = {slice.arr, i, slice.end};
            int exec_end = find_statement_end(exec_slice_part);
            TokenSlice exec_slice = {slice.arr, i, exec_end};
            GenericOp op = parse_op(exec_slice);
            entrypoint = op.op.op_exec;
        }
        else {
            err_at("on module level only entrypoint (EXEC) and scope definitions are allowed", i);
        }
    }

    ScopeVector scope_vec = {scopes, scope_count};
    Module module = {scope_vec, entrypoint};
    return module;
}




















