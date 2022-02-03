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

void SetupNode(Node *node, NodeConstructor *nodeConstructor)
{
	node->op = nodeConstructor->op;
	node->drawFunc = BaseNodeDrawFunc;

	ObjectHandle dataHandle = {};
	switch(node->type) {
		case DATA_TEXTURE: {
			// dataHandle = _nodeState->textures.InsertNew();
			Texture texture = {};
			// dataHandle = AddNewTexture(TEXTURE_SIZE, TEXTURE_SIZE);
			dataHandle = GetTextures()->Insert(texture);
			break;
		}
		case DATA_MESH: {
			Mesh mesh = {};
			dataHandle = _nodeState->meshes.Insert(mesh);
			break;
		}
		case DATA_RENDEROBJECT: {
			dataHandle = AddNewRenderObject();
			break;
		}
		case DATA_RENDEROBJECT_GROUP: {
			dataHandle = _nodeState->renderObjectGroups.Insert(RenderObjectGroup());
			break;
		}
		case DATA_POINTLIGHT: {
			PointLight light = {};
			dataHandle = _nodeState->pointLights.Insert(light);
			break;
		}
		case DATA_DOUBLE: {
			double value = 0.0;
			dataHandle = _nodeState->doubles.Insert(value);
			break;
		}
		case DATA_INT: {
			int value = 0;
			dataHandle = GetInts()->Insert(value);
			break;
		}
		case DATA_VEC3: {
			vec3 v3 = {};
			dataHandle = _nodeState->vec3s.Insert(v3);
			break;
		}
		case DATA_SCENE: {
			dataHandle = GetScenes()->Insert(Scene());
		}
		case DATA_NONE: {
			break;
		}
		default: {
			NOT_IMPLEMENTED;
		}
	}
	node->SetDataHandle(dataHandle);

	if(nodeConstructor->setupFunc != NULL) 
		nodeConstructor->setupFunc(node);
}

ObjectHandle ConstructNode(String name, NodeConstructor *nodeConstructor)
{
	ObjectHandle handle = nodeConstructor->createFunc();
	Node *node = GetNode(handle);
	sprintf(node->name, "%s", name.buffer);
	node->labelHandle = AddString(name.buffer);
	SetupNode(node, nodeConstructor);

	return handle;
}

void AddNodeConstructor(String name, NodeOp op, NodeCreateFunc createFunc, NodeSetupFunc setupFunc = NULL)
{
	NodeConstructorState *state = _nodeConstructorState;

	state->names.Insert(name);
	NodeConstructor constructor = {};
	constructor.op = op;
	constructor.createFunc = createFunc;
	constructor.setupFunc = setupFunc;
	state->constructors.Insert(name, constructor);
}

void SetupNodeConstructors()
{
	/////////////////
	// TEXTURE NODES
	/////////////////
	AddNodeConstructor(String("load texture"), LoadTextureOp, LoadTextureCreate);
	AddNodeConstructor(String("blur texture"), BlurOperation, CreateBlurTexture);
	AddNodeConstructor(String("mix texture"), MixTextureOperation, CreateMixTexture);
	AddNodeConstructor(String("video"), VideoOperation, CreateVideoNode, SetupVideoNode);
	AddNodeConstructor(String("s2t"), SceneRenderNodeOp, CreateSceneRenderNode, SetupSceneRenderNode);

	/////////////////
	// SCENE NODES
	/////////////////
	AddNodeConstructor(String("scene"), SceneNodeOp, CreateSceneNode, SetupSceneNode);

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
	AddNodeConstructor(String("histogram"), HistogramOperation, CreateHistogramNode, SetupHistogramNode);
	AddNodeConstructor(String("mul"), MulOperation, CreateMulNode, SetupMulNode);
	AddNodeConstructor(String("add"), AddOperation, CreateAddNode, SetupAddNode);
	AddNodeConstructor(String("time"), TimeOperation, CreateTimeNode, SetupTimeNode);
	AddNodeConstructor(String("i2d"), I2DOp, CreateI2DNode);

	/////////////////
	// INT NODES
	/////////////////
	AddNodeConstructor(String("midicc"), MidiCCOperation, CreateMidiCCNode);

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

void BaseNodeDrawFunc(Node *node)
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

