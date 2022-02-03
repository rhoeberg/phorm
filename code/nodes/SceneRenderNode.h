#pragma once

void SceneRenderNodeOp(Node *self)
{
	i32 width = self->params[0].Int();
	i32 height = self->params[1].Int();
	vec3 bgColor = self->params[2].Vec3();

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
	scene->bgColor = bgColor;

	if(sceneData->width != width || sceneData->height != height) {

		glBindFramebuffer(GL_FRAMEBUFFER, sceneData->fbo);

		glGenTextures(1, &sceneData->textureID);
		glBindTexture(GL_TEXTURE_2D, sceneData->textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sceneData->textureID, 0);

		// create RBO
		glGenRenderbuffers(1, &sceneData->rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, sceneData->rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, sceneData->rbo); // now actually attach it

		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			ErrorLog("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);  

		// Create PBO
		glGenBuffers(1, &sceneData->pbo);
		glBindBuffer(GL_PIXEL_PACK_BUFFER, sceneData->pbo);
		glBufferData(GL_PIXEL_PACK_BUFFER, width * height * sizeof(Pixel), NULL, GL_STREAM_COPY);

		sceneData->width = width;
		sceneData->height = height;
		output->Create(width, height);
	}

	AddSceneRenderCommand(GetInputData(self->inputs[0]), self->GetDataLast(), self->extraHandle);
}

void SetupSceneRenderNode(Node *self)
{
	self->extraHandle = GetSceneRenderDatas()->Insert(SceneRenderData());

	SceneRenderData *sceneData = GetSceneRenderDatas()->Get(self->extraHandle);
	glGenFramebuffers(1, &sceneData->fbo);
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
