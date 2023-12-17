#ifndef INTERNAL_VERIFY_HEADER_FOR_TESTS
#define INTERNAL_VERIFY_HEADER_FOR_TESTS

#include "../../../src/parser/slice/token_slice.h"

int is_terminated(TokenSlice slice, int next_token_after_arg_pos);
int has_only_one_argument(TokenSlice slice);
int has_only_two_arguments(TokenSlice slice);

#endif
