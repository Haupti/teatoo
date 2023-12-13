#include "argument.h"

Argument new_sequence_argument(Sequence sequence){
    Argument arg = {'\0', 0, sequence, 1, NULL, 0, 0};
    return arg;
}

Argument new_byte_argument(Byte byte){
    Argument arg = {byte, 1, (Sequence) {}, 0, NULL, 0, 0};
    return arg;
}

Argument new_scope_ref_argument(char * scope_name){
    Argument arg = {'\0', 0, (Sequence) {}, 0, scope_name, 1, 0};
    return arg;
}

Argument new_scope_copy_ref_argument(char * scope_name){
    Argument arg = {'\0', 0, (Sequence) {}, 0, scope_name, 0, 1};
    return arg;
}
