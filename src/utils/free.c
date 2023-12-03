#include <stdlib.h>

void free_s(void * ptr){
    free(ptr);
    ptr = NULL;
}
