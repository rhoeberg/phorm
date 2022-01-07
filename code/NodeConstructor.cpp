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
	nodeConstructor->createFunc(name, nodeConstructor->op, nodeConstructor->drawFunc);
}

void AddNodeConstructor(String name, NodeOp op, NodeDrawFunc drawFunc, NodeCreateFunc createFunc)
{
	NodeConstructorState *state = _nodeConstructorState;

	state->names.Insert(name);
	NodeConstructor constructor = {};
	constructor.op = op;
	constructor.drawFunc = drawFunc;
	constructor.createFunc = createFunc;
	state->constructors.Insert(name, constructor);
}

void AddNodeConstructor(String name, NodeOp op, NodeCreateFunc createFunc)
{
	AddNodeConstructor(name, op, BaseNodeDrawFunction, createFunc);
}

void SetupNodeConstructors()
{
	/////////////////
	// TEXTURE NODES
	/////////////////
	AddNodeConstructor(String("blur texture"), BlurOperation, CreateBlurTexture);
	AddNodeConstructor(String("mix texture"), MixTextureOperation, CreateMixTexture);
	AddNodeConstructor(String("load texture"), LoadTextureOperation, CreateLoadTexture);
	AddNodeConstructor(String("video"), VideoOperation, CreateVideoNode);

	/////////////////
	// MESH NODES
	/////////////////
	AddNodeConstructor(String("cube mesh"), CubeOperation, CreateCubeNode);
	AddNodeConstructor(String("grid mesh"), GridOperation, CreateGridNode);
	AddNodeConstructor(String("noise mesh"), MeshNoiseOperation, CreateMeshNoise);


	/////////////////
	// DOUBLE NODES
	/////////////////
	AddNodeConstructor(String("double"), DoubleOperation, CreateDoubleNode);
	AddNodeConstructor(String("sin"), SinOperation, CreateSinNode);
	AddNodeConstructor(String("sinwave"), SinWaveOperation, CreateSinWaveNode);
	AddNodeConstructor(String("sawwave"), SawWaveOperation, CreateSawWaveNode);
	AddNodeConstructor(String("histogram"), HistogramOperation, DrawHistogramNode, CreateHistogramNode);
	AddNodeConstructor(String("mul"), MulOperation, DrawMulNode, CreateMulNode);
	AddNodeConstructor(String("add"), AddOperation, DrawAddNode, CreateAddNode);
	AddNodeConstructor(String("time"), TimeOperation, DrawTimeNode, CreateTimeNode);

	/////////////////
	// RENDEROBJECT NODES
	/////////////////
	AddNodeConstructor(String("renderobject"), RenderObjectOperation, CreateRenderObjectNode);
	AddNodeConstructor(String("mirror"), MirrorOperation, CreateMirrorNode);

	/////////////////
	// RENDEROBJECT GROUP NODES
	/////////////////
	AddNodeConstructor(String("combine"), CombineObjectsOperation, CreateCombineObjectsNode);

	/////////////////
	// POINTLIGHT NODES
	/////////////////
	AddNodeConstructor(String("pointlight"), PointLightOperation, CreatePointLightNode);

	/////////////////
	// VEC3 NODES
	/////////////////
	AddNodeConstructor(String("vec3"), Vec3NodeOperation, CreateVec3Node);
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

