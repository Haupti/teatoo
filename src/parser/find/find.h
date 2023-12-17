#ifndef PARSER_FIND_HEADER
#define PARSER_FIND_HEADER

#include "../slice/token_slice.h"
int find_matching_sequence_end(TokenSlice slice);
int find_byte_end(TokenSlice slice);
int find_scope_end(TokenSlice slice);
int find_statement_end(TokenSlice slice);
int find_argument_end(TokenSlice slice);
#endif
