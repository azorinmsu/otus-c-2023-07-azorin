#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include "hashtable.h"
#include <string.h>

struct words {
  char** content;
  size_t size;
};

struct words splitWordsBySpace(const char* content, const size_t size) {
  size_t count = 0;
  
  printf("TEST1");

  for (size_t i = 0; i < size; ++i) {
    if (content[i] == ' ') {
      ++count;
    }
  }
  
printf("TEST2");

  char** wordsContent = (char**) malloc(sizeof(char*) * (count + 1));
  if (!wordsContent) {
    printf("Not having enough memory");
    exit(1);
  }

  size_t indexWordsContent = 0;
  size_t index = 0;
  size_t lastDelimiterIndex = 0;
  while (index < size) {
    if (content[index] == ' ') {
      wordsContent[indexWordsContent] = (char*) malloc(sizeof(char) * index - lastDelimiterIndex);
      memcpy(wordsContent[indexWordsContent], &content[lastDelimiterIndex], index - lastDelimiterIndex);
      
      lastDelimiterIndex = index + 1;
      ++indexWordsContent;
    }
  
    printf("%lu, %lu, %lu", index, indexWordsContent, count + 1);

    if (index == size -1 && lastDelimiterIndex > 0 && indexWordsContent < count) {
      wordsContent[indexWordsContent] = (char*) malloc(sizeof(char) * index - lastDelimiterIndex);
      memcpy(wordsContent[indexWordsContent], &content[lastDelimiterIndex], index - lastDelimiterIndex);
    }
    ++index;

  }

  struct words words;
  words.content = wordsContent;
  words.size = count;

  return words;
}

int main(int argc, char *argv[]) {

  printf("TEST-1");
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
 
  printf("TEST0");

  struct words words = splitWordsBySpace(content, size);

  for (size_t i = 0; i < words.size; ++i) {
    const char* word = words.content[i];
    const size_t keySize = sizeof(&word) / sizeof(char);

    if (contains(word, keySize)) {
      addAndIncrementExistsValue(word, keySize);
    } else {
      addNewWithDefaultValue(word, keySize);
    }
  }

  printHashTable();

  fclose(inputFile);

  return EXIT_SUCCESS;
}
