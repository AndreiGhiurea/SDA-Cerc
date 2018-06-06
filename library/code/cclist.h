#pragma once

#include "status_types.h"

typedef struct _CC_LIST_ENTRY { 
	struct _CC_LIST_ENTRY *next;
	int value;
	int size;
} CC_LIST_ENTRY, *CC_PLIST_ENTRY;
  
int LstCreate(CC_LIST_ENTRY **List); 
int LstDestroy(CC_LIST_ENTRY **List); 
 
int LstInsertValue(CC_LIST_ENTRY *List, int Value); 
int LstRemoveNode(CC_LIST_ENTRY *List, CC_LIST_ENTRY *Node); 
int LstGetNodeValue(CC_LIST_ENTRY *List, CC_LIST_ENTRY *Node, int *Value); 
int LstGetNthNode(CC_LIST_ENTRY *List, int Index, CC_LIST_ENTRY **Node); 
int LstGetNodeCount(CC_LIST_ENTRY *List); 
int LstClear(CC_LIST_ENTRY *List); 
int LstSortByValues(CC_LIST_ENTRY *List); 
int LstMergeSortedLists(CC_LIST_ENTRY *Destination, CC_LIST_ENTRY *Source); 
