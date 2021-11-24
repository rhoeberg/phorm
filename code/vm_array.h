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
	~VMArray();
	int Insert(T e);
	int InsertNew();
	int Count();
	void Free();
	void Clear();

	T& operator[](int index);
};
