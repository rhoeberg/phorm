template <typename T>
ObjectContainer<T>::ObjectContainer(HandleType _handleType, DataType _dataType)
{
	// elements = VMArray<T>();
	// isFree = VMArray<bool>();
	// slotID = VMArray<u32>();

	handleType = _handleType;
	dataType = _dataType;
}

template <typename T>
ObjectHandle ObjectContainer<T>::InsertRaw(T e, bool _free, u32 _slotID)
{
	ObjectHandle result = {};
	result.id = elements.Insert(e);
	result.slotID = _slotID;
	isFree.Insert(_free);
	slotID.Insert(_slotID);
	result.handleType = handleType;
	result.dataType = dataType;
	result.isset = true;
	return result;
}

template <typename T>
ObjectHandle ObjectContainer<T>::Insert(T e)
{
	ObjectHandle result = {};
	// check for free slot reuse
	for(int i = 0; i < isFree.Count(); i++) {
		if(isFree[i]) {
			elements[i] = e;
			isFree[i] = false;
			slotID[i]++;

			result.id = i;
			result.slotID = slotID[i];
			result.handleType = handleType;
			result.dataType = dataType;
			result.isset = true;
			return result;
		}
	}

	// insert new slot
	result.id = elements.Insert(e);
	result.slotID = 0;
	isFree.Insert(false);
	slotID.Insert(result.slotID);
	result.handleType = handleType;
	result.dataType = dataType;
	result.isset = true;

	return result;
}

template <typename T>
ObjectHandle ObjectContainer<T>::InsertNew()
{
	ObjectHandle result = {};
	// check for free slot reuse
	for(int i = 0; i < isFree.Count(); i++) {
		if(isFree[i]) {
			isFree[i] = false;
			slotID[i]++;

			result.id = i;
			result.slotID = slotID[i];
			result.handleType = handleType;
			result.dataType = dataType;
			result.isset = true;
			return result;
		}
	}

	// insert new slot
	result.id = elements.InsertNew();
	result.slotID = 0;
	isFree.Insert(false);
	slotID.Insert(result.slotID);
	result.handleType = handleType;
	result.dataType = dataType;
	result.isset = true;

	return result;
}

// we simply set the array slot to free when removing
// insert takes care of checking for empty slots to use instead of inserting new
template <typename T>
void ObjectContainer<T>::Remove(ObjectHandle handle)
{
	// make sure handle exist to avoid removing
	// wrong slotID or handle with different type
	if(!Exists(handle)) {
		return;
	}
	
	isFree[handle.id] = true;
}

template <typename T>
bool ObjectContainer<T>::Exists(ObjectHandle handle)
{
	// wrong type
	if(handle.handleType != handleType ||
	   handle.dataType != dataType) {
		return false;
	}

	// out of bounds
	if(handle.id > (elements.Count() - 1) ||
	   handle.id < 0) {
		return false;
	}

	// slot is empty
	if(isFree[handle.id]) {
		return false;
	}

	// wrong slot id
	if(slotID[handle.id] != handle.slotID) {
		return false;
	}

	return true;
}

template <typename T>
T* ObjectContainer<T>::Get(ObjectHandle handle)
{
	if(!Exists(handle)) {
		handle.isset = false;
		return NULL;
	}

	// TODO (rhoe) make sure we have some kind of type validation here
	return &elements[handle.id];
}

template <typename T>
T* ObjectContainer<T>::Get(NodeInput input)
{
	if(!NodeExists(input.handle)) {
		return NULL;
	}

	Node *inputNode = GetNode(input.handle);

	// TODO (rhoe) make sure we have some kind of type validation here
	return &elements[inputNode->GetData().id];
}

template <typename T>
T* ObjectContainer<T>::GetAt(int index)
{
	ObjectHandle handle = GetHandle(index);
	if(!Exists(handle)) {
		return NULL;
	}
	return &elements[handle.id];
}

template <typename T>
ObjectHandle ObjectContainer<T>::GetHandle(int index)
{
	ObjectHandle handle = {};
	handle.id = index;
	handle.slotID = slotID[index];
	handle.handleType = handleType;
	handle.dataType = dataType;

	if(Exists(handle)) {
		handle.isset = true;
	}

	return handle;
}

template <typename T>
int ObjectContainer<T>::Count()
{
	return elements.Count();
}

template <typename T>
void ObjectContainer<T>::Clear()
{
	elements.Clear();
	isFree.Clear();
	slotID.Clear();
}

template <typename T>
void ObjectContainer<T>::Free()
{
	elements.Free();
	isFree.Free();
	slotID.Free();
}
