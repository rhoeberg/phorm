#pragma once

void SceneRenderNodeOp(Node *self)
{
	Texture *output = GetTextures()->Get(self->GetDataLast());
	if(!output) {
		ErrorLog("SceneRenderNode: could not find texture output");
		return;
	}

	SceneRenderData *sceneData = GetSceneRenderDatas()->Get(self->extraHandle);
	if(!sceneData) {
		ErrorLog("SceneRenderNode: could not find sceneData");
		return;
	}

	Scene *scene = GetScenes()->Get(self->inputs[0]);
	if(!scene) {
		ErrorLog("SceneRenderNode: could not find input scene");
		return;
	}

	i32 width = self->params[0].Int();
	i32 height = self->params[1].Int();
	vec3 bgColor = self->params[2].Vec3();

	scene->bgColor = bgColor;
	sceneData->SetSize(width, height);
	output->Create(width, height);

	AddSceneRenderCommand(GetInputData(self->inputs[0]), self->GetDataLast(), self->extraHandle);
}

void SetupSceneRenderNode(Node *self)
{
	SceneRenderData sceneRenderData = {};
	sceneRenderData.Initialize();
	self->extraHandle = GetSceneRenderDatas()->Insert(sceneRenderData);
}

ObjectHandle CreateSceneRenderNode()
{
	FixedArray<NodeParameter> params = {
		NodeParameter("width", 800),
		NodeParameter("height", 600),
		NodeParameter("bg col", vec3(0, 0, 0)),
	};

	FixedArray<NodeInput> inputs = {
		NodeInput(DATA_SCENE),
	};

	return AddNode(DATA_TEXTURE, params, inputs);
}
