#pragma once 

struct NodeInput;

template <typename T>
struct ObjectContainer {
	// its very important that these 3 arrays are completely synced.
	// each index in the arrays refers to the same objects
	VMArray<T> elements;
	VMArray<bool> isFree;
	VMArray<u32> slotID;

	// TODO put back type here
	HandleType handleType;
	DataType dataType;

	ObjectContainer();
	ObjectContainer(HandleType _handleType, DataType _dataType = DATA_NONE);

	// Used for serialization
	ObjectHandle InsertRaw(T e, bool _free, u32 _slotID);
	ObjectHandle Insert(T e);
	ObjectHandle InsertNew();

	// we simply set the array slot to free when removing
	// insert takes care of checking for empty slots to use instead of inserting new
	void Remove(ObjectHandle handle);
	bool Exists(ObjectHandle handle);
	T* Get(ObjectHandle handle);
	T* Get(NodeInput input);
	T* ObjectContainer<T>::GetAt(int index);

	ObjectHandle GetHandle(int index);
	int Count();
	void Clear();
	void Free();
};
