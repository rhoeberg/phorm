void InitializeNodeConstructors()
{
	_nodeConstructorState = (NodeConstructorState*)malloc(sizeof(NodeConstructorState));

	new(&_nodeConstructorState->names) VMArray<String>();
	new(&_nodeConstructorState->constructors) HashMap<NodeConstructor>(1024);
	_nodeConstructorState->nextPos = vec2(0, 0);

	SetupNodeConstructors();
}

VMArray<String>* GetNodeNames()
{
	return &_nodeConstructorState->names;
}

HashMap<NodeConstructor>* GetNodeConstructors()
{
	return &_nodeConstructorState->constructors;
}

void SetNextConstructPos(vec2 pos)
{
	_nodeConstructorState->nextPos = pos;
}

vec2 GetNextNewNodePos()
{
	return _nodeConstructorState->nextPos;
}

void ConstructNode(String name, NodeConstructor *nodeConstructor)
{
	nodeConstructor->createFunc(name, nodeConstructor->dataType, nodeConstructor->op, nodeConstructor->drawFunc);
}

void AddNodeConstructor(String name, DataType dataType, NodeOp op, NodeDrawFunc drawFunc, NodeCreateFunc createFunc)
{
	NodeConstructorState *state = _nodeConstructorState;

	state->names.Insert(name);
	NodeConstructor constructor = {};
	constructor.dataType = dataType;
	constructor.op = op;
	constructor.drawFunc = drawFunc;
	constructor.createFunc = createFunc;
	state->constructors.Insert(name, constructor);
}

void AddNodeConstructor(String name, DataType dataType, NodeOp op, NodeCreateFunc createFunc)
{
	AddNodeConstructor(name, dataType, op, BaseNodeDrawFunction, createFunc);
}

void SetupNodeConstructors()
{
	/////////////////
	// TEXTURE NODES
	/////////////////
	AddNodeConstructor(String("blur texture"), DATA_TEXTURE, BlurOperation, CreateBlurTexture);
	AddNodeConstructor(String("mix texture"), DATA_TEXTURE, MixTextureOperation, CreateMixTexture);
	AddNodeConstructor(String("load texture"), DATA_TEXTURE, LoadTextureOperation, CreateLoadTexture);
	AddNodeConstructor(String("video"), DATA_TEXTURE, VideoOperation, CreateVideoNode);

	/////////////////
	// MESH NODES
	/////////////////
	AddNodeConstructor(String("cube mesh"), DATA_MESH, CubeOperation, CreateCubeNode);
	AddNodeConstructor(String("grid mesh"), DATA_MESH, GridOperation, CreateGridNode);
	AddNodeConstructor(String("noise mesh"), DATA_MESH, MeshNoiseOperation, CreateMeshNoise);


	/////////////////
	// DOUBLE NODES
	/////////////////
	AddNodeConstructor(String("double"), DATA_DOUBLE, DoubleOperation, CreateDoubleNode);
	AddNodeConstructor(String("sin"), DATA_DOUBLE, SinOperation, CreateSinNode);
	AddNodeConstructor(String("sinwave"), DATA_DOUBLE, SinWaveOperation, CreateSinWaveNode);
	AddNodeConstructor(String("sawwave"), DATA_DOUBLE, SawWaveOperation, CreateSawWaveNode);
	AddNodeConstructor(String("histogram"), DATA_DOUBLE, HistogramOperation, DrawHistogramNode, CreateHistogramNode);
	AddNodeConstructor(String("mul"), DATA_DOUBLE, MulOperation, DrawMulNode, CreateMulNode);
	AddNodeConstructor(String("add"), DATA_DOUBLE, AddOperation, DrawAddNode, CreateAddNode);
	AddNodeConstructor(String("time"), DATA_DOUBLE, TimeOperation, DrawTimeNode, CreateTimeNode);

	/////////////////
	// RENDEROBJECT NODES
	/////////////////
	AddNodeConstructor(String("renderobject"), DATA_RENDEROBJECT, RenderObjectOperation, CreateRenderObjectNode);
	AddNodeConstructor(String("mirror"), DATA_RENDEROBJECT, MirrorOperation, CreateMirrorNode);

	/////////////////
	// RENDEROBJECT GROUP NODES
	/////////////////
	AddNodeConstructor(String("combine"), DATA_RENDEROBJECT_GROUP, CombineObjectsOperation, CreateCombineObjectsNode);

	/////////////////
	// POINTLIGHT NODES
	/////////////////
	AddNodeConstructor(String("pointlight"), DATA_POINTLIGHT, PointLightOperation, CreatePointLightNode);

	/////////////////
	// VEC3 NODES
	/////////////////
	AddNodeConstructor(String("vec3"), DATA_VEC3, Vec3NodeOperation, CreateVec3Node);
}

/*
  Returns an array with results from search
  note: clears array before use so will remove all data before
 */
void NamesBeginningWith(VMArray<String> *array, String typed)
{

	array->Clear();
	NodeConstructorState *state = _nodeConstructorState;

	// VMArray<String> results = VMArray<String>();
	for(i32 i = 0; i < state->names.Count(); i++) {
		if(state->names[i].length >= typed.length) {
			bool match = true;
			for(i32 j = 0; j < typed.length; j++) {
				// TODO (rhoe) should probably be case insensitive
				if(typed.buffer[j] != state->names[i].buffer[j]) {
					match = false;
					break;
				}
			}
			if(match) {
				// results.Insert(state->names[i]);
				array->Insert(state->names[i]);
			}
		}
	}

	// return results;
}

void BaseNodeDrawFunction(Node *node)
{
	// Rect rect = GetNodeRect(handle);
	node->rect.width = NODE_BASE_WIDTH;
	node->rect.width += node->params.Count() * PARAM_WIDTH;
	node->rect.width += node->inputs.Count() * PARAM_WIDTH;
	node->rect.height = NODE_HEIGHT;
	// Rect rect = node->rect;
	ImDrawSetColor(vec3(1.0f, 1.0f, 1.0f));
	ImDrawRect(node->rect);

	vec2 namePos = node->rect.pos + vec2(8.0f, node->rect.height - 8.0f);
	ImDrawText(namePos, node->name);
}

