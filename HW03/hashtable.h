#ifndef HASH_TABLE_H 
#define HASH_TABLE_H

#include <stdbool.h>
#include <stddef.h>

bool contains(const unsigned char* key, const size_t size);

void addNewWithDefaultValue(const unsigned char* key, const size_t size);

void addAndIncrementExistsValue(const unsigned char* key, const size_t size);

void destroyHashtable();

void printHashtable();

#endif
