#include "cchashtable.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


STATUS HtCreate(CC_HASH_TABLE** HashTable)
{
	if (NULL == HashTable)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	CC_PHASH_TABLE ptr = (CC_PHASH_TABLE)malloc(sizeof(CC_HASH_TABLE));

	if (NULL == ptr)
	{
		return STATUS_UNSUCCESSFUL;
	}

	ptr->capacity = HASH_TABLE_SIZE;
	ptr->keyCount = 0;

	
	ptr->keysArray = (HASH_NODE**)malloc(sizeof(HASH_NODE*) * ptr->capacity);

	if (NULL == ptr->keysArray)
	{
		return STATUS_UNSUCCESSFUL;
	}
	
	unsigned long i = 0;
	for (i = 0; i < HASH_TABLE_SIZE; i++)
	{
		ptr->keysArray[i] = NULL;
	}
	

	*HashTable = ptr;
	return STATUS_SUCCESS;
}

STATUS HtDestroy(CC_HASH_TABLE** HashTable)
{
	if (NULL == HashTable)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	
	unsigned long i;
	for (i = 0; i < HASH_TABLE_SIZE; i++)
	{
		if ((*HashTable)->keysArray[i] != NULL)
		{
			free((*HashTable)->keysArray[i]);
		}
	}

	free((*HashTable)->keysArray);

	free(*HashTable);

	return STATUS_SUCCESS;
}

static int HtHash(char* Key)
{
	unsigned long hash = 0;
	int i = 0;

	while (hash < ULONG_MAX && i < strlen(Key)) 
	{
		hash = hash << 8;
		hash += Key[i];

		i++;
	}

	return hash % HASH_TABLE_SIZE;
}

STATUS HtSetKeyValue(CC_HASH_TABLE* HashTable, char* Key, int Value)
{
	if (NULL == HashTable)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	if (NULL == Key)
	{
		return STATUS_INVALID_PARAMETER_2;
	}

	
	PHASH_NODE node = (PHASH_NODE)malloc(sizeof(HASH_NODE));

	if (NULL == node)
	{
		return STATUS_UNSUCCESSFUL;
	}

	node->value = Value;
	strcpy(node->key, Key);	

	if (HashTable->keyCount >= HashTable->capacity)
	{
		return STATUS_UNSUCCESSFUL;
	}

	HashTable->keysArray[HtHash(Key)] = node;
	HashTable->keyCount++;

	return STATUS_SUCCESS;
}

STATUS HtGetKeyValue(CC_HASH_TABLE* HashTable, char* Key, int *Value)
{
	if (NULL == HashTable)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	if (NULL == Key)
	{
		return STATUS_INVALID_PARAMETER_2;
	}

	if (NULL == Value)
	{
		return STATUS_INVALID_PARAMETER_3;
	}

	PHASH_NODE node = NULL;
	node = HashTable->keysArray[HtHash(Key)];

	if (NULL == node)
	{
		return STATUS_UNSUCCESSFUL;
	}

	*Value = node->value;

	return STATUS_SUCCESS;
}

STATUS HtRemoveKey(CC_HASH_TABLE* HashTable, char* Key)
{
	if (NULL == HashTable)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	if (NULL == Key)
	{
		return STATUS_INVALID_PARAMETER_2;
	}

	if (HashTable->keysArray[HtHash(Key)] != NULL)
	{
		free(HashTable->keysArray[HtHash(Key)]);
		HashTable->keysArray[HtHash(Key)] = NULL;
		HashTable->keyCount--;
	}

	return STATUS_SUCCESS;
}

STATUS HtHasKey(CC_HASH_TABLE* HashTable, char* Key)
{
	if (HashTable->keysArray[HtHash(Key)] != NULL)
	{
		return STATUS_SUCCESS;
	}

	return STATUS_UNSUCCESSFUL;
}

STATUS HtGetNthKey(CC_HASH_TABLE* HashTable, int Index, char** Key)
{
	unsigned long i;
	unsigned long k = 0;
	PHASH_NODE node = NULL;
	for (i = 0; i < HASH_TABLE_SIZE; i++)
	{
		if (HashTable->keysArray[i] != NULL && k == Index)
		{
			node = HashTable->keysArray[i];
			break;
		}
		if (HashTable->keysArray[i] != NULL && k < Index)
		{
			k++;
		}
	}

	if (NULL == node)
	{
		return STATUS_UNSUCCESSFUL;
	}

	strcpy(*Key, node->key);
	
	return STATUS_SUCCESS;
}

STATUS HtClear(CC_HASH_TABLE* HashTable)
{
	if (NULL == HashTable)
	{
		STATUS_INVALID_PARAMETER_1;
	}

	unsigned long i;
	for (i = 0; i < HASH_TABLE_SIZE; i++)
	{
		if (HashTable->keysArray[i] != NULL)
		{
			free(HashTable->keysArray[i]);
			HashTable->keysArray[i] = NULL;
		}
	}

	HashTable->keyCount = 0;

	return STATUS_SUCCESS;
}

int HtGetKeyCount(CC_HASH_TABLE* HashTable)
{
	if (NULL == HashTable)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	return HashTable->keyCount;
}