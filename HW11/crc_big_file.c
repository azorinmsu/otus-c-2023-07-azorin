#include <sys/mman.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <zlib.h>

int main(int argc, char** argv) {  
	if (argc != 2) {
    printf("Need specify file's name");
    exit(1);
  }

  const char* fname = argv[1];
  int fp = open(fname, O_RDONLY);
	if (fp < 0) {
		fprintf(stderr, "can't open %s: %s\n", fname, strerror(errno));
		exit(1);
	}

  struct stat sb;
  stat(fname, &sb);

  const char* memblock = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fp, 0);
  
  uLong crc = 0;

  for (long int i = 0; i < sb.st_size; i = i + 256) {
    int size = i + 256 >= sb.st_size ? sb.st_size - i : 256 ;
    char buffer[size];
    strncpy(buffer, memblock + i, size);
    crc = crc32(crc, (const void*) buffer, size); 
  }

  printf("%lX\n", crc);

  return 0;
}
