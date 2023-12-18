#include "byte.h"
#include "../slice/token_slice.h"
#include "../error/error.h"
#include <stdio.h>

Byte parse_byte(TokenSlice slice){
    if(slice.arr[slice.start].type != BYTE_START){
        err_expected_token(new_token(BYTE_START), slice.start);
    }

    if(slice.arr[slice.start+2].type == BYTE_END){
        if(slice.arr[slice.start+1].type == BIT_OFF){
            return 0x0;
        }
        else if(slice.arr[slice.start+1].type == BIT_ON){
            return 0xFF;
        }
        else {
            err_unexpected_token(slice.arr[slice.start + 1], slice.start+1);
        }
    }
    else if(slice.arr[slice.start+9].type == BYTE_END){
        int offset = slice.start + 1; //start is byte start, next must be the first bit
        Byte c = 0;
        for(int i = 0; i < 8; i++){
            if(slice.arr[i+offset].type == BIT_ON){
                c = c | (1 << (7-i));
            }
            else if(slice.arr[i+offset].type != BIT_OFF){
                err_unexpected_token(slice.arr[i+offset], i+offset);
            }
        }
        return c;
    }
    err_unexpected_token(slice.arr[slice.start + 9], slice.start + 9);
    return 0x0;
}
