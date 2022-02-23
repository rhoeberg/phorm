#pragma once

void WaveyOp(Node *self)
{
	// INPUTS
	Bitmap *inputBitmap = GetBitmaps()->Get(self->inputs[0]);
	if(!inputBitmap) {
		ErrorLog("BlurNode: cannot find input bitmap");
		return;
	}

	// SELF
	ObjectHandle handle = self->GetDataLast();
	Bitmap *output = GetBitmaps()->Get(handle);
	if(!output) {
		ErrorLog("BlurNode: cannot find output bitmap");
		return;
	}

	// STATE
	BlurNodeState *state = GetBlurNodes()->Get(self->extraHandle);
	if(!state) {
		ErrorLog("BlurNode: cannot find BlurNodeState");
		return;
	}

	int amount = self->params[0].Int();
	int offset = self->params[1].Int();
	double freq = self->params[2].Double();

	output->Create(inputBitmap->width, inputBitmap->height);

	if(state->width != inputBitmap->width ||
	   state->height != inputBitmap->height) {
		state->width = inputBitmap->width;
		state->height = inputBitmap->height;

		// create buffers matching size of inputbitmap
		GFXBufferSetData(state->pboHandle, inputBitmap->GetMemSize(), GFX_STREAM_COPY);
	}

	GFXTextureUploadBitmap(state->textureHandle, inputBitmap);

	state->compute.Begin();
	state->compute.SetUniform("offset", offset);
	state->compute.SetUniform("amount", amount);
	state->compute.SetUniform("freq", (float)freq);

	GFXTextureBindBase(state->textureHandle, 0);

	GFXCompute(inputBitmap->width, inputBitmap->height, 1);
	GFXMemBarrier(GFX_BARRIER_IMAGE);
	state->compute.End();
	/* GFXFinish(); */

	GFXTextureGetImage(state->textureHandle, state->pboHandle);
	void *mappedBuffer = GFXBufferMap(state->pboHandle);
	if(mappedBuffer) {
		memcpy(output->pixels, mappedBuffer, output->GetMemSize());
		GFXBufferUnmap(state->pboHandle);
	}
}

void SetupWaveyNode(Node *self)
{
	BlurNodeState state = {};
	state.compute = Shader("assets/shaders/wavey.comp");
	state.pboHandle = GFXBufferAdd(GFX_BUFFER_PBO);
	state.textureHandle = GFXTextureAdd();
	self->extraHandle = GetBlurNodes()->Insert(state);
}

ObjectHandle CreateWaveyNode()
{
	FixedArray<NodeParameter> params = {
		NodeParameter("amount", 20),
		NodeParameter("offset", 0),
		NodeParameter("freq", 0.0),
	};

	FixedArray<NodeInput> inputs = {
		NodeInput(DATA_BITMAP),
	};

	return AddNode(DATA_BITMAP, params, inputs);
}
