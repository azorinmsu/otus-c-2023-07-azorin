#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include "hashtable.h"
#include <string.h>

static void addWord(const unsigned char* word, const size_t size) {
  if (contains(word, size)) {
    addAndIncrementExistsValue(word, size);
  } else {
    addNewWithDefaultValue(word, size);
  }
}

static void addAllWordsHashtable(const unsigned char* content, const size_t size) {
  size_t startWord = 0;

  for (size_t i = 0; i < size; ++i) {
    if (content[i] != '\n' && content[i] != ' ') {
      if (i > 0 && (content[i - 1] == ' ' || content[i-1] == '\n')) {
        startWord = i;
      }
    } else {
        if (i != 0 && (content[i - 1] != ' ' && content[i- 1] != '\n')) {
          size_t sizeWord = i - startWord;
          unsigned char word[sizeWord + 1];

          memcpy(word, &content[startWord], sizeWord);
          word[sizeWord] = '\0';
          addWord(word, sizeWord);
        }
    }
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
  unsigned char* content = (unsigned char*) malloc(sizeof(unsigned char) * size);

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
