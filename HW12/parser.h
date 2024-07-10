#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stddef.h>

typedef struct {
  char* url;
  size_t bytes;
  char* referer;
} Log;

Log parsedRow(char* row);

void freeLog(Log log);

static const Log EMPTY_LOG = {.url = NULL, .bytes = 0, .referer = NULL};

#endif
