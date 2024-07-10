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

  Files files = getAllFiles(argv[1]);

  for (int i = 0; i < files.size; ++i) {
    char* content = readFile(files.content[i]);

    if (strlen(content) == 0) {
      continue;
    }

      
    int count = 0;  
    char* beginLine = &content[0];
    char* endLine = NULL;

    char* endFile = &content[strlen(content) - 1];

    int lineCount = 0;
    do {      
      lineCount++;
      printf("%d\n", lineCount);
      endLine = strstr(beginLine, "\n");

      if (endLine == endFile) {
        break;
      }

      if (endLine == NULL) {
        count = &content[strlen(content) - 1] - beginLine;
      } else {
        count = endLine - beginLine;
      }
    
      char* result = (char*) malloc(sizeof(char) * (count + 1));

      strncpy(result, beginLine, count);
      result[count] = '\0';

      // printf("%s\n", result);

      Log log = parsedRow(result);

      freeLog(log);
      free(result);

      beginLine = endLine + 1;
    } while (endLine != NULL);

    free(content);
    free(files.content[i]);
  }

  free(files.content);

  return 0;
}


