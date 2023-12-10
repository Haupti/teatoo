#include <stdlib.h>
#include <stdio.h>

void * checked_malloc(int size){
    void * void_ptr = malloc(size);
    if(void_ptr == NULL){
        printf("ERROR: cannot allocate memory\n");
        exit(EXIT_FAILURE);
    }
    return void_ptr;
}

void * checked_realloc(void * source, int new_size){
    void * void_ptr = realloc(source, new_size);
    if(void_ptr == NULL){
        printf("ERROR: cannot realloc memory\n");
        exit(EXIT_FAILURE);
    }
    return void_ptr;
}
