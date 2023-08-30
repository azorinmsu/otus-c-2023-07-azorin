#include "weather.h"
#include <stddef.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include "jsonParser.h"

static const char* WTTR_URL = "https://wttr.in/";
static const char* FORMAT_OUTPUT_REQUEST = "?format=j1";

struct Memory {
  char* response;
  size_t size;
};

size_t recievedData(void* data, size_t size, size_t nmemb, void* clientp) {
  size_t realSize = size * nmemb;
  struct Memory* mem = (struct Memory*) clientp;

  char* ptr = realloc(mem->response, mem->size + realSize + 1);
  if (ptr == NULL) {
    return 0;
  }

  mem->response = ptr;
  memcpy(&(mem->response[mem->size]), data, realSize);
  mem->size += realSize;
  mem->response[mem->size] = 0;

  return realSize;
}

void requestWeatherForLocation(char *location) {
  CURL* handle = curl_easy_init();
  if (!handle) {
    perror("Can't create curl struct. Programm was stopped");
    exit(1);
  }

  size_t sizeLocation = sizeof(&location)/sizeof(char);
  char urlGetRequest[sizeLocation + 10 + 16];
  sprintf(urlGetRequest, "%s%s%s", WTTR_URL, location, FORMAT_OUTPUT_REQUEST);

  curl_easy_setopt(handle, CURLOPT_URL, urlGetRequest);

  struct Memory chunk = {0};
  curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*) &chunk);

  curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, recievedData);

  curl_easy_perform(handle);

  parseResponse(chunk.response);

  curl_easy_cleanup(handle);
  free(chunk.response);

  exit(1);
}

