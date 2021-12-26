#include "vm_array.h"

template <typename T>
VMArray<T>::VMArray()
{
	max = ARRAY_START_SIZE;
	data = (T*)calloc(max, sizeof(T));
	count = 0;
}

template <typename T>
VMArray<T>::VMArray(const VMArray<T>& old)
{
	count = old.count;
	max = old.max;
	data = (T*)calloc(max, sizeof(T));
	memcpy(data, old.data, sizeof(T) * max);
}


// TODO (rhoe) more space efficient init list
// we could save memory by setting max to the size of the init list
// this way the array would possibly create a better contextual size
template <typename T>
VMArray<T>::VMArray(std::initializer_list<T> init)
{
	max = ARRAY_START_SIZE;
	data = (T*)calloc(max, sizeof(T));
	count = 0;

	for(const T &t : init) {
		Insert(t);
	}
}


template <typename T>
VMArray<T>::VMArray(int _max, int _count, T *_data)
{
	max = _max;
	count = _count;
	data = (T*)calloc(max, sizeof(T));
	memcpy(data, _data, count * sizeof(T));
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
	int oldMax = max;
	max *= 2;
	data = (T*)realloc(data, sizeof(T) * max);
	T* offset = data + oldMax;
	memset(offset, 0, max - oldMax);
}

template <typename T>
int VMArray<T>::Insert(const T &e)
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
int VMArray<T>::Max()
{
	return max;
}

// TODO (rhoe) this function is not testet
template <typename T>
bool VMArray<T>::Contains(const T &e)
{
	bool result = false;
	for(int i = 0; i < count; i++) {
		if(data[i] == e) {
			result = true;
			break;
		}
	}

	return result;
}

template <typename T>
T* VMArray<T>::Data()
{
	return data;
}

template <typename T>
T& VMArray<T>::operator[](int index)
{
	//TODO (rhoe) check bounds here
	return data[index];
}

// template <typename T>
// VMArray<T>& VMArray<T>::operator=(VMArray<T> &other)
// {
// 	this->count = other.count;
// 	this->max = other.max;
// 	this->data = (T*)calloc(max, sizeof(T));
// 	memcpy(this->data, other.data, sizeof(T) * this->max);

// 	return *this;
// }

// template <typename T>
// VMArray<T>& VMArray<T>::operator=(std::initializer_list<T> init)
// {
// 	this->max = ARRAY_START_SIZE;
// 	this->data = (T*)calloc(max, sizeof(T));
// 	this->count = 0;

// 	for(const T &t : init) {
// 		this->Insert(t);
// 	}

// 	return *this;
// }

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
