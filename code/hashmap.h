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
struct HashMap
{
	HashNode<T> *elements;
	size_t max;
	int salt;

	HashMap();
	HashMap(int _max);
	~HashMap();
	void InitializeElements();
	void Free();
	int CalcHash(String &key);
	void Insert(String &key, T value);
	bool Remove(String &key);
	T* Get(String key);
	bool Exist(String &key);
};

template <typename T>
struct HashIter {
	int index;
	int depth;
	HashMap<T> *map;

	HashIter(HashMap<T> *_map) {
		map = _map;
		index = 0;
		depth = 0;
	}

	HashNode<T>* Next() {
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

			// found the next result
			// return it to user
			if(next && !next->free) {
				return next;
			}

			// continue the iteration
			if(next->next) {
				depth++;
			}
			else {
				index++;
				depth = 0;
			}
		}

		return NULL;
	}
};

void Test_HashMap();
