#include "../../src/parser/slice/token_slice.h"
#include "../../src/parser/scope/scope.h"

int find_byte_end(TokenSlice slice);
int find_scope_end(TokenSlice slice);
int find_matching_sequence_end(TokenSlice slice);
Scope parse_scope(char * name, TokenSlice slice);
int find_statement_end(TokenSlice slice);
Sequence parse_sequence_arg(TokenSlice slice);
GenericOp parse_op(TokenSlice slice);
Argument collect_one_argument(TokenSlice slice);
