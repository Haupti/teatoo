#include "parser.h"
#include "instruction/instruction.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utils/checked_alloc.h"
#include "scope/scope.h"

void parse_err_at(char * msg, int pos){
    printf("ERROR WHILE PARSING: at token %d: ", pos);
    printf("%s\n", msg);
    exit(EXIT_FAILURE);
}

Token get_token(TokenVector vec, int index){
    if(index >= vec.len){
        printf("ERROR WHILE PARSING: missing a token at %d\n", index);
        exit(EXIT_FAILURE);
    }
    Token token = vec.arr[index];
    return token;
}

void err_expected_token(Token token_expected, int index){
    printf("ERROR WHILE PARSING: expected token %s at %d\n", TKN_STR(token_expected), index);
    exit(EXIT_FAILURE);
}

void err_unexpected_token(Token token_unexpected, int index){
    printf("ERROR WHILE PARSING: unexpected token %s at %d\n", TKN_STR(token_unexpected), index);
    exit(EXIT_FAILURE);
}

void err_at(char * msg, int index){
    printf("ERROR WHILE PARSING: at %d: ", index);
    printf("%s\n",msg);
    exit(EXIT_FAILURE);
}

typedef struct {
    Token * arr;
    int start;
    int end;
} TokenSlice;

typedef struct {
    GenericOp * arr;
    int len;
} OpVector;

Sequence parse_sequence(TokenSlice slice){
    // sequence either starts with ( then it contains multiple,
    // or it starts with a instruction, then it only has this one instruction
    exit(EXIT_FAILURE);
}

int expects_zero_args(TokenType type){
    return type == TAKE
            || type == PEEK;
}

int expects_one_arg(TokenType type){
    return type == NOT
            || type == RETURN
            || type == EXEC
            || type == PUT
            || type == OUT
            || type == OUTCHAR
            || type == OUTNUM;
}

int expects_two_args(TokenType type){
    return type == IF
            || type == AND
            || type == OR
            || type == XOR;
}

int find_byte_end(TokenSlice slice){
    int start = slice.start;
    if(slice.arr[start].type != BYTE_START){
        err_at("not at byte start", start);
    }
    int i;
    for(i = start+1; i <= start+8; i++){
        if(i > start+1 && slice.arr[i].type == BYTE_END){
            return i;
        }
        if(slice.arr[i].type != BIT_OFF || slice.arr[i].type != BIT_ON){
            err_unexpected_token(slice.arr[i], i);
        }
    }
    err_expected_token(new_token(BYTE_END),i);
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

typedef struct {
    Sequence first;
    Sequence second;
} ArgumentPair;

GenericOp parse_op(TokenSlice slice);

Byte parse_byte(TokenSlice slice){
    if(slice.arr[slice.start].type != BYTE_START){
        err_expected_token(new_token(BYTE_START), slice.start);
    }
    if(slice.arr[slice.start+2].type == BYTE_END){
        if(slice.arr[slice.start+1].type == BIT_OFF){
            return 0;
        }
        else if(slice.arr[slice.start+1].type == BIT_ON){
            return 127;
        }
        else {
            err_unexpected_token(slice.arr[slice.start + 1], slice.start+1);
        }
    }
    else if(slice.arr[slice.start+9].type == BYTE_END){
        char c = 0;
        for(int i = 0; i < 8; i++){
            if(slice.arr[i].type == BIT_ON){
                c = c | (1 << i);
            }
            else if(slice.arr[i].type != BIT_OFF){
                err_unexpected_token(slice.arr[i], slice.start+1+i);
            }
        }
        return c;
    }
    err_unexpected_token(slice.arr[slice.start + 9], slice.start + 9);
    return '\0';
}

typedef struct Argument {
    Byte byte;
    int is_byte;
    Sequence sequence;
    int is_sequence;
} Argument;

Argument new_sequence_argument(Sequence sequence){
    Argument arg = {'\0', 0, sequence, 1};
    return arg;
}

Argument new_byte_argument(Byte byte){
    Argument arg = {byte, 1, (Sequence) {}, 0};
    return arg;
}

Sequence parse_sequence_arg(TokenSlice slice){
    if(slice.arr[slice.start].type != GRP_OPEN){
        err_at("cannot parse sequence if not at sequence start", slice.start);
    }

}

Argument collect_one_sequence_arg(TokenSlice slice){
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
    }
}

ArgumentPair collect_two_sequence_args(TokenSlice slice){
    // TODO
    return (ArgumentPair){};
}

GenericOp parse_op(TokenSlice slice){
    TokenSlice after_token = {slice.arr, slice.start + 1, slice.end};
    switch(after_token.arr[after_token.start].type){
        case EQ:{
            ArgumentPair pair = collect_two_sequence_args(after_token);

            Op_EQ op_eq = {pair.first, pair.second};
            union Op op;
            op.op_eq = op_eq;
            GenericOp gen_op = {OT_EQ, op};
            return gen_op;
        }
        case NEQ:{
            ArgumentPair pair = collect_two_sequence_args(after_token);

            Op_NEQ op_neq = {pair.first, pair.second};
            union Op op;
            op.op_neq = op_neq;
            GenericOp gen_op = {OT_NEQ, op};
            return gen_op;
        }
        case AND:{
            ArgumentPair pair = collect_two_sequence_args(after_token);

            Op_AND op_and = {pair.first, pair.second};
            union Op op;
            op.op_and = op_and;
            GenericOp gen_op = {OT_AND, op};
            return gen_op;
        }
        case OR:{
            ArgumentPair pair = collect_two_sequence_args(after_token);

            Op_OR op_or = {pair.first, pair.second};
            union Op op;
            op.op_or = op_or;
            GenericOp gen_op = {OT_OR, op};
            return gen_op;
        }
        case XOR:{
            ArgumentPair pair = collect_two_sequence_args(after_token);

            Op_XOR op_xor = {pair.first, pair.second};
            union Op op;
            op.op_xor = op_xor;
            GenericOp gen_op = {OT_XOR, op};
            return gen_op;
        }
        case IF:{
            ArgumentPair pair = collect_two_sequence_args(after_token);

            Op_IF op_if = {pair.first, pair.second};
            union Op op;
            op.op_if = op_if;
            GenericOp gen_op = {OT_IF, op};
            return gen_op;
        }
        case NOT:{
            Op_NOT op_not = {collect_one_sequence_arg(after_token)};
            union Op op;
            op.op_not = op_not;
            GenericOp gen_op = {OT_NOT, op};
            return gen_op;
        }
        case RETURN:{
            Op_RETURN op_return = {collect_one_sequence_arg(after_token)};
            union Op op;
            op.op_return = op_return;
            GenericOp gen_op = {OT_RETURN, op};
            return gen_op;
        }
        case EXEC:{
            Op_EXEC op_exec = {collect_one_sequence_arg(after_token)};
            union Op op;
            op.op_exec = op_exec;
            GenericOp gen_op = {OT_EXEC, op};
            return gen_op;
        }
        case PUT:{
            Op_PUT op_put = {collect_one_sequence_arg(after_token)};
            union Op op;
            op.op_put = op_put;
            GenericOp gen_op = {OT_PUT, op};
            return gen_op;
        }
        case OUT:{
            Op_OUT op_out = {collect_one_sequence_arg(after_token)};
            union Op op;
            op.op_out = op_out;
            GenericOp gen_op = {OT_OUT, op};
            return gen_op;
        }
        case OUTCHAR:{
            Op_OUTCHAR op_outchar = {collect_one_sequence_arg(after_token)};
            union Op op;
            op.op_outchar = op_outchar;
            GenericOp gen_op = {OT_OUTCHAR, op};
            return gen_op;
        }
        case OUTNUM:{
            Op_OUTNUM op_outnum = {collect_one_sequence_arg(after_token)};
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

int find_statement_end(TokenSlice slice){
    int sequence_counter = 0;
    Token current;
    for(int i = slice.start; i <= slice.end; i++){
        current = slice.arr[i];
        if(sequence_counter == 0 && (current.type == TERM_NL || current.type == TERM_SEM || current.type == SCOPE_CLOSE)){
            return i;
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

Scope parse_scope(char * name, TokenSlice slice){
    GenericOp * ops = NULL;
    int ops_count = 0;

    int line_start = slice.start;
    TokenSlice rest;
    int found_slice_end = 0;
    while(!found_slice_end){
        rest = (TokenSlice) {slice.arr, line_start, slice.end};
        int statement_end = find_statement_end(rest);
        TokenSlice statement_slice = {slice.arr, line_start, statement_end};
        GenericOp op = parse_op(statement_slice);

        ops_count += 1;
        ops = checked_realloc(ops, sizeof(GenericOp) * (ops_count));
        ops[ops_count-1] = op;

        line_start = statement_end + 1;

        // stop loop
        if(slice.arr[line_start].type == SCOPE_CLOSE){
            found_slice_end = 1;
        }
    }

    Statements statements = {ops, ops_count};
    Scope scope = new_scope(name, statements);
    return scope;
}

Module parse_module(TokenVector vec){

    TokenSlice slice = {vec.arr, 0, vec.len -1};



    // scope read mode vars
    char * scope_name = NULL;
    Scope current_scope;
    GenericOp current_instruction;
    // --



    // at this level only definitons of scopes and exec can occur
    Token current;
    int i;
    for(i = slice.start; i <= slice.end; i++){
        current = vec.arr[i];
        if(current.type == IDENTIFIER){
            scope_name = current.name;

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
            // add to list of scopes
            continue;
        }
        else if(current.type == EXEC){
            // add entry point
        }
        else {
            err_at("on module level only entrypoint (EXEC) and scope definitions are allowed", i);
        }
    }
    // TODO
    return (Module) {};
}




















