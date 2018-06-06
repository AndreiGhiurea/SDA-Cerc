#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "ccvector.h"

STATUS VecCreate(CC_VECTOR **Vector)
{
	if (NULL == Vector)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	CC_PVECTOR ptr = (CC_PVECTOR)malloc(sizeof(CC_VECTOR));

	if (NULL == ptr)
	{
		return STATUS_UNSUCCESSFUL;
	}

	ptr->base_capacity = 10;
	ptr->capacity = 10;
	ptr->size = 0;
	ptr->values = (int*)malloc(sizeof(int) * ptr->capacity);

	if (NULL == ptr->values)
	{
		free(ptr);
		return STATUS_UNSUCCESSFUL;
	}

	*Vector = ptr;
	return STATUS_SUCCESS;
}

STATUS VecDestroy(CC_VECTOR **Vector)
{
	if (NULL == Vector || NULL == *Vector)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	free((*Vector)->values);
	free(*Vector);

	return STATUS_SUCCESS;
}

static STATUS VecRealloc(CC_VECTOR *Vector, int newSize)
{
	if (NULL == Vector)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	int *ptr = (int*)realloc(Vector->values, sizeof(int) * newSize);

	if (NULL == ptr)
	{
		return STATUS_UNSUCCESSFUL;
	}

	Vector->values = ptr;
	Vector->capacity = newSize;

	return STATUS_SUCCESS;
}

STATUS VecInsertTail(CC_VECTOR *Vector, int Value)
{
	if (NULL == Vector)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	if (Vector->size == Vector->capacity)
	{
		STATUS Status;
		Status = VecRealloc(Vector, Vector->capacity * 2);
		if (!SUCCESS(Status))
		{
			return STATUS_UNSUCCESSFUL;
		}
	}

	Vector->values[Vector->size] = Value;
	Vector->size += 1;

	return STATUS_SUCCESS;
}

STATUS VecInsertHead(CC_VECTOR *Vector, int Value)
{
	if (NULL == Vector)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	if (Vector->size == Vector->capacity)
	{
		STATUS Status;
		Status = VecRealloc(Vector, Vector->capacity * 2);
		if (!SUCCESS(Status))
		{
			return STATUS_UNSUCCESSFUL;
		}
	}

	int size = Vector->size;
	for (; size > 0; size--)
	{
		Vector->values[size] = Vector->values[size - 1];
	}

	Vector->values[0] = Value;
	Vector->size += 1;

	return STATUS_SUCCESS;
}

STATUS VecInsertAfterIndex(CC_VECTOR *Vector, int Index, int Value)
{
	if (NULL == Vector)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	if (Index < 0 || Index > Vector->size - 1)
	{
		return STATUS_INVALID_PARAMETER_2;
	}

	if (Vector->size == Vector->capacity)
	{
		STATUS Status;
		Status = VecRealloc(Vector, Vector->capacity * 2);
		if (!SUCCESS(Status))
		{
			return STATUS_UNSUCCESSFUL;
		}
	}

	int size = Vector->size;
	for (; size > Index + 1; size--)
	{
		Vector->values[size] = Vector->values[size - 1];
	}

	Vector->values[Index + 1] = Value;
	Vector->size += 1;

	return STATUS_SUCCESS;
}

STATUS VecRemoveByIndex(CC_VECTOR *Vector, int Index)
{
	if (NULL == Vector)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	if (Index < 0 || Index > Vector->size - 1)
	{
		return STATUS_INVALID_PARAMETER_2;
	}

	int size = Index;
	for (; size < Vector->size - 1; size++)
	{
		Vector->values[size] = Vector->values[size + 1];
	}

	Vector->size -= 1;

	if ((Vector->size <= Vector->capacity >> 2) && (Vector->capacity > Vector->base_capacity))
	{
		STATUS Status;
		Status = VecRealloc(Vector, Vector->capacity >> 1);
		if (!SUCCESS(Status))
		{
			return STATUS_UNSUCCESSFUL;
		}
	}

	return STATUS_SUCCESS;
}

STATUS VecGetValueByIndex(CC_VECTOR *Vector, int Index, int *Value)
{
	if (NULL == Vector)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	if (Index < 0 || Index > Vector->size - 1)
	{
		return STATUS_INVALID_PARAMETER_2;
	}

	if (NULL == Value)
	{
		return STATUS_INVALID_PARAMETER_3;
	}

	*Value = Vector->values[Index];

	return STATUS_SUCCESS;
}

int VecGetCount(CC_VECTOR *Vector)
{
	if (NULL == Vector)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	return Vector->size;
}

STATUS VecClear(CC_VECTOR *Vector)
{
	if (NULL == Vector)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	int i;
	for (i = 0; i < Vector->size; i++)
	{
		VecRemoveByIndex(Vector, 0);
	}

	return STATUS_SUCCESS;
}

static void Swap(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

static int Partition(int *data, int left, int right)
{
	int x = data[right];
	int i = (left - 1);

	for (int j = left; j <= right - 1; ++j)
	{
		if (data[j] <= x)
		{
			++i;
			Swap(&data[i], &data[j]);
		}
	}

	Swap(&data[i + 1], &data[right]);

	return (i + 1);
}

static STATUS QuickSortIterative(int *data, int size) {
	// Create an aux stack
	int startIndex = 0;
	int endIndex = size - 1;
	int top = -1;
	int* stack = (int*)malloc(sizeof(int) * size);
	if (NULL == stack)
	{
		return STATUS_UNSUCCESSFUL;
	}

	// push initial values of l and h to stack
	stack[++top] = startIndex;
	stack[++top] = endIndex;

	// Keep popping from stack while is not empty
	while (top >= 0)
	{
		// Pop h and l
		endIndex = stack[top--];
		startIndex = stack[top--];

		// Set pivot element at its correct position
		// in sorted array
		int p = Partition(data, startIndex, endIndex);

		// If there are elements on left side of pivot,
		// then push left side to stack
		if (p - 1 > startIndex)
		{
			stack[++top] = startIndex;
			stack[++top] = p - 1;
		}

		// If there are elements on right side of pivot,
		// then push right side to stack
		if (p + 1 < endIndex)
		{
			stack[++top] = p + 1;
			stack[++top] = endIndex;
		}
	}

	free(stack);

	return STATUS_SUCCESS;
}

STATUS VecSort(CC_VECTOR *Vector)
{
	if (NULL == Vector)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	/*
	STATUS Status;
	Status = QuickSortIterative(Vector->values, Vector->size);
	if (!SUCCESS(Status))
	{
	return STATUS_UNSUCCESSFUL;
	}

	return STATUS_SUCCESS;
	*/

	return QuickSortIterative(Vector->values, Vector->size);
}