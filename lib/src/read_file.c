#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_file.h"

FileContent read_file(char * filename){
    char * buffer = 0;
    long filesize;
    FILE * fptr = fopen (filename, "rb");

    if (fptr) {
      fseek (fptr, 0, SEEK_END);
      filesize = ftell (fptr);
      fseek (fptr, 0, SEEK_SET);
      buffer = malloc(filesize+1);
      if (buffer) {
        fread (buffer, 1, filesize, fptr);
      }
      else {
          printf("ERROR: cannot allocate space to read file\n");
          exit(EXIT_FAILURE);
      }
      fclose (fptr);
    }

    buffer[filesize] = '\0';

    FileContent file_content = { buffer, filesize+1 };
    return file_content;
}

int file_size(char * filename){
    FILE * fptr;
    fptr = fopen(filename, "r");
    fseek(fptr, 0, SEEK_END);
    int size = ftell(fptr);
    fclose(fptr);
    return size;
}
