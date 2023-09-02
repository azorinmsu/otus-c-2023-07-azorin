#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include "hashtable.h"
#include <string.h>

struct word {
  char* content;
  size_t size;
};

struct words {
  struct word* values;
  size_t size;
};

struct words splitWordsBySpace(const char* content, const size_t size) {
  size_t count = 0;
  size_t lastSpace = 0;
  for (size_t i = 0; i < size; ++i) {
    if (content[i] == ' ') {
      ++count;
      lastSpace = i;
    }
  }
  
  if (size - lastSpace > 0) {
    ++count;
  }

  struct word* words = (struct word*) malloc(sizeof(struct word) * count);
  if (!words) {
    printf("Not having enough memory");
    exit(1);
  }
  
  size_t indexWordsContent = 0;
  size_t index = 0;
  size_t lastDelimiterIndex = 0;
  while (index < size) {
    if (content[index] == ' ') {
      words[indexWordsContent].content = (char*) malloc(sizeof(char) * (index - lastDelimiterIndex));
      words[indexWordsContent].size = index - lastDelimiterIndex;
      memcpy(words[indexWordsContent].content, &content[lastDelimiterIndex], index - lastDelimiterIndex);
      
      lastDelimiterIndex = index + 1;
      ++indexWordsContent;
    }
  
    if (index == size -1 && indexWordsContent == count - 1) {
      words[indexWordsContent].content = (char*) malloc(sizeof(char) * (index - lastDelimiterIndex));
      words[indexWordsContent].size = index - lastDelimiterIndex;
      memcpy(words[indexWordsContent].content, &content[lastDelimiterIndex], index - lastDelimiterIndex);
    }
    ++index;

  }

  struct words result;
  result.values = words;
  result.size = count;

  return result;
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
 
  struct words words = splitWordsBySpace(content, size);

  for (size_t i = 0; i < words.size; ++i) {
    const struct word word = words.values[i];

    printf("keySize %lu", word.size);

    contains(word.content, word.size);

   /* if (contains(word, keySize)) {
      printf("TEST1\n");
      //addAndIncrementExistsValue(word, keySize);
    } else {
      printf("TEST2\n");
      //addNewWithDefaultValue(word, keySize);
    }*/
  }

  //printHashTable();

  fclose(inputFile);

  return EXIT_SUCCESS;
}
