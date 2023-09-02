#include "hashtable.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

struct entry {
  char* key;
  int value;
};

static const size_t DEFAULT_SIZE = 10;
static const int HASH_CONST_NUMBER = 53;
static const struct entry EMPTY_ENTRY;

static struct entry* hashtable;
static size_t capacity = 0;


static long long hashcode(const char* key, const size_t size) {
  long long hash = 0;
  long long p_pow = 1;

  for (size_t i = 0; i < size; ++i) {
    hash += (key[i] - 'a'  + 1) * p_pow;
    p_pow *= HASH_CONST_NUMBER;
  }

  return hash;
}

static void add(struct entry* entries, size_t size, const char* key, int value);

static void expand(const size_t size, const size_t sizeKey) {
  if (hashtable == NULL) {
    hashtable = (struct entry*) malloc(sizeof(struct entry) * size);
    capacity = size;
    return;
  }

  struct entry* nHashtable = (struct entry*) malloc(sizeof(struct entry) * size);
  if (!nHashtable) {
    printf("Failed initialize array.");
    exit(1);
  }

  for (size_t i = 0; i < capacity; ++i) {
    struct entry backet = hashtable[i];
    if (backet.key == NULL) {
      continue;
    }

    add(nHashtable, sizeKey, backet.key, backet.value);
  }

  free(hashtable);
  hashtable = nHashtable;
  capacity = size;
  free(nHashtable);
}

static void add(struct entry* entries, size_t size, const char* key, int value) {
  long long hash = hashcode(key, size);

  if (hashtable == NULL) {
    expand(DEFAULT_SIZE, size);
  }

  size_t index = (size_t) (hash & (long long) (capacity - 1));

  if (index >= capacity / 2 ) {
    expand(capacity + DEFAULT_SIZE * 2, size);
    index = (size_t) (hash & (long long) (capacity - 1));
  }

  while (entries[index].key != NULL) {
    if (strcmp(entries[index].key, key) == 0) {
      entries->value = value;
      return;
    }

    index++;
    if (index >= capacity) {
      index = 0;
    }
  }

  struct entry backet;
  char* key_copy = (char*) malloc(sizeof(char) * size);
  memcpy(key_copy, key, size);
  backet.value = value;
  backet.key = key_copy;
  entries[index] = backet;
}

static struct entry find(const char* key, const size_t size) {
  long long hash = hashcode(key, size);
  size_t index = (size_t) (hash & (long long) (capacity - 1));

  while (hashtable[index].key != NULL) {
    if (strcmp(hashtable[index].key, key) == 0) {
      return hashtable[index];
    }

    index++;
    if (index >= capacity) {
      index=0;
    }
  }

  return EMPTY_ENTRY;
}

bool contains(const char* key, const size_t size) {
  struct entry backet = find(key, size);
  return backet.key != NULL;
}

void addNewWithDefaultValue(const char* key, const size_t size) {
  add(hashtable, size, key, 1);
}

void addAndIncrementExistsValue(const char* key, const size_t size) { 
  struct entry backet = find(key, size);
  if (backet.key == NULL) {
    return;
  }

  backet.value = backet.value + 1;

}

void printHashTable() {
  if (hashtable == NULL) {
    return;
  }

  for (size_t i = 0; i < capacity; ++i) {
    struct entry backet = hashtable[i];

    if (backet.key == NULL) {
      continue;
    }

    printf("Key=%s; Value=%d", backet.key, backet.value);
    printf("\n");
  }
}


