#ifndef PARSER_VERIFIER_HEADER
#define PARSER_VERIFIER_HEADER

#include "../slice/token_slice.h"

void verify_has_only_one_argument(TokenSlice slice);
void verify_has_only_two_arguments(TokenSlice slice);

#endif
