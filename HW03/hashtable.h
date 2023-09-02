#ifndef HASH_TABLE_H 
#define HASH_TABLE_H

#include <stdbool.h>
#include <stddef.h>

bool contains(const char* key, const size_t size);

void addNewWithDefaultValue(const char* key, const size_t size);

void addAndIncrementExistsValue(const char* key, const size_t size);

void printHashTable();

#endif
