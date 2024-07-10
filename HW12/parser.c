#include "parser.h"
#include "string.h"
#include "stdlib.h"

static char* getReferer(char* row) {
  char* http = strstr(row, "HTTP");

  if (http == NULL) {
    char* result = (char*) malloc(sizeof(char));
    result[0] = '\0';
    return result;
  }

  char* firstSpace = strstr(http, " ");

  char* beginRefer = strstr(firstSpace + 1, "\"") + 1;

  char* endRefer = strstr(beginRefer, "\"");

  int count = endRefer - beginRefer;

  char* result = (char*) malloc(sizeof(char) * (count + 1));

  strncpy(result, beginRefer, count);
  result[count] = '\0';

  return result;
}

static size_t getBytes(char* row) {
  char* http = strstr(row, "HTTP");

  if (http == NULL) {
    return 0;
  }

  char* firstSpace = strstr(http, " ");

  char* beginBytes = strstr(firstSpace + 1, " ") + 1;

  char* endBytes = strstr(beginBytes, " ");

  int count = endBytes - beginBytes;

  char* result =  (char*) malloc(sizeof(char) * (count + 1));

  strncpy(result, beginBytes, count);
  result[count] = '\0';

  size_t bytes;

  sscanf(result, "%ld", &bytes);

  return bytes;
}

static char* getUrl(char* row) {
  char* qoutesOpen = strstr(row, "\"");

  char* qoutesEnd = strstr(qoutesOpen + 1, "\"");

  if (qoutesEnd - qoutesOpen == 1) {
    char* result = (char*) malloc(sizeof(char));
    result[0] = '\0';
    return result;
  }

  char* beginUrl = strstr(qoutesOpen, " ") + 1;

  char* endUrl = strstr(beginUrl, " ");

  int count = endUrl - beginUrl;

  char* result =  (char*) malloc(sizeof(char) * (count + 1));

  strncpy(result, beginUrl, count);
  result[count] = '\0';

  return result;
}


Log parsedRow(char* row) {
  Log log;
  log.url = getUrl(row);
  log.bytes = getBytes(row);
  log.referer = getReferer(row);

  return log;
}

void freeLog(Log log) {
  free(log.url);
  free(log.referer);
}