#pragma once

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
	VMArray(const VMArray<T>&);
	VMArray(int _max, int _count, T *_data);
	VMArray(std::initializer_list<T> init);
	~VMArray();
	int Insert(const T &e);
	int InsertNew();
	int Count();
	int Max();
	T* Data();
	void Free();
	void Clear();

	T& operator[](int index);
};
