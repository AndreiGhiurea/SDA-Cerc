#pragma once

#include "ccheap.h"
#include <stdio.h>
#include <stdlib.h>

// HpCreateMaxHeap and HpCreateMinHeap should create a max heap or a min heap, 
// respectively. InitialElements is an optional parameter and, if it is not null, the constructed
// heap should initially contain all the elements in the provided vector.
STATUS HpCreateMaxHeap(CC_HEAP **MaxHeap, CC_VECTOR* InitialElements)
{
	if (NULL == MaxHeap)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	CC_PHEAP ptr = (CC_PHEAP)malloc(sizeof(CC_HEAP));

	if (NULL == ptr)
	{
		return STATUS_UNSUCCESSFUL;
	}

	if (!SUCCESS(VecCreate(&ptr->vector)))
	{
		return STATUS_UNSUCCESSFUL;
	}

	ptr->heapType = MAX_HEAP;

	int Value;
	while (VecGetCount(InitialElements) > 0)
	{
		VecGetValueByIndex(InitialElements, VecGetCount(InitialElements) - 1, &Value);
		VecRemoveByIndex(InitialElements, VecGetCount(InitialElements) - 1);
		HpInsert(ptr, Value);
	}

	*MaxHeap = ptr;

	return STATUS_SUCCESS;
}

STATUS HpCreateMinHeap(CC_HEAP **MinHeap, CC_VECTOR* InitialElements)
{
	if (NULL == MinHeap)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	CC_PHEAP ptr = (CC_PHEAP)malloc(sizeof(CC_HEAP));

	if (NULL == ptr)
	{
		return STATUS_UNSUCCESSFUL;
	}

	if (!SUCCESS(VecCreate(&ptr->vector)))
	{
		return STATUS_UNSUCCESSFUL;
	}

	ptr->heapType = MIN_HEAP;

	int Value;
	while (VecGetCount(InitialElements) > 0)
	{
		VecGetValueByIndex(InitialElements, VecGetCount(InitialElements) - 1, &Value);
		VecRemoveByIndex(InitialElements, VecGetCount(InitialElements) - 1);
		HpInsert(ptr, Value);
	}

	*MinHeap = ptr;

	return STATUS_SUCCESS;
}

STATUS HpDestroy(CC_HEAP **Heap)
{
	if (NULL == Heap || NULL == *Heap)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	if (!SUCCESS(VecDestroy(&(*Heap)->vector)))
	{
		return STATUS_UNSUCCESSFUL;
	}

	free(*Heap);

	return STATUS_SUCCESS;
}

static void Swap(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

static STATUS MaxHeapifyUp(CC_PHEAP Heap, int index)
{
	int Node;
	VecGetValueByIndex(Heap->vector, index, &Node);
	int Parent;
	VecGetValueByIndex(Heap->vector, PARENT(index), &Parent);

	if (index && Parent < Node)
	{
		Swap(&Heap->vector->values[index], &Heap->vector->values[PARENT(index)]);
		MaxHeapifyUp(Heap, PARENT(index));
	}

	return STATUS_SUCCESS;
}

static STATUS MinHeapifyUp(CC_PHEAP Heap, int index)
{
	int Node;
	VecGetValueByIndex(Heap->vector, index, &Node);
	int Parent;
	VecGetValueByIndex(Heap->vector, PARENT(index), &Parent);

	if (index && Parent > Node)
	{
		Swap(&Heap->vector->values[index], &Heap->vector->values[PARENT(index)]);
		MinHeapifyUp(Heap, PARENT(index));
	}

	return STATUS_SUCCESS;
}

static STATUS MaxHeapifyDown(CC_PHEAP Heap, int index)
{
	int leftIndex = LEFT(index);
	int rightIndex = RIGHT(index);
	int largestIndex = index;

	if (leftIndex < VecGetCount(Heap->vector) && Heap->vector->values[leftIndex] > Heap->vector->values[index])
	{
		largestIndex = leftIndex;
	}

	if (rightIndex < VecGetCount(Heap->vector) && Heap->vector->values[rightIndex] > Heap->vector->values[largestIndex])
	{
		largestIndex = rightIndex;
	}

	if (largestIndex != index)
	{
		Swap(&Heap->vector->values[index], &Heap->vector->values[largestIndex]);
		MaxHeapifyDown(Heap, largestIndex);
	}

	return STATUS_SUCCESS;
}

static STATUS MinHeapifyDown(CC_PHEAP Heap, int index)
{
	int leftIndex = LEFT(index);
	int rightIndex = RIGHT(index);
	int smallestIndex = index;

	if (leftIndex < VecGetCount(Heap->vector) && Heap->vector->values[leftIndex] < Heap->vector->values[index])
	{
		smallestIndex = leftIndex;
	}

	if (rightIndex < VecGetCount(Heap->vector) && Heap->vector->values[rightIndex] < Heap->vector->values[smallestIndex])
	{
		smallestIndex = rightIndex;
	}

	if (smallestIndex != index)
	{
		Swap(&Heap->vector->values[index], &Heap->vector->values[smallestIndex]);
		MinHeapifyDown(Heap, smallestIndex);
	}

	return STATUS_SUCCESS;
}

STATUS HpInsert(CC_HEAP *Heap, int Value)
{
	if (NULL == Heap)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	if (!SUCCESS(VecInsertTail(Heap->vector, Value)))
	{
		return STATUS_UNSUCCESSFUL;
	}

	int index = VecGetCount(Heap->vector) - 1;

	if (MAX_HEAP == Heap->heapType)
	{
		MaxHeapifyUp(Heap, index);
	}
	else
	{
		MinHeapifyUp(Heap, index);
	}

	return STATUS_SUCCESS;
}

// HpRemove should remove all elements with the value Value in the heap
STATUS HpRemove(CC_HEAP *Heap, int Value)
{
	if (NULL == Heap)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	int i = 0;
	int VecValue, ParentValue;
	while (i < VecGetCount(Heap->vector))
	{
		VecGetValueByIndex(Heap->vector, i, &VecValue);
		if (VecValue == Value)
		{
			VecGetValueByIndex(Heap->vector, VecGetCount(Heap->vector) - 1, &Heap->vector->values[i]);
			VecRemoveByIndex(Heap->vector, VecGetCount(Heap->vector) - 1);
			if (MAX_HEAP == Heap->heapType)
			{
				VecGetValueByIndex(Heap->vector, i, &VecValue);
				VecGetValueByIndex(Heap->vector, PARENT(i), &ParentValue);
				if (ParentValue < VecValue)
				{
					MaxHeapifyUp(Heap, i);
				}
				else
				{
					MaxHeapifyDown(Heap, i);
				}
			}
			else
			{
				VecGetValueByIndex(Heap->vector, i, &VecValue);
				VecGetValueByIndex(Heap->vector, PARENT(i), &ParentValue);
				if (ParentValue > VecValue)
				{
					MinHeapifyUp(Heap, i);
				}
				else
				{
					MinHeapifyDown(Heap, i);
				}
			}
		}
		else
		{
			i++;
		}
	}

	return STATUS_SUCCESS;
}

// HpGetExtreme should return the maximum/minumum value in the heap, depending on the 
// type of heap constructed
STATUS HpGetExtreme(CC_HEAP *Heap, int* ExtremeValue)
{
	if (NULL == Heap)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	if (NULL == ExtremeValue)
	{
		return STATUS_INVALID_PARAMETER_2;
	}

	int Value;
	if (!SUCCESS(VecGetValueByIndex(Heap->vector, 0, &Value)))
	{
		return STATUS_UNSUCCESSFUL;
	}

	*ExtremeValue = Value;

	return STATUS_SUCCESS;
}

// HpPopExtreme should return the maximum/minimum value in the heap, and remove all 
// instances of said value from the heap
STATUS HpPopExtreme(CC_HEAP *Heap, int* ExtremeValue)
{
	if (NULL == Heap)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	if (NULL == ExtremeValue)
	{
		return STATUS_INVALID_PARAMETER_2;
	}

	int Value;
	if (!SUCCESS(VecGetValueByIndex(Heap->vector, 0, &Value)))
	{
		return STATUS_UNSUCCESSFUL;
	}

	//if (!SUCCESS(HpRemove(Heap, Value)))
	//{
	//	return STATUS_UNSUCCESSFUL;
	//}
	
	remove:
	VecGetValueByIndex(Heap->vector, VecGetCount(Heap->vector) - 1, &Heap->vector->values[0]);
	VecRemoveByIndex(Heap->vector, VecGetCount(Heap->vector) - 1);

	if (MAX_HEAP == Heap->heapType)
	{
		MaxHeapifyDown(Heap, 0);
	}
	else
	{
		MinHeapifyDown(Heap, 0);
	}
	
	int Temp;
	VecGetValueByIndex(Heap->vector, 0, &Temp);
	if (Temp == Value)
	{
		goto remove;
	}

	*ExtremeValue = Value;

	return STATUS_SUCCESS;
}

int HpGetElementCount(CC_HEAP *Heap)
{
	if (NULL == Heap)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	return VecGetCount(Heap->vector);
}

// HpSortToVector should construct and return (in the SortedVector parameter) a vector 
// sorted in increasing order containing all the elements present in the heap
STATUS HpSortToVector(CC_HEAP *Heap, CC_VECTOR* SortedVector)
{
	if (NULL == Heap)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	if (NULL == SortedVector)
	{
		return STATUS_INVALID_PARAMETER_2;
	}

	int Value;
	if (MAX_HEAP == Heap->heapType)
	{
		while (HpGetElementCount(Heap) > 0)
		{
			HpPopExtreme(Heap, &Value);
			VecInsertHead(SortedVector, Value);
		}
	}
	else
	{
		while (HpGetElementCount(Heap) > 0)
		{
			HpPopExtreme(Heap, &Value);
			VecInsertTail(SortedVector, Value);
		}
	}

	return STATUS_SUCCESS;
}