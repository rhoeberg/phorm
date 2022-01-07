#include "hashmap.h"

template <typename T>
void HashMap<T>::InitializeElements()
{
	for(int i = 0; i < max; i++) {
		elements[i].free = true;
		elements[i].next = NULL;
	}
}

template <typename T>
HashMap<T>::HashMap()
{
	elements = (HashNode<T>*)calloc(1, sizeof(HashNode<T>) * HASHMAP_DEFAULT_SIZE);
	max = HASHMAP_DEFAULT_SIZE;
	salt = 0;

	InitializeElements();
}

template <typename T>
HashMap<T>::HashMap(int _max)
{
	elements = (HashNode<T>*)calloc(1, sizeof(HashNode<T>) * _max);
	max = _max;
	salt = 0;

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

// TODO (rhoe) implement better hashing algo
template <typename T>
int HashMap<T>::CalcHash(String &key)
{
	int n = 0;
	for(int i = 0; i < key.length; i++) {
		n += key[i] * (salt + 1);
	}

	return n % max;
}

template <typename T>
void HashMap<T>::Insert(String key, T value)
{
	int hash = CalcHash(key);
	HashNode<T> *cur = &elements[hash];
	while(!cur->free) {
		if(!cur->next) {
			// found unused pointer for value
			cur->next = (HashNode<T>*)calloc(1, sizeof(HashNode<T>));
			cur = cur->next;
			break;
		}

		cur = cur->next;
	}

	cur->value = value;
	cur->key = key;
	cur->next = NULL;
	cur->free = false;
}

/*
  Returns true if we found a key and removed it
  Returns false if the key didnt exist
*/
template <typename T>
bool HashMap<T>::Remove(String key)
{
	int hash = CalcHash(key);
	HashNode<T> *next = &elements[hash];
	int layer = 0;
	while(!next->key.Equals(key)) {

		if(!next->next) {
			// no key found
			return false;
		}

		next = next->next;
		layer++;
	}

	// we found the hashnode we want to free
	next->free = true;

	// if target key is chain parent then move chain up
	if(next->next) {
		HashNode<T> *tmp = next->next;
		*next = *next->next;
		free(next->next);
	}

	return true;
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

template <typename T>
bool HashMap<T>::Exist(String key)
{
	int hash = CalcHash(key);
	HashNode<T> *next = &elements[hash];
	while(!next->key.Equals(key)) {

		// no key found
		if(!next->next) {
			return false;
		}

		next = next->next;
	}

	return true;
}

void Test_HashMap()
{

	printf("HASHMAP TEST: starting\n");

	{
		// test normal hashmap
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
	}

	{
		// test with salt 0 to test collisions;
		// all strings will hash to 0

		HashMap<int> map = HashMap<int>();
		map.salt = -1;

		map.Insert(String("rasmus"), 33);
		map.Insert(String("louise"), 29);
		map.Insert(String("peter"), 100);

		ASSERT(map.Exist(String("louise")));

		map.Remove(String("louise"));
		map.Remove(String("rasmus"));

		ASSERT(!map.Exist(String("rasmus")));
		ASSERT(!map.Exist(String("louise")));
		ASSERT(map.Exist(String("peter")));
	}

	printf("HASHMAP TEST: success\n");
}
