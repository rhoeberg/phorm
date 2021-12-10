/*
  minimal hashmap with Strings as keys
  no resizing you have to choose the right size in the beginning
*/

#pragma once

#include "string.h"

#define HASHMAP_DEFAULT_SIZE 256

template <typename T>
struct HashNode {
	T value;
	String key;
	bool free;
	HashNode *next;
};

template <typename T>
class HashMap
{
	HashNode<T> *elements;
	size_t max;

	void InitializeElements();

public:
	HashMap();
	HashMap(int _max);
	~HashMap();
	void Free();
	int CalcHash(String key);
	void Insert(String key, T value);
	T* Get(String key);
};

void Test_HashMap();
