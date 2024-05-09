#include "parser.h"
#include <regex.h>
#include <stdbool.h>

bool isCombinedLog(char* row) {
  regex_t regex;

  if (regcomp(&regex, COMBINED_FORMAT_REGEXP, REG_NEWLINE)) {
    fprintf(stderr, "Cannot compile regexp pattern");
    return false;
  }

  int result = regexec(&regex, row, 0, NULL, 0);

  if (!result) {
    return true;
  }

  return false;
}

Log parsedRow(char* row, size_t length) {
  if (!isCombinedLog(row)) {
    return EMPTY_LOG; 
  }

  return EMPTY_LOG;
}
