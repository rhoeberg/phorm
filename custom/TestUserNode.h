struct MyTestData
{
	i32 i;
};

global ObjectContainer<MyTestData> *myTestDataContainer;

void TestUserOperation(Node *self)
{
	printf("hello from user node\n");

	MyTestData *data = myTestDataContainer->Get(&self->extraHandle);
	if(data != NULL)
		printf("Our custom data value is: %d\n", data->i);
}

ObjectHandle CreateTestUserNode(String name, NodeOp op, NodeDrawFunc drawFunc)
{
	FixedArray<NodeParameter> params = {
	};

	FixedArray<NodeInput> inputs = {
	};

	// To add custom data we can use the extra handle
	MyTestData testData = {};
	testData.i = 123;
	ObjectHandle extraHandle = myTestDataContainer->Insert(testData);

	return AddNode("TestUserNode", DATA_NONE, op, drawFunc, params, inputs, extraHandle);
}
