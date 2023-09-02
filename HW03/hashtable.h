#ifndef HASH_TABLE_H 
#define HASH_TABLE_H

#include <stdbool.h>

bool contains(const char* key);

void addNewWithDefaultValue(const char* key);

void addAndIncrementExistsValue(const char* key);

void printHashTable();

#endif
