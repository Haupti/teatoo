#include "../slice/token_slice.h"
#include "verifyer.h"
#include "../find/find.h"
#include "../error/error.h"

int is_terminated(TokenSlice slice, int next_token_after_arg_pos){
    if(slice.end <= next_token_after_arg_pos){
        return 1;
    }
    int next_token_type = slice.arr[next_token_after_arg_pos].type;
    if(next_token_type == TERM_NL || next_token_type == TERM_SEM || next_token_type == SCOPE_CLOSE){
        return 1;
    }
    return 0;
}


int has_only_one_argument(TokenSlice slice){
    if(slice.arr[slice.start].type == GRP_OPEN){
        int end_pos = find_matching_sequence_end(slice);
        return is_terminated(slice, end_pos + 1);
    }
    else if(slice.arr[slice.start].type == BYTE_START){
        int end_pos = find_byte_end(slice);
        return is_terminated(slice, end_pos + 1);
    }
    else if(slice.arr[slice.start].type == IDENTIFIER){
        return is_terminated(slice, slice.start + 1);
    }
    else if(slice.arr[slice.start].type == COPY && slice.arr[slice.start+1].type == IDENTIFIER){
        return is_terminated(slice, slice.start + 2);
    }
    return -1;
}

void verify_has_only_one_argument(TokenSlice slice){
    if (has_only_one_argument(slice) == 0){
        err_at("expected only one argument, but found no end of the argument", slice.start, slice.arr[slice.start].line_nr);
    }
    else if(has_only_one_argument(slice) == -1){
        err_at("while verifying argument count", slice.start, slice.arr[slice.start].line_nr);
    }
}

int has_only_two_arguments(TokenSlice slice){
    int arg1_end = find_argument_end(slice);
    if(arg1_end == -1){
        err_at("could not find end of the first argument (incorrect starting point)", slice.start, slice.arr[slice.start].line_nr);
    }

    TokenSlice first_slice = {slice.arr, slice.start, arg1_end};
    TokenSlice second_slice = {slice.arr, arg1_end + 1, slice.end};

    int first_arg_result =  has_only_one_argument(first_slice);
    int second_arg_result =  has_only_one_argument(second_slice);

    if(first_arg_result == -1 || second_arg_result == -1){
        return -1;
    }
    else {
        return first_arg_result == 1 && second_arg_result == 1;
    }
}

void verify_has_only_two_arguments(TokenSlice slice){
    int two_arg_result = has_only_two_arguments(slice);

    if(two_arg_result == -1){
        err_at("while verifying argument count", slice.start, slice.arr[slice.start].line_nr);
    }
    else if(two_arg_result == 0){
        err_at("expected two arguments, but the count does not match", slice.start, slice.arr[slice.start].line_nr);
    }
}
