#include "parser.h"
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stddef.h>
#include <onigmo.h>

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

  OnigRegex regex_begin;
  OnigRegion *region_begin;

  char* patternBeginUrl = "/\0";

  result = onig_new(&regex_begin, patternBeginUrl, patternBeginUrl + strlen(patternBeginUrl), 
            ONIG_OPTION_DEFAULT, ONIG_ENCODING_ASCII, ONIG_SYNTAX_DEFAULT, NULL);

  if (result != ONIG_NORMAL) {
    return -1;
  }



  OnigPosition position = onig_search(regex_begin, row, row + length, row + start, row + length, region_begin, ONIG_OPTION_NONE);

  onig_region_free(region_begin, 1);
  onig_free(regex_begin);

  return position;
}

static int getEndUrll(char* row, size_t length) {
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

Log parsedRow(char* row, size_t length) {
  int begin = getBeginUrl(row, length);
  int end = getEndUrll(row, length);

  char* url = malloc(sizeof(char) * (end - begin));
  memcpy(url, &row[begin], end - begin);

  printf("%s", url);


  Buffer bufferUrl = {.buffer = url, .length = strlen(url)};

  Log log;
  log.url = bufferUrl;


  return EMPTY_LOG;
}

