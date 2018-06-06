#pragma once

#include "status_types.h"

#define HASH_TABLE_SIZE 1000001

typedef struct _HASH_NODE {
	char key[100];
	int value;
} HASH_NODE, *PHASH_NODE;

typedef struct _CC_HASH_TABLE { 
	unsigned long keyCount;
	unsigned long capacity;
	PHASH_NODE *keysArray;
} CC_HASH_TABLE, *CC_PHASH_TABLE;
  
int HtCreate(CC_HASH_TABLE** HashTable); 
int HtDestroy(CC_HASH_TABLE** HashTable); 
 
int HtSetKeyValue(CC_HASH_TABLE* HashTable, char* Key, int Value); 
int HtGetKeyValue(CC_HASH_TABLE* HashTable, char* Key, int *Value); 
int HtRemoveKey(CC_HASH_TABLE* HashTable, char* Key); 
int HtHasKey(CC_HASH_TABLE* HashTable, char* Key); 
int HtGetNthKey(CC_HASH_TABLE* HashTable, int Index, char** Key); 
int HtClear(CC_HASH_TABLE* HashTable); 
int HtGetKeyCount(CC_HASH_TABLE* HashTable);  
