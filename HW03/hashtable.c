#include "hashtable.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

struct entry {
  char* key;
  size_t keySize;
  int value;
};

static const size_t DEFAULT_SIZE = 10;
static const size_t HASH_CONST_NUMBER = 53;

static const struct entry EMPTY_ENTRY = {.key = NULL, .keySize = 0, .value = 0};


struct hashtable {
  struct entry* table;
  size_t capacity;
};

static struct hashtable hashtable;

static void add(struct hashtable* hashtable, size_t size, 
                const char* key, int value);

static size_t hashcode(const char* key, const size_t size) {
  size_t hash = 0;
  size_t p_pow = 1;

  for (size_t i = 0; i < size; ++i) {
    hash += (key[i] - 'a'  + 1) * p_pow;
    p_pow *= HASH_CONST_NUMBER;
  }

  return hash;
}

static void destroy(struct hashtable* hashtable) {
  for (size_t i = 0; i < hashtable->capacity; ++i) {
    if (hashtable->table[i].key == NULL) {
      continue;
    }

    free(hashtable->table[i].key);
  }

  free(hashtable->table);
}


static void expand(struct hashtable* hashtable) {
  if (hashtable->table == NULL) {
    hashtable->table = (struct entry*) malloc(sizeof(struct entry) * DEFAULT_SIZE);
    hashtable->capacity = DEFAULT_SIZE;

    for (size_t i = 0; i < hashtable->capacity; ++i) {
      hashtable->table[i] = EMPTY_ENTRY;
    }
    return;
  }

  size_t newSize = hashtable->capacity + DEFAULT_SIZE*2;
  struct hashtable nHashtable;
  nHashtable.table = (struct entry*) malloc(sizeof(struct entry) * newSize);
  nHashtable.capacity = newSize;

  for (size_t i = 0; i< nHashtable.capacity; ++i) {
    nHashtable.table[i] = EMPTY_ENTRY;
  }

  if (!nHashtable.table) {
    printf("Failed initialize array.");
    exit(1);
  }

  for (size_t i = 0; i < hashtable->capacity; ++i) {
    struct entry backet = hashtable->table[i];
    if (backet.key == NULL) {
      continue;
    }

    add(&nHashtable, backet.keySize, backet.key, backet.value);
  }

  destroy(hashtable);

  hashtable->table = nHashtable.table;
  hashtable->capacity = nHashtable.capacity;
}

static void add(struct hashtable* container, size_t size, 
                const char* key, const int value) {
  size_t hash = hashcode(key, size);

  if (container->table == NULL) {
    expand(container);
  }

  size_t index = (hash % container->capacity);

  if (index >= container->capacity / 2 ) {
    expand(container);
    index = (size_t) (hash % container->capacity);
  }

  while (container->table[index].key != NULL) {
    if (memcmp(container->table[index].key, key, size) == 0) {
      container->table[index].value = value;
      return;
    }

    index++;
    if (index >= container->capacity) {
      index = 0;
    }
  }

  struct entry backet;
  char* key_copy = (char*) malloc(sizeof(char) * size);
  memcpy(key_copy, key, size);

  backet.value = value;
  backet.key = key_copy;
  backet.keySize = size;
  container->table[index] = backet;
}

static struct entry find(const char* key, const size_t size) {
  if (hashtable.table == NULL) {
    expand(&hashtable);
    return EMPTY_ENTRY;
  }

  size_t hash = hashcode(key, size);
  size_t index = (hash % hashtable.capacity);

  while (hashtable.table[index].key != NULL) {
    if (memcmp(hashtable.table[index].key, key, size) == 0) {
      return hashtable.table[index];
    }

    index++;
    if (index >= hashtable.capacity) {
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
  add(&hashtable, size, key, 1);
}

void addAndIncrementExistsValue(const char* key, const size_t size) { 
  struct entry backet = find(key, size);
  if (backet.key == NULL) {
    return;
  }

  int value = backet.value + 1;
  add(&hashtable, size, key, value);
}

void printHashtable() {
  if (hashtable.table == NULL) {
    return;
  }

  for (size_t i = 0; i < hashtable.capacity; ++i) {
    struct entry backet = hashtable.table[i];

    if (backet.key == NULL) {
      continue;
    }

    printf("Key=%s; Value=%d", backet.key, backet.value);
    printf("\n");
  }
}


void destroyHashtable() {
  destroy(&hashtable);
}
