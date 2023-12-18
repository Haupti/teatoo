#include "result.h"

Result null_result(){
    Result result = {0,0,1};
    return result;
}

Result byte_result(Byte byte){
    Result result = {byte, 1, 0};
    return result;
}
