#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include "file_reader.h"
#include <stddef.h>

int main(int argc, char** argv) {
  if (argc != 3) {
    perror("Error number of parameters");
		return EXIT_FAILURE;
  }

  // Files files = getAllFiles(argv[1]);

  char* c = "66.249.68.12 - - [20/Sep/2020:22:11:20 +0000] \"GET /%D0%BC%D0%B0%D0%BB%D0%B5%D0%BD%D1%8C%D0%BA%D0%B8%D0%B9-%D0%BC%D0%B0%D0%BB%D1%8C%D1%87%D0%B8%D0%BA-%D0%BF%D0%BE-%D0%B8%D0%BC%D0%B5%D0%BD%D0%B8-%D0%9D%D1%83%D1%80%D0%B1%D0%B5%D0%BA-%D0%B6%D0%B8%D0%BB-%D0%B2-%D0%BD%D0%B5%D0%B1%D0%BE%D0%BB%D1%8C%D1%88%D0%BE%D0%B9/?p=2 HTTP/1.1\" 304 1234 \"http://localhost\" \"Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/85.0.4183.110 Mobile Safari/537.36 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)\" \"-\"\0";

  Log log = parsedRow(c, strlen(c));

  free(log.url.buffer);
  free(log.referer.buffer);

  // for (int i = 0; i < files.size; ++i) {
  //   //printf("%s\n", files.content[i]);
  //   free(files.content[i]);
  // }

  // free(files.content);

  return 0;
}


