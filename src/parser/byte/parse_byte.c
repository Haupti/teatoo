#include "byte.h"
#include "../slice/token_slice.h"
#include "../error/error.h"
#include <stdio.h>
#include <stdlib.h>

void err(char * expected_msg, Token found_token, int pos, int line){
    printf("ERROR WHILE PARSING BYTE:");
    printf("%s", expected_msg);
    printf(" but found %s", TKN_STR(found_token));
    printf(" at %d in line %d\n", pos, line);
    exit(EXIT_FAILURE);
}

Byte parse_byte(TokenSlice slice){
    if(slice.arr[slice.start].type != BYTE_START){
        int line = slice.arr[slice.start].line_nr;
        err_expected_token(new_token(BYTE_START, 0), slice.start, line);
    }

    if(slice.arr[slice.start+2].type == BYTE_END){
        if(slice.arr[slice.start+1].type == BIT_OFF){
            return 0x0;
        }
        else if(slice.arr[slice.start+1].type == BIT_ON){
            return 0xFF;
        }
        else {
            err("expected a bit", slice.arr[slice.start+2], slice.start+1, slice.arr[slice.start+2].line_nr);
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
                err("expected a bit", slice.arr[i+offset], i+offset, slice.arr[i+offset].line_nr);
            }
        }
        return c;
    }
    err_unexpected_token(slice.arr[slice.start + 9], slice.start + 9);
    return 0x0;
}
