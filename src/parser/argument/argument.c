#include "argument.h"

Argument new_sequence_argument(Sequence sequence){
    Argument arg = {'\0', 0, sequence, 1};
    return arg;
}

Argument new_byte_argument(Byte byte){
    Argument arg = {byte, 1, (Sequence) {}, 0};
    return arg;
}
