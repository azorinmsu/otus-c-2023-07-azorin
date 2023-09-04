#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include "hashtable.h"
#include <string.h>

static void addWord(const char* word, const size_t size) {
  if (contains(word, size)) {
    addAndIncrementExistsValue(word, size);
  } else {
    addNewWithDefaultValue(word, size);
  }
}

static void addAllWordsHashtable(const char* content, const size_t size) {
  size_t tempCounter = 0;
  char* tempContent = malloc(sizeof(char) * size);
  for (size_t i = 0; i < size; ++i) {
    if (content[i] != '\n' && content[i] != ' ') {
      tempContent[tempCounter] = content[i];
      ++tempCounter;
    } else {
        if (i != 0 && (content[i - 1] != ' ' && content[i- 1] != '\n')) {
          tempContent[tempCounter] = ' ';
          ++tempCounter;
        }
    }
  }
  
  size_t index = 0;
  size_t lastDelimiterIndex = 0;
  while (index < tempCounter) {
    if (tempContent[index] == ' ') {
      size_t sizeWord = index - lastDelimiterIndex;
      char* word = (char*) malloc(sizeof(char) * sizeWord);
      memcpy(word, &tempContent[lastDelimiterIndex], sizeWord);
      addWord(word, sizeWord);
      lastDelimiterIndex = index + 1;
    }
    ++index;
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    perror("Number of argumets must be equals two");
    return EXIT_FAILURE;
  }

  const char* inputFileName = argv[1];
  FILE* inputFile = fopen(inputFileName, "r");
  if (!inputFile) {
    perror("Program can't read input file. File isn't found");
    return EXIT_FAILURE;
  }

  struct stat buffer;
  if (stat(inputFileName, &buffer) == -1) {
    perror("Failed determineted size of file");
    return EXIT_FAILURE;
  }

  const size_t size = buffer.st_size;
  char content[size];

  if (fread(content, sizeof content[0], size, inputFile) != size) {
    fprintf(stderr, "%s", strerror(errno));
    return EXIT_FAILURE;
  }
 
  addAllWordsHashtable(content, size);

  printHashtable();

  destroyHashtable();
  fclose(inputFile);

  return EXIT_SUCCESS;
}
