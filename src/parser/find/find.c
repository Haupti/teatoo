#include "../slice/token_slice.h"
#include "../error/error.h"
#include "find.h"

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

int find_scope_ref_end(TokenSlice slice){
    int start_type = slice.arr[slice.start].type;
    if(start_type == COPY && slice.end >= slice.start + 1 && slice.arr[slice.start + 1].type == IDENTIFIER){
        return slice.start + 1;
    }
    if(start_type == IDENTIFIER){
        return slice.start;
    }
    parse_err_at("not at an identifier ref, therefor I cannot find the end", slice.start);
    return -1;
}

int find_argument_end(TokenSlice slice){
    int start_type = slice.arr[slice.start].type;
    switch(start_type){
        case GRP_OPEN:
            return find_matching_sequence_end(slice);
            break;
        case BYTE_START:
            return find_byte_end(slice);
            break;
        case COPY:
            return find_scope_ref_end(slice);
            break;
        case IDENTIFIER:
            return find_scope_ref_end(slice);
            break;
        default:
            return -1;
    }
}




















