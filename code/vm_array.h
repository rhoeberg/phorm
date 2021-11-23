#pragma once

using namespace std;

#define ARRAY_START_SIZE 16

template <typename T>
class VMArray {
 private:
	T *data;
	int max;
	int count;
	void Grow();
 public:
	VMArray();
	int Insert(T e);
	T* InsertNew();
	void Free();
	int Count();

	T& operator[](int index);
};
