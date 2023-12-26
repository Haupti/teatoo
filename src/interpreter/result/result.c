#include "result.h"
#include <stdlib.h>

Result null_result(){
    Result result = {0, 0, NULL, 0, 0, (Scope) {}, 0, 1};
    return result;
}

Result byte_result(Byte byte){
    Result result = {byte, 1, NULL, 0, 0, (Scope) {}, 0, 0};
    return result;
}

Result scope_result(char * name, int is_copy){
    Result result = {0, 0, name, is_copy, 1, (Scope) {}, 0, 0};
    return result;
}

Result whole_scope_result(Scope scope){
    Result result = {0, 0, NULL, 1, 0, scope, 1, 0};
    return result;
}
