#ifndef READER
#define READER

#include <stdio.h>
#include <stddef.h>

typedef struct {
    int size;
    char** content;
} Files;

Files getAllFiles(char* dir);

char* readFile(char* fname);

#endif