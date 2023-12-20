#ifndef INTERPRETER_INTERNAL_HEADER
#define INTERPRETER_INTERNAL_HEADER

#include "../../src/interpreter/result/result.h"
#include "../../src/interpreter/active_scope.h"
#include "../../src/parser/parser.h"

Result exec_op(Module * module, ActiveScope * context, GenericOp op);
Result exec_arg(Module * module, ActiveScope * context, Argument arg);
Result execute_scope(Module * context, char * scope_name, int is_copy);

#endif
