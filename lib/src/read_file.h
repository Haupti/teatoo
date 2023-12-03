#ifndef READ_FILE_HEADER
#define READ_FILE_HEADER

typedef struct FileContent {
    char * buffer;
    long size;
} FileContent;

FileContent read_file(char * filename);

#endif
