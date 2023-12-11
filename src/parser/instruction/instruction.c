#include "instruction.h"
#include <stdlib.h>

Statements new_statements(GenericOp * statements, size_t len){
    Statements s = {statements, len};
    return s;
}
