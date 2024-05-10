#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include "file_reader.h"

int main(int argc, char** argv) {
  if (argc != 3) {
    perror("Error number of parameters");
		return EXIT_FAILURE;
  }

  Files files = getAllFiles(argv[1]);

  printf("%x", files.size);

  for (int i = 0; i < files.size; ++i) {
    printf("%s\n", files.content[i]);
    free(files.content[i]);
  }

  free(files.content);


  return 0;
}


