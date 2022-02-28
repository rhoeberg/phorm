#include "hashmap.h"

template <typename K, typename V>
void HashMap<K, V>::InitializeElements()
{
	for(int i = 0; i < max; i++) {
		elements[i].free = true;
		elements[i].next = NULL;
	}
}

template <typename K, typename V>
HashMap<K, V>::HashMap()
{
	elements = (HashNode<K, V>*)calloc(1, sizeof(HashNode<K, V>) * HASHMAP_DEFAULT_SIZE);
	max = HASHMAP_DEFAULT_SIZE;
	salt = 0;

	InitializeElements();
}

template <typename K, typename V>
HashMap<K, V>::HashMap(int _max)
{
	elements = (HashNode<K, V>*)calloc(1, sizeof(HashNode<K, V>) * _max);
	max = _max;
	salt = 0;

	InitializeElements();
}

template <typename K, typename V>
HashMap<K, V>::~HashMap()
{
	free(elements);
}

template <typename K, typename V>
void HashMap<K, V>::Free()
{
	free(elements);
}

// TODO (rhoe) implement better hashing algo
template <typename K, typename V>
int HashMap<K, V>::CalcHash(K &key)
{
	size_t hash = std::hash<K>{}(key);
	return hash % max;

	// int n = 0;
	// for(int i = 0; i < key.length; i++) {
	// 	n += key[i] * (salt + 1);
	// }

	// return n % max;
}

template <typename K, typename V>
bool HashMap<K, V>::Insert(K key, V value)
{
	if(Exist(key))
		return false;

	int hash = CalcHash(key);
	HashNode<K, V> *cur = &elements[hash];
	while(!cur->free) {
		if(!cur->next) {
			// found unused pointer for value
			cur->next = (HashNode<K, V>*)calloc(1, sizeof(HashNode<K, V>));
			cur = cur->next;
			break;
		}

		cur = cur->next;
	}

	cur->value = value;
	cur->key = key;
	cur->next = NULL;
	cur->free = false;

	return true;
}

/*
  Returns true if we found a key and removed it
  Returns false if the key didnt exist
*/
template <typename K, typename V>
bool HashMap<K, V>::Remove(K key)
{
	int hash = CalcHash(key);
	HashNode<K, V> *next = &elements[hash];
	int layer = 0;
	while(next->key != key) {

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
		HashNode<K, V> *tmp = next->next;
		*next = *next->next;
		free(next->next);
	}

	return true;
}

template <typename K, typename V>
V* HashMap<K, V>::Get(K key)
{
	int hash = CalcHash(key);
	HashNode<K, V> *next = &elements[hash];
	while(!(next->key == key)) {

		// no key found
		if(!next->next) {
			return NULL;
		}

		next = next->next;
	}

	return &next->value;
}

template <typename K, typename V>
bool HashMap<K, V>::Exist(K key)
{
	int hash = CalcHash(key);
	HashNode<K, V> *next = &elements[hash];
	while(next->key != key) {
		// no key found
		if(!next->next) {
			return false;
		}

		next = next->next;
	}

	return !next->free;
}

void Test_HashMap()
{

	printf("HASHMAP TEST: starting\n");

	{
		// test normal hashmap
		HashMap<String, int> map = {};

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

		HashMap<String, int> map = HashMap<String, int>();
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
