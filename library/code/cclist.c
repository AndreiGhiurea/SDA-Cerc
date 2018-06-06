#pragma once

#include <stdlib.h>
#include <stdio.h>
#include "cclist.h"

STATUS LstCreate(CC_LIST_ENTRY **List)
{
	if (NULL == List)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	CC_PLIST_ENTRY ptr = (CC_PLIST_ENTRY)malloc(sizeof(CC_LIST_ENTRY));

	if (NULL == ptr)
	{
		return STATUS_UNSUCCESSFUL;
	}

	ptr->next = NULL;
	ptr->size = 0;
	ptr->value = 0;

	*List = ptr;

	return STATUS_SUCCESS;
}

STATUS LstDestroy(CC_LIST_ENTRY **List)
{
	if (NULL == List || NULL == *List)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	CC_PLIST_ENTRY first = (*List)->next;
	CC_PLIST_ENTRY temp;

	while(NULL != first)
	{
		temp = first->next;
		free(first);
		first = temp;
	}

	free(*List);

	return STATUS_SUCCESS;
}

STATUS LstInsertValue(CC_LIST_ENTRY *List, int Value)
{
	if (NULL == List)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	CC_PLIST_ENTRY newNode = (CC_PLIST_ENTRY)malloc(sizeof(CC_LIST_ENTRY));

	if (NULL == newNode)
	{
		return STATUS_UNSUCCESSFUL;
	}

	newNode->next = List->next;
	newNode->value = Value;
	newNode->size = 0;

	List->next = newNode;
	List->size += 1;

	return STATUS_SUCCESS;
}


STATUS LstRemoveNode(CC_LIST_ENTRY *List, CC_LIST_ENTRY *Node)
{
	if (NULL == List)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	if (NULL == Node)
	{
		return STATUS_INVALID_PARAMETER_2;
	}

	CC_PLIST_ENTRY node = List->next;
	CC_PLIST_ENTRY last = List;

	while (NULL != node && node != Node)
	{
		last = node;
		node = node->next;
	}

	if (NULL == node)
	{
		return STATUS_UNSUCCESSFUL;
	}

	last->next = node->next;
	List->size -= 1;

	free(node);

	return STATUS_SUCCESS;
}

STATUS LstGetNodeValue(CC_LIST_ENTRY *List, CC_LIST_ENTRY *Node, int *Value)
{
	if (NULL == List)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	if (NULL == Node)
	{
		return STATUS_INVALID_PARAMETER_2;
	}

	if (NULL == Value)
	{
		return STATUS_INVALID_PARAMETER_3;
	}

	CC_PLIST_ENTRY node = List->next;

	while (NULL != node && node != Node)
	{
		node = node->next;
	}

	if (NULL == node)
	{
		return STATUS_UNSUCCESSFUL;
	}

	*Value = node->value;

	return STATUS_SUCCESS;
}

STATUS LstGetNthNode(CC_LIST_ENTRY *List, int Index, CC_LIST_ENTRY **Node)
{
	if (NULL == List)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	if (Index < 1 || Index > List->size)
	{
		return STATUS_INVALID_PARAMETER_2;
	}

	if (NULL == Node)
	{
		return STATUS_INVALID_PARAMETER_3;
	}

	int count = 1;
	CC_PLIST_ENTRY node = List->next;

	while (count != Index)
	{
		count += 1;
		node = node->next;
	}

	*Node = node;

	return STATUS_SUCCESS;
}

int LstGetNodeCount(CC_LIST_ENTRY *List)
{
	if (NULL == List)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	return List->size;
}

STATUS LstClear(CC_LIST_ENTRY *List)
{
	if (NULL == List)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	CC_PLIST_ENTRY first;

	while (NULL != List->next)
	{
		first = List->next;
		List->next = first->next;
		free(first);
	}

	List->size = 0;
	
	/*
	CC_PLIST_ENTRY node = List->next;

	while (NULL != node)
	{
		node->value = 0;
		node = node->next;
	}
	*/

	return STATUS_SUCCESS;
}

static CC_PLIST_ENTRY MergeSort(CC_LIST_ENTRY *head)
{
	if (NULL == head)
	{
		return NULL;
	}

	if (NULL == head->next)
	{
		return head;
	}

	int Count = 0;
	CC_PLIST_ENTRY temp = head;
	while (NULL != temp)
	{
		temp = temp->next;
		Count++;
	}

	int HalfCount = (Count / 2) - 1;
	CC_PLIST_ENTRY mid = head;
	while (HalfCount > 0)
	{
		mid = mid->next;
		HalfCount--;
	}

	CC_PLIST_ENTRY newHead = mid->next;
	mid->next = NULL;

	CC_PLIST_ENTRY h1 = MergeSort(head);
	CC_PLIST_ENTRY h2 = MergeSort(newHead);

	CC_LIST_ENTRY h1Header, h2Header;
	h1Header.next = h1;
	h2Header.next = h2;

	LstMergeSortedLists(&h1Header, &h2Header);

	return h1Header.next;
}

STATUS LstSortByValues(CC_LIST_ENTRY *List)
{
	if (NULL == List)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	List->next = MergeSort(List->next);

	return STATUS_SUCCESS;
}

STATUS LstMergeSortedLists(CC_LIST_ENTRY *Destination, CC_LIST_ENTRY *Source)
{
	if (NULL == Destination)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	if (NULL == Source)
	{
		return STATUS_INVALID_PARAMETER_2;
	}

	CC_PLIST_ENTRY destNode = Destination->next;
	CC_PLIST_ENTRY sourceNode = Source->next;

	if (NULL == sourceNode)
	{
		return STATUS_SUCCESS;
	}

	if (NULL == destNode)
	{
		Destination->next = Source->next;
		Destination->size = Source->size;
		Source->size = 0;
		Source->next = NULL;
		return STATUS_SUCCESS;
	}

	CC_PLIST_ENTRY sortedHead, sortedNode;

	if (destNode->value <= sourceNode->value)
	{
		sortedHead = destNode;
		destNode = destNode->next;
	}
	else
	{
		sortedHead = sourceNode;
		sourceNode = sourceNode->next;
	}

	sortedNode = sortedHead;

	while (NULL != destNode && NULL != sourceNode)
	{
		if (destNode->value <= sourceNode->value)
		{
			sortedNode->next = destNode;
			destNode = destNode->next;
			sortedNode = sortedNode->next;
		}
		else
		{
			sortedNode->next = sourceNode;
			sourceNode = sourceNode->next;
			sortedNode = sortedNode->next;
		}
	}

	while (NULL != destNode)
	{
		sortedNode->next = destNode;
		destNode = destNode->next;
		sortedNode = sortedNode->next;
	}

	while (NULL != sourceNode)
	{
		sortedNode->next = sourceNode;
		sourceNode = sourceNode->next;
		sortedNode = sortedNode->next;
	}

	Destination->next = sortedHead;
	Destination->size += Source->size;
	Source->size = 0;
	Source->next = NULL;

	return STATUS_SUCCESS;
}