#ifndef INTERPRETER_INTERNAL_HEADER
#define INTERPRETER_INTERNAL_HEADER

#include "../../src/interpreter/result/result.h"
#include "../../src/interpreter/active_scope.h"

Result exec_op(ActiveScope * context, GenericOp op);
#endif
