#include "result/result.h"
#include "../parser/scope/scope.h"
#include "active_scope.h"

ActiveScope new_active_scope(
    char * name,
    ByteVector stack,
    Statements statements,
    Result return_result,
    int is_returned
){
    ActiveScope active_scope = {
        name,
        stack,
        statements,
        return_result,
        is_returned
    };
    return active_scope;
}
