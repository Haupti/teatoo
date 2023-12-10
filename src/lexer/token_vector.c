#include <stdio.h>
#include "token_vector.h"

void print_tknvec(TokenVector vec){
    printf("VECTOR LENGTH: %d\n", vec.len);
    for(int i =0; i<vec.len; i++){
        printf("%s\n", TKN_STR(vec.arr[i]));
    }
}
