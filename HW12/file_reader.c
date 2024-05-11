#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <dirent.h> 
#include <string.h>
#include "file_reader.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static int countFiles(char* dir) {
    struct dirent *de;
    
    DIR *dr = opendir(dir); 
  
    if (dr == NULL) { 
        printf("Could not open current directory" ); 
        return 0; 
    }
  
    int count = 0;

    while ((de = readdir(dr)) != NULL) {
        if (de->d_type != DT_REG) {
            continue;
        }
        
        count++;
    }
  
    closedir(dr);     
    return count; 
}

Files getAllFiles(char* dir) {
    struct dirent *de;
    
    DIR *dr = opendir(dir); 
  
    if (dr == NULL) { 
        printf("Could not open current directory" ); 
        exit(0);
    }
  
    const int size = countFiles(dir);

    char** result = (char**) malloc(sizeof(char*) * size);

    int count = 0;

    while ((de = readdir(dr)) != NULL) {      
        if (de->d_type != DT_REG) {
            continue;
        }

        char* name = (char*) malloc(sizeof(char) * sizeof(de->d_name));
        strcpy(name, de->d_name);

        result[count++] = name;
    }
  
    closedir(dr);

    Files files;
    files.size = size;
    files.content = result;

    return files; 
}

char* readFile(char* fname) {
    FILE* fp = fopen(fname, "r");
	if (fp == NULL) {
		fprintf(stderr, "can't open %s: %s\n", fname, strerror(errno));
		exit(1);
	}

    struct stat buffer;
	stat(fname, &buffer);
	const size_t size = buffer.st_size;
    
	char* content = (char*) malloc(sizeof(char) * size);

	const size_t ret_code = fread(content, sizeof content[0], size, fp);
    if (ret_code != size) {
		perror("Error reading file");
		exit(1);		
	}

    return content;
}
