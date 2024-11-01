#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashMap.h"

// Creates the hashmap
HashMap* createHashMap(int size) {
  HashMap* hashMap = (HashMap*)malloc(sizeof(HashMap));

  hashMap->size = size;
  hashMap->hashTable = (KeyValue**)malloc(hashMap->size * sizeof(KeyValue*));

  for (unsigned int i = 0; i < hashMap->size; i++) {
    hashMap->hashTable[i] = NULL;
  }

  hashMap->load = 0;
  
  return hashMap;
}

// Takes in a string and return the hash Value
int hashFunction(const char* hashString, int hashMapSize) {
  int i = 0;
  int strSum = 0;

  while (hashString[i] != '\0') {
    strSum += hashString[i];
    i++;
  }

  return strSum % hashMapSize;
}

// Resize of the map
void resizeHashMap(HashMap** hashMap) {
  HashMap* newHashMap = createHashMap(2 * (*hashMap)->size);
  KeyValue* keyValue;

  for (unsigned int i = 0; i < (*hashMap)->size; i++) {
    keyValue = (*hashMap)->hashTable[i];
    while (keyValue != NULL) {
      addValue(&newHashMap, keyValue->value, keyValue->key);
      keyValue = keyValue->next;
    }
  }

  destroyHashMap(*hashMap);
  *hashMap = newHashMap;
}

// Adds a Value to the hash table
int addValue(HashMap** hashMap, void* value, const char* key) {
  if ((*hashMap)->load / ((*hashMap)->size * 1.0) >= LOAD_FACTOR) { resizeHashMap(hashMap); }

  KeyValue* newKeyValue = (KeyValue*)malloc(sizeof(KeyValue));
  int tempKey = hashFunction(key, (*hashMap)->size);
  newKeyValue->key = strdup(key);
  newKeyValue->value = value;
  newKeyValue->next = (*hashMap)->hashTable[tempKey];
  (*hashMap)->hashTable[tempKey] = newKeyValue;
  (*hashMap)->load += 1;

  return tempKey;
}

// Removes a value from the hash table
int removeValue(HashMap* hashMap, const char* key) {
  if (strcmp(key, "") == 0) {
    printf("Empty string given for key in removeValue\n");
    return -1; 
  }

  int tempKey = hashFunction(key, hashMap->size);
  KeyValue* searchValue = hashMap->hashTable[tempKey];
  KeyValue* previousValue = hashMap->hashTable[tempKey];

  while (searchValue != NULL) {
    if (strcmp(searchValue->key, key) != 0) {
      previousValue = searchValue;
      searchValue = searchValue->next;
    }else { break; }
  }

  // Node not found
  if (searchValue == NULL) {
    printf("Hash value %s not found at %d\n", key, tempKey);
    return -1;
  }

  if (strcmp(searchValue->key, previousValue->key) != 0) { previousValue->next = searchValue->next; }
  else { hashMap->hashTable[tempKey] = searchValue->next; }
  
  hashMap->load -= 1;
  free(searchValue->key);
  free(searchValue);

  return 0;
}

// Finds a Value in the hash table in O(1) time
KeyValue* findKeyValue(HashMap* hashMap, const char* id) {
  int hashValue = hashFunction(id, hashMap->size);
  KeyValue* keyValue = hashMap->hashTable[hashValue];
  while (keyValue != NULL) {
    if (strcmp(id, keyValue->key) != 0) { keyValue = keyValue->next; }
    else { break; }
  }

  if (keyValue != NULL) { return keyValue; }
  else { return NULL; }
}

// For debuging
void printHashMap(HashMap* hashMap) {
  printf("=========== HashMap ===================\n");
  for (unsigned int i = 0; i < hashMap->size; i++) {
    KeyValue* keyValue = hashMap->hashTable[i];
    while (keyValue != NULL) {
      printf("The value is %s for pos %d and size %d\n", keyValue->key, hashFunction(keyValue->key, hashMap->size), hashMap->size);
      keyValue = keyValue->next;
    }
  }
}

// Frees allocated memory 
void destroyHashMap(HashMap* hashMap) {
  KeyValue* keyValue;
  for (unsigned int i = 0; i < hashMap->size; i++) {
    keyValue = hashMap->hashTable[i];
    while(keyValue != NULL) {
      keyValue = keyValue->next;
      removeValue(hashMap, hashMap->hashTable[i]->key);
    }
  }
  free(hashMap->hashTable);
  free(hashMap);
}