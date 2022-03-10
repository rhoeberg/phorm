struct GLBuffer
{
	u32 id;
};

struct GLTexture
{
	u32 id;
};

struct GLState
{
	PArray<GLBuffer> buffers;
	PArray<Texture> textures;
};

global GLState *_glState;

void GFXLayerInit()
{
	_gfx = new GFXState();
	_glState = new GLState();
}

void CreateTexture(Texture *texture)
{
	// create default texture
	Pixel white = Pixel(255, 255, 255, 255);
	glGenTextures(1, &defaultTexture);
	glBindTexture(GL_TEXTURE_2D, defaultTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	GFXTexture result;

	result.width = texture->width;
	result.height = texture->height;

	_gfx->textures.Insert
}

u32 GFXCreateBuffer(GFXBufferType type, u64 size)
{
	i32 handle = _glState->buffers.InsertNew();
	glGenBuffers(1, &_glState->buffers[handle]);
	switch(type) {
		case BUFFER_STORAGE: {
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, _glState->buffers[handle]);
			glBufferData(GL_SHADER_STORAGE_BUFFER, size, NULL, GL_DYNAMIC_READ);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
			break;
		}
	}

	GFXBuffer gfxBuffer;
	gfxBuffer.apiHandle = handle;
	gfxBuffer.type = type;
	gfxBuffer.size = size;

	return _gfx->buffers.Insert(gfxBuffer);
}

void GFXBindBuffer(u32 handle, u32 binding = 0)
{
	GFXBuffer *gfxBuffer = _gfx->buffers[handle];
	GLBuffer *glBuffer = _glState->buffers[gfxBuffer->apiHandle];

	switch(gfxBuffer->type) {
		case BUFFER_STORAGE: {
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, glBuffer->id);
			break;
		}
	}
}

void GFXLayerCleanup()
{
	delete _gfx;
}
