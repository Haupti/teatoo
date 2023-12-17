#include "../../src/parser/slice/token_slice.h"
#include "../../src/parser/scope/scope.h"

Scope parse_scope(char * name, TokenSlice slice);
Sequence parse_sequence_arg(TokenSlice slice);
GenericOp parse_op(TokenSlice slice);
Argument collect_one_argument(TokenSlice slice);
