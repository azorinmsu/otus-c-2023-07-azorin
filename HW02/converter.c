#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "converter.h"
#include <string.h>

typedef unsigned char uchar;

static const uchar LAST_ASCII_CODE = 0x7F;

void convertUTF8(uchar content[], const size_t size, wchar_t mapping[], FILE* outputFile) {
  for (size_t i = 0; i < size; ++i) {
    uchar symbol = content[i];

    
    if (symbol <= LAST_ASCII_CODE) {
      fwrite(&symbol, sizeof(uchar), 1, outputFile);
      continue;
    }

    int mappingindex = symbol - LAST_ASCII_CODE - 1;
    wchar_t unicode = mapping[mappingindex];
    
    uchar first = (uchar) (((unicode >> 6) & 0x1F) | 0xC0);
    uchar second = (char) (((unicode >> 0) & 0x3F) | 0x80);

    fwrite(&first, sizeof(uchar), 1, outputFile);
    fwrite(&second, sizeof(uchar), 1, outputFile);
  }
}

void convertEncodingFile(uchar content[], const size_t size, const char* encodingInputFile, FILE* outputFile) {
  if (strcmp(CP1251_NAME_ENCODING, encodingInputFile) == 0) {
    convertUTF8(content, size, CP1251_MAPPING_UNICODE, outputFile);
  } else if (strcmp(ISO_8859_5_NAME_ENCODING, encodingInputFile) == 0) {
    convertUTF8(content, size, ISO_8859_5_MAPPING_UNICODE, outputFile);
  } else if (strcmp(KOI8_R_NAME_ENCODING, encodingInputFile) == 0) {
    convertUTF8(content, size, KOI8_MAPPING_UNICODE, outputFile);
  } else {
    perror("unknown encoding");
    exit(1);
  }
}

int main(int argc, char *argv[]) { 
  if (argc != 4) {
    perror("Number of parameters must be equal 3");
    return EXIT_FAILURE;
  }

  const char* inputFileName = argv[1];
  FILE* inputFile = fopen(inputFileName, "r");
  if (!inputFile) {
    fprintf(stderr, "can't open file %s: %s", inputFileName, strerror(errno));
    exit(1);
  }

  const char* outputFileName = argv[3];
  FILE* outputFile = fopen(outputFileName, "w+");
  if (!outputFile) {
    fprintf(stderr, "can't open file %s: %s", outputFileName, strerror(errno));
  }
 
  const char* encodingInputFile = argv[2];

  struct stat buffer;
  int retrunCodeStatFn = stat(inputFileName, &buffer);
  if (retrunCodeStatFn == -1) {
    perror("Error call stat function");
    exit(1);
  }

  const size_t size = buffer.st_size;
  uchar content[size];
  
  const size_t returnSizeInputFile = fread(content, sizeof content[0], size, inputFile);
  if (returnSizeInputFile != size) {
    fprintf(stderr, "Error reading file: %s", strerror(errno));
    exit(1);
  }
  
  convertEncodingFile(content, size, encodingInputFile, outputFile);

  fclose(inputFile);
  fclose(outputFile);

  return EXIT_SUCCESS;
}
