#pragma once

void CameraSwitchOp(Node *self)
{
	i32 selected = self->params[0].Int();
	Node *selectedNode = GetNode(self->inputs[selected].handle);
	if(selectedNode) 
		self->SetDataHandle(selectedNode->GetData());
}

ObjectHandle CreateCameraSwitch()
{
	FixedArray<NodeParameter> params = {
		NodeParameter("selected", 0),
	};

	FixedArray<NodeInput> inputs = {
		NodeInput(DATA_CAMERA),
		NodeInput(DATA_CAMERA),
		NodeInput(DATA_CAMERA),
	};

	return AddNode(DATA_CAMERA, params, inputs);
}

void AddCameraSwitchConstructor()
{
	NodeConstructor constructor = {};
	constructor.op = CameraSwitchOp;
	constructor.createFunc = CreateCameraSwitch;
	constructor.setupFunc = NULL;
	constructor.noCache = true;

	AddNodeConstructor("cam switch", constructor);
}
