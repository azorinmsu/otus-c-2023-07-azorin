#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>

typedef struct {
  char* buffer;
  size_t length;
} Buffer;

typedef struct {
  Buffer url;
  size_t bytes;
  Buffer referer;
} Log;

Log parsedRow(char* row, size_t length);

static const Log EMPTY_LOG = {.url = {.buffer = NULL, .length = 0}, .bytes = 0, .referer = {.buffer = NULL, .length = 0}};

static const char* COMBINED_FORMAT_REGEXP = "\\d+\\.\\d+\\.\\d+\\.\\d+ \\- \\- \[\\d+\\/\\w+\\/\\d\\d\\d\\d\\:\\d\\d\\:\\d\\d\\:\\d\\d \\+\\d\\d\\d\\d\\] \"\\w+ \\/.+HTTP\\/\\d.\\d\" \\d+ \\d+ \"-\" .+http.+";

#endif
