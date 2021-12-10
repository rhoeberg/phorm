#pragma once

#define FIXED_LIST_SIZE 12

template <typename T>
class FixedArray {
	T data[FIXED_LIST_SIZE];
	int count;

public:
	FixedArray();
	FixedArray(std::initializer_list<T> init);
	bool Insert(T e);
	int Count();
	T& operator[](int index);
};
