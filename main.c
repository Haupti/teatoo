#include <stdlib.h>
#include <stdio.h>
#include "lib/src/read_file.h"
#include "src/tt.h"

int main(int args, char * argv[]){
    if(args != 2){
        puts("ERROR: takes exaclty one argument");
        exit(EXIT_FAILURE);
    }
    FileContent content = read_file(argv[1]);
    if(content.buffer[content.size-1] != '\0'){
        puts("ERROR: while reading file: no \0 terminator");
        exit(EXIT_FAILURE);
    }
    int result = tt(content.buffer);
    free(content.buffer);
    content.buffer = NULL;
    return result;
}
