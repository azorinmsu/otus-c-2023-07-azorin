#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

static const unsigned char SIGNATURE_ZIP[] =  {0x50, 0x4b, 0x03, 0x04};
static const unsigned char SIGNATURE_EMPTY_ZIP[] = {0x50, 0x4b, 0x05, 0x06};

bool detectZipJpeg(char* content, const unsigned char* signature , size_t size) {
    size_t magic_number_size = 4;

    for (size_t i = 0; i < size - magic_number_size; ++i) {
        size_t j;
        for (j = 0; j < magic_number_size; ++j) {
            if (content[i + j] != signature[j]) {
                break;
            }
        }

        if (j == magic_number_size) {
            return true;
        }
    }

    return false;
}


int main(int argc, char** argv) {
    if (argc != 2) {
		perror("Error number of parameters");
		return EXIT_FAILURE;
	}

	const char* fname = argv[1];
	FILE* fp = fopen(fname, "r");
	if (fp == NULL) {
		fprintf(stderr, "can't open %s: %s\n", fname, strerror(errno));
		exit(1);
	}

	struct stat buffer;
	stat(fname, &buffer);
	const size_t size = buffer.st_size;
	char content[size];

	const size_t ret_code = fread(content, sizeof content[0], size, fp);
    if (ret_code != size) {
		perror("Error reading file");
		exit(1);		
	}


    if (detectZipJpeg(content, SIGNATURE_ZIP, size) || 
            detectZipJpeg(content, SIGNATURE_EMPTY_ZIP, size)) {
		printf("This is zipjpeg");
	}

	fclose(fp);                                  	
	return EXIT_FAILURE;
}
