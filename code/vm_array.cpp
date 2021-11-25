#include "vm_array.h"

template <typename T>
VMArray<T>::VMArray()
{
	max = ARRAY_START_SIZE;
	data = (T*)malloc(sizeof(T) * max);
	count = 0;
}

template <typename T>
VMArray<T>::VMArray(const VMArray<T>& old)
{
	count = old.count;
	max = old.max;
	data = (T*)malloc(sizeof(T) * max);
	memcpy(data, old.data, sizeof(T) * max);
}

template <typename T>
VMArray<T>::VMArray(std::initializer_list<T> init)
{
	max = ARRAY_START_SIZE;
	data = (T*)malloc(sizeof(T) * max);
	count = 0;

	for(T const &t : init) {
		Insert(t);
	}
}

template <typename T>
VMArray<T>::~VMArray()
{
	// if(data != NULL)
		// free(data);
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
int VMArray<T>::InsertNew()
{
	int result = -1;
	if(count == max - 1) {
		Grow();
	}
	result = count;
	count++;

	return result;
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

template <typename T>
void VMArray<T>::Free()
{
	free(data);
}

// very cheap clear.
// we simpy set the count to 0, which means there will most likely
// be leftover data in the array, but since we care more about performance than
// security this has been the priorty
template <typename T>
void VMArray<T>::Clear()
{
	count = 0;
}
