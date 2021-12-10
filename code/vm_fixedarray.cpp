#include "vm_fixedarray.h"

template <typename T>
FixedArray<T>::FixedArray()
{
	count = 0;
}

template <typename T>
FixedArray<T>::FixedArray(std::initializer_list<T> init)
{
	count = 0;

	for(T const &t : init) {
		Insert(t);
	}
}

template <typename T>
bool FixedArray<T>::Insert(T e)
{
	if(count < FIXED_LIST_SIZE - 1) {
		data[count] = e;
		count++;
		return true;
	}

	return false;
}

template <typename T>
int FixedArray<T>::Count()
{
	return count;
}

template <typename T>
T& FixedArray<T>::operator[](int index)
{
	//TODO (rhoe) check bounds here
	return data[index];
}
