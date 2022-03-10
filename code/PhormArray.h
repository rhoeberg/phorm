#pragma once

#define ARRAY_START_SIZE 16

template <typename T>
class PArray {
 private:
	T *data;
	int max;
	int count;
	bool initialized;
	void Grow();
 public:
	PArray();
	PArray(const PArray<T>&);
	PArray(int _max, int _count, T *_data);
	PArray(u64 _size, T *_data);
	PArray(std::initializer_list<T> init);
	PArray(int _size);
	~PArray();
	int Insert(const T &e);
	int InsertNew();
	int Count();
	int Max();
	bool Contains(const T &e);
	T* Data();
	void Free();
	void Clear();

	T& operator[](int index);
	// PArray<T>& operator=(PArray<T> &other);
	// PArray<T>& operator=(std::initializer_list<T> init);
};
