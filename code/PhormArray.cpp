#include "PhormArray.h"

template <typename T>
PArray<T>::PArray()
{
	max = ARRAY_START_SIZE;
	data = (T*)calloc(max, sizeof(T));
	count = 0;
	initialized = true;
}

template <typename T>
PArray<T>::PArray(const PArray<T>& old)
{
	count = old.count;
	max = old.max;
	data = (T*)calloc(max, sizeof(T));
	memcpy(data, old.data, sizeof(T) * max);
	initialized = true;
}


// TODO (rhoe) more space efficient init list
// we could save memory by setting max to the size of the init list
// this way the array would possibly create a better contextual size
template <typename T>
PArray<T>::PArray(std::initializer_list<T> init)
{
	max = ARRAY_START_SIZE;
	data = (T*)calloc(max, sizeof(T));
	count = 0;

	for(const T &t : init) {
		Insert(t);
	}

	initialized = true;
}

template <typename T>
PArray<T>::PArray(int _max, int _count, T *_data)
{
	max = _max;
	count = _count;
	data = (T*)calloc(max, sizeof(T));
	memcpy(data, _data, count * sizeof(T));
	initialized = true;
}

template <typename T>
PArray<T>::PArray(u64 _size, T *_data)
{
	max = _size;
	count = _size;
	data = (T*)calloc(_size, sizeof(T));
	memcpy(data, _data, count * sizeof(T));
	initialized = true;
}

template <typename T>
PArray<T>::PArray(int _size)
{
	max = _size;
	count = _size;
	data = (T*)calloc(max, sizeof(T));
	initialized = true;
}

template <typename T>
PArray<T>::~PArray()
{
	// TODO (rhoe) for now we call Free manually on arrays when we need to
	// if(data != NULL)
	//   Free();
}

template <typename T>
void PArray<T>::Grow()
{
	int oldMax = max;
	max *= 4;
	u64 size = max * sizeof(T);
	T* tmp = data;
	data = (T*)calloc(max, sizeof(T));
	memcpy(data, tmp, oldMax * sizeof(T));
	free(tmp);
}

template <typename T>
int PArray<T>::Insert(const T &e)
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
int PArray<T>::InsertNew()
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
int PArray<T>::Count()
{
	return count;
}

template <typename T>
void PArray<T>::RemoveLast()
{
	if(count > 0) {
		count--;
	}
}

// TODO (rhoe) maybe pop should return a copy instead of pointer
//             since freeing between the pop and using the T*
//             would invalidate the T*
template <typename T>
T* PArray<T>::PopLast()
{
	if(count > 0) {
		T* result = &data[count-1];
		RemoveLast();
		return result;
	}

	return NULL;
}

template <typename T>
int PArray<T>::Max()
{
	return max;
}

// TODO (rhoe) this function is not testet
template <typename T>
bool PArray<T>::Contains(const T &e)
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
T* PArray<T>::Data()
{
	return data;
}

template <typename T>
T& PArray<T>::operator[](int index)
{
	//TODO (rhoe) check bounds here
	return data[index];
}

// template <typename T>
// PArray<T>& PArray<T>::operator=(PArray<T> &other)
// {
// 	this->count = other.count;
// 	this->max = other.max;
// 	this->data = (T*)calloc(max, sizeof(T));
// 	memcpy(this->data, other.data, sizeof(T) * this->max);

// 	return *this;
// }

// template <typename T>
// PArray<T>& PArray<T>::operator=(std::initializer_list<T> init)
// {
// 	max = ARRAY_START_SIZE;
// 	data = (T*)calloc(max, sizeof(T));
// 	count = 0;

// 	for(const T &t : init) {
// 		Insert(t);
// 	}

// 	initialized = true;

// 	return *this;
// }
// 	this->max = ARRAY_START_SIZE;
// 	this->data = (T*)calloc(max, sizeof(T));
// 	this->count = 0;

// 	for(const T &t : init) {
// 		this->Insert(t);
// 	}

// 	return *this;
// }

template <typename T>
void PArray<T>::Free()
{
	if(initialized && data != NULL)
		free(data);
}

// very cheap clear.
// we simpy set the count to 0, which means there will most likely
// be leftover data in the array, but since we care more about performance than
// security this has been the priorty
template <typename T>
void PArray<T>::Clear()
{
	count = 0;
}
