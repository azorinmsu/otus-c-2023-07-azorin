#include "parser.h"
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stddef.h>
#include <onigmo.h>

typedef struct {
  int bytes;
  Buffer referer;
} Part;

static int getIndexFrom(char* row, size_t length, char* pattern, int lengthPatter, int start) {
  OnigRegex regex_begin;
  OnigRegion *region_begin;

  int result = onig_new(&regex_begin, pattern, pattern + lengthPatter, 
            ONIG_OPTION_DEFAULT, ONIG_ENCODING_ASCII, ONIG_SYNTAX_DEFAULT, NULL);

  if (result != ONIG_NORMAL) {
    return -1;
  }

  OnigPosition position = onig_search(regex_begin, row, row + length, row + start, row + length, region_begin, ONIG_OPTION_NONE);

  onig_region_free(region_begin, 1);
  onig_free(regex_begin);

  return position;
}

static int getBeginUrl(char* row, size_t length) {
  OnigRegex regex;
  OnigRegion *region;

  int result = onig_new(&regex, BEGIN_URL_REGEXP, BEGIN_URL_REGEXP + strlen(BEGIN_URL_REGEXP), 
            ONIG_OPTION_DEFAULT, ONIG_ENCODING_ASCII, ONIG_SYNTAX_DEFAULT, NULL);

  if (result != ONIG_NORMAL) {
    return -1;
  }

  region = onig_region_new();

  OnigPosition start = onig_search(regex, row, row + length, row, row + length, region, ONIG_OPTION_NONE);

  onig_region_free(region, 1);
  onig_free(regex);

  return getIndexFrom(row, length, "/\0", 1, start);
}

static int getEndUrl(char* row, size_t length) {
  OnigRegex regex;
  OnigRegion *region;

  int result = onig_new(&regex, END_URL_REGEXP, END_URL_REGEXP + strlen(END_URL_REGEXP), 
            ONIG_OPTION_DEFAULT, ONIG_ENCODING_ASCII, ONIG_SYNTAX_DEFAULT, NULL);

  if (result != ONIG_NORMAL) {
    return -1;
  }

  region = onig_region_new();

  OnigPosition position = onig_search(regex, row, row + length, row, row + length, region, ONIG_OPTION_NONE);

  onig_region_free(region, 1);
  onig_free(regex);

  return position;
}
static Part getNextPart(char* row, size_t length) {
  OnigRegex regex;
  OnigRegion *region;

  int result = onig_new(&regex, BEGIN_BYTES_REGEXP, BEGIN_BYTES_REGEXP + strlen(BEGIN_BYTES_REGEXP), 
            ONIG_OPTION_DEFAULT, ONIG_ENCODING_ASCII, ONIG_SYNTAX_DEFAULT, NULL);

  if (result != ONIG_NORMAL) {
    exit(EXIT_FAILURE);
  }

  region = onig_region_new();

  OnigPosition start = onig_search(regex, row, row + length, row, row + length, region, ONIG_OPTION_NONE);

  onig_region_free(region, 1);
  onig_free(regex);

  int beforeHttpStatus = getIndexFrom(row, length, " ", 1, start) + 1;
  int beginIndexBytes = getIndexFrom(row, length, " ", 1, beforeHttpStatus) + 1;
  int endIndexBytes = getIndexFrom(row, length, "\"", 1, beginIndexBytes);

  char* bytes = malloc(sizeof(char) * (endIndexBytes - beginIndexBytes));
  memcpy(bytes, &row[beginIndexBytes], (endIndexBytes - beginIndexBytes));

  int b;
  sscanf(bytes, "%d", &b);

  Part part;
  part.bytes = b;

  int endRefererUrl = getIndexFrom(row, length, "\"", 1, endIndexBytes + 1);
  char* referer = malloc(sizeof(char) * (endRefererUrl - endIndexBytes - 1));
  memcpy(referer, &row[endIndexBytes + 1], (endRefererUrl - endIndexBytes - 1));

  Buffer refererBuf = {.buffer = referer, .length = strlen(referer)};
  part.referer = refererBuf;

  printf(" bytes = %d referer= %s\n", part.bytes, part.referer.buffer);

  return part;
}

static Buffer getUrl(char* row, size_t length) {
  int begin = getBeginUrl(row, length);
  int end = getEndUrl(row, length);

  char* url = malloc(sizeof(char) * (end - begin));
  memcpy(url, &row[begin], end - begin);

  printf("%s\n", url);

  Buffer result = {.buffer = url, .length = strlen(url)};

  return result;
}



Log parsedRow(char* row, size_t length) {
  Log log;
  log.url = getUrl(row, length);
  
  Part part = getNextPart(row, length);

  log.bytes = part.bytes;
  log.referer = part.referer;

  return log;
}

