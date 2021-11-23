#include "vm_array.h"

template <typename T>
VMArray<T>::VMArray()
{
	max = ARRAY_START_SIZE;
	data = (T*)malloc(sizeof(T) * max);
	Grow();
	count = 0;
}

template <typename T>
void VMArray<T>::Grow()
{
	max *= 2;
	data = (T*)realloc(data, sizeof(T) * max);
}

template <typename T>
int VMArray<T>::Insert(T e)
{
	int result = -1;

	if(count == max - 1) {
		Grow();
	}
	data[count] = e; 
	result = count;
	count++;

	return result;
}

template <typename T>
T* VMArray<T>::InsertNew()
{
	if(count == max - 1) {
		Grow();
	}
	T* e = &data[count];
	count++;

	return e;
}

template <typename T>
void VMArray<T>::Free()
{
	free(data);
}

template <typename T>
int VMArray<T>::Count()
{
	return count;
}

template <typename T>
T& VMArray<T>::operator[](int index)
{
	//TODO (rhoe) check bounds here
	return data[index];
}
