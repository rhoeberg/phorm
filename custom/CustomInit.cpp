/*

  Custom user code goes here.

 */
void CustomInit()
{

	/////////////
	// You can allocate memory for custom data here
	// myTestDataContainer = (ObjectContainer<MyTestData>*)malloc(sizeof(ObjectContainer<MyTestData>));
	// new(myTestDataContainer) ObjectContainer<MyTestData>();


	/////////////
	// Add New Node
	//
	// to use your custom node in the system
	// you need to add a constructor to your node
	// this is done by calling AddNodeConstructor
	// and giving it a pointer to your custom node
	// operation function and node creation function
	// (and optionally a pointer to a custom node
	// drawing function for custom UI drawing)
	//
	// Node Operation function needs following signature:
	// Operation(Node *self)
	//
	// Node Creation signature
	// ObjectHandle Create(String name, DataType dataType, NodeOp op, NodeDrawFunc DrawFunc)
	// 
	// OPTIONAL
	// NodeDrawFunc signature (for the UI)
	// void DrawFunction(Node *self)
	// the drawing function pointer is the last optional parameter
	// of AddNodeConstructor, if none are given it defaults to
	// BaseNodeDrawFunction 
	//
	//
	//
	// Example:
	// AddNodeConstructor(String("Test User Node"), TestUserOperation, CreateTestUserNode);

	AddNodeConstructor(String("BlazeReader"), BlazeReaderOp, CreateBlazeReaderNode);
	AddNodeConstructor(String("BlazeOutput"), BlazeOutputOp, CreateBlazeOutputNode);

}

void CleanupCustom()
{
	// myTestDataContainer->Free();
	// free(myTestDataContainer);
	// myTubes.Free();
}
