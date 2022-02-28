/*
  minimal hashmap
  no resizing you have to choose the right size in the beginning
 
  TODO (rhoe) swap std::hash to custom generic hash type/function
*/

#pragma once

#define HASHMAP_DEFAULT_SIZE 256

template <typename K, typename V>
struct HashNode {
	V value;
	K key;
	bool free;
	HashNode *next;
};

template <typename K, typename V>
struct HashMap
{
	HashNode<K, V> *elements;
	size_t max;
	int salt;

	HashMap();
	HashMap(int _max);
	~HashMap();
	void InitializeElements();
	void Free();
	int CalcHash(K &key);
	bool Insert(K key, V value);
	bool Remove(K key);
	V* Get(K key);
	bool Exist(K key);
};

template <typename K, typename V>
struct HashIter {
	int index;
	int depth;
	HashMap<K, V> *map;

	HashIter(HashMap<K, V> *_map) {
		map = _map;
		index = 0;
		depth = 0;
	}

	HashNode<K, V>* Next() {
		// get next

		while(true) {
			HashNode<T> *next = &map->elements[index];
			if(index >= map->max) {
				return NULL;
			}

			for(int i = 0; i < depth; i++) {
				if(next->next) {
					next = next->next;
				}
			}

			// continue the iteration
			if(next->next) {
				depth++;
			}
			else {
				index++;
				depth = 0;
			}

			// found the next result
			// return it to user
			if(next && !next->free) {
				return next;
			}
		}

		return NULL;
	}
};

void Test_HashMap();
