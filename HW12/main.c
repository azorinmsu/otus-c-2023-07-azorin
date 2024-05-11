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

  char* c = "144.76.1.137 - - [19/Nov/2020:21:48:14 +0000] \"GET /sitemap.xml HTTP/2.0\" 200 691352 \"-\" \"-\"\0";

  parsedRow(c, strlen(c));

  // for (int i = 0; i < files.size; ++i) {
  //   //printf("%s\n", files.content[i]);
  //   free(files.content[i]);
  // }

  // free(files.content);

  return 0;
}


