#include "list_pointers.cpp"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define Assert(expr) if ((expr) == 0) {                   \
                                                          \
   PRINT_RED(BOLD(#expr "   is not true\n"));             \
   printf("In %s:%d\n", __PRETTY_FUNCTION__, __LINE__);   \
   return 0;                                              \
} 

typedef unsigned long long Hash_t;
const int TABLE_SIZE = 18061;

struct HashTable {
  List** hash_array;
};


Hash_t Hash_First  (void* buffer, size_t buffer_size);
Hash_t Hash_Sum    (void* buffer, size_t buffer_size);
Hash_t Hash_Len    (void* buffer, size_t buffer_size);
Hash_t Hash_Rotate (void* buffer, size_t buffer_size);
Hash_t Hash_GNU    (void* buffer, size_t buffer_size);
Hash_t Hash_Rot13  (void* buffer, size_t buffer_size);
Hash_t Hash_FAQ6   (void* buffer, size_t buffer_size);
Hash_t Hash_Ly     (void* buffer, size_t buffer_size);
Hash_t Hash_Rs     (void* buffer, size_t buffer_size);


int  HashTableValid   (HashTable* htable);
int  HashTableInit    (HashTable* htable);
void HashTableDisinit (HashTable* htable);

int HashTablePush (HashTable* htable, char* value, Hash_t (*Hash)(void* buffer, size_t buffer_size));


int HashTableValid(HashTable* htable) {
  Assert(htable != NULL);

  for (int i = 0; i < TABLE_SIZE; ++i)
    if (ListValid(htable->hash_array[i]) == 0)
      return 0;
  
  return 1;
}

int HashTableInit(HashTable* htable) {
  assert(htable != NULL);
  
  htable->hash_array = (List**) calloc(TABLE_SIZE, sizeof(List*));
  if (htable->hash_array == NULL)
    return 0;

  //fill hash array
  for (int i = 0; i < TABLE_SIZE; ++i) {
    htable->hash_array[i] = (List*)calloc(1, sizeof(List));
    if (htable->hash_array[i] == NULL)
      return 0;

    ListInit(htable->hash_array[i]);
  }
  
  return 1;
}

void HashTableDisinit(HashTable* htable) {
  assert(HashTableValid(htable));

  for (int i = 0; i < TABLE_SIZE; ++i) {
    ListDisinit(htable->hash_array[i]);
    free(htable->hash_array[i]);
  }
  
  free(htable->hash_array);
}

//------------------------------------------------------------------------------------------
//
// Return 0, if value wasn't pushed
// Return 1, if Value was pushed
// Return 2, if value is already exist
//
//------------------------------------------------------------------------------------------
int HashTablePush(HashTable* htable, char* value, Hash_t (*Hash)(void* buffer, size_t buffer_size)) {
  assert(HashTableValid(htable));

  Hash_t hash = (*Hash)(value, strlen(value));
  List* list = htable->hash_array[hash % TABLE_SIZE];
  
  //creating new ListNode
  if (ListPush(list, value)) return 0;
  
  return 1;
}




//////////////////////////// HASH /////////////////////////////////////////////////////////////////////////////////////////
Hash_t Hash_First(void* buffer, size_t buffer_size) {
  assert(buffer != NULL);
  assert(buffer_size > 0);

  char* norm_buffer = (char*) buffer;

  return (unsigned char)norm_buffer[0];
}

Hash_t Hash_Len(void* buffer, size_t buffer_size) {
  assert(buffer != NULL);
  assert(buffer_size > 0);

  return buffer_size;
}

Hash_t Hash_Sum(void* buffer, size_t buffer_size) {
  assert(buffer != NULL);
  assert(buffer_size > 0);

  char* norm_buffer = (char*) buffer;
  Hash_t hash = 0;

  for (int i = 0; i < buffer_size; ++i)
    hash += (unsigned char)norm_buffer[i];

  return hash;
}

Hash_t Hash_Rotate(void* buffer, size_t buffer_size) {
  assert(buffer != NULL);
  assert(buffer_size > 0);

  char* norm_buffer = (char*) buffer;
  Hash_t hash = 0;
  
  
  for (int i = 0 ; i < buffer_size; ++i) {
    hash = (hash << 4) ^ (unsigned char)norm_buffer[i];
  }
  return hash;
}

Hash_t Hash_GNU(void* buffer, size_t buffer_size) {
  assert(buffer != NULL);
  assert(buffer_size > 0);

  char* norm_buffer = (char*) buffer;
  Hash_t hash = 0;

  for (int i = 0; i < buffer_size; ++i)
    hash = (hash << 5  + hash) + (unsigned char)norm_buffer[i];
  
  return hash;
}

Hash_t Hash_FAQ6(void* buffer, size_t buffer_size) {
  assert(buffer != NULL);
  assert(buffer_size > 0);

  char* norm_buffer = (char*) buffer;
  Hash_t hash = 0;

  for (int i = 0; i < buffer_size; ++i) {
    hash += (unsigned char)norm_buffer[i];
    hash += hash << 10;
    hash ^= hash >> 6;
  }
  hash += hash << 3;
  hash ^= hash >> 11;
  hash += hash << 15;

  return hash;
}

Hash_t Hash_Rot13(void* buffer, size_t buffer_size) {
  assert(buffer != NULL);
  assert(buffer_size > 0);

  char* norm_buffer = (char*) buffer;
  Hash_t hash = 0;

  for (int i = 0; i < buffer_size; ++i) {
    hash += (unsigned char)norm_buffer[i];
    hash -= (hash << 13) | (hash >> 19);
  }

  return hash;
}

Hash_t Hash_Ly(void* buffer, size_t buffer_size) {
  assert(buffer != NULL);
  assert(buffer_size > 0);

  char* norm_buffer = (char*) buffer;
  Hash_t hash = 0;

  for (int i = 0; i < buffer_size; ++i)
    hash = hash * 1664525 + (unsigned char)norm_buffer[i] + 1013904223;

  return hash;
}

Hash_t Hash_Rs(void* buffer, size_t buffer_size) {
  assert(buffer != NULL);
  assert(buffer_size > 0);

  char* norm_buffer = (char*) buffer;
  Hash_t hash = 0;
  
  static const unsigned int b = 378551;
  unsigned int a = 63689;
  for (int i = 0; i < buffer_size; ++i) {
    hash = hash * 63689 + (unsigned char)norm_buffer[i];
    a *= b;
  }

  return hash;
}
