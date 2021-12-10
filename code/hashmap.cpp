#include "hashmap.h"

template <typename T>
void HashMap<T>::InitializeElements()
{
	for(int i = 0; i < max; i++) {
		elements[i].next = NULL;
		elements[i].free = true;
	}
}

template <typename T>
HashMap<T>::HashMap()
{
	elements = (HashNode<T>*)malloc(sizeof(HashNode<T>) * HASHMAP_DEFAULT_SIZE);
	max = HASHMAP_DEFAULT_SIZE;

	InitializeElements();
}

template <typename T>
HashMap<T>::HashMap(int _max)
{
	data (T*)malloc(sizeof(T) * _max);
	max = _max;

	InitializeElements();
}

template <typename T>
HashMap<T>::~HashMap()
{
	free(elements);
}

template <typename T>
void HashMap<T>::Free()
{
	free(elements);
}

template <typename T>
int HashMap<T>::CalcHash(String key)
{
	int salt = 1;

	int n = 0;
	for(int i = 0; i < key.length; i++) {
		n += key[i] * salt;
	}

	return n % max;
}

template <typename T>
void HashMap<T>::Insert(String key, T value)
{
	int hash = CalcHash(key);
	HashNode<T> *next = &elements[hash];
	while(!next->free) {
		if(!next->next) {
			next->next = (HashNode<T>*)malloc(sizeof(HashNode<T>));
			break;
		}

		next = next->next;
	}

	next->value = value;
	next->key = key;
	next->next = NULL;
	next->free = true;
}

template <typename T>
T* HashMap<T>::Get(String key)
{
	int hash = CalcHash(key);
	HashNode<T> *next = &elements[hash];
	while(!next->key.Equals(key)) {

		// no key found
		if(!next->next) {
			return NULL;
		}

		next = next->next;
	}

	return &next->value;
}

void Test_HashMap() {
	HashMap<int> map = {};

	map.Insert(String("rasmus"), 33);
	map.Insert(String("louise"), 29);
	map.Insert(String("anna"), 62);
	map.Insert(String("john"), 73);

	ASSERT(*map.Get(String("rasmus")) == 33);
	ASSERT(*map.Get(String("rasmus")) != 0);
	ASSERT(*map.Get(String("rasmus")) != -1);
	ASSERT(*map.Get(String("rasmus")) != 999999);
	ASSERT(*map.Get(String("louise")) == 29);
	ASSERT(*map.Get(String("anna")) != 100);
	ASSERT(*map.Get(String("anna")) == 62);
	ASSERT(*map.Get(String("john")) == 73);

	printf("HASHMAP TEST: success\n");
}
