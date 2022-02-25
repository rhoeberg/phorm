/*
  
  optimise using compute shaders.

  approach:
  1. create compute shader
  2. create buffer object to match input texture (recreate when input changes)
       (mapped buffer object so we can read back data)
  3. use compute shader 
  4. bind buffer object
  5. dispatch
  6. memory barrier
  7. memcpy from mapped buffer to output texture
  

  todos:
  - create texture for the blurnode state
  - create pbo
  - upload texture before compute
  - compute
  - copy from pbo to output pixels
  

 */

#pragma once

void BlurOperation(Node *self)
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
	BitmapComputeState *state = GetBitmapComputeStates()->Get(self->extraHandle);
	if(!state) {
		ErrorLog("BlurNode: cannot find BlurNodeState");
		return;
	}

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

	GFXTextureBindBase(state->textureHandle, 0);

	GFXCompute(inputBitmap->width, inputBitmap->height, 1);
	GFXMemBarrier(GFX_BARRIER_IMAGE);
	state->compute.End();

	GFXTextureGetImage(state->textureHandle, state->pboHandle);
	void *mappedBuffer = GFXBufferMap(state->pboHandle);
	if(mappedBuffer) {
		memcpy(output->pixels, mappedBuffer, output->GetMemSize());
		GFXBufferUnmap(state->pboHandle);
	}
}

/* void AddBlurNodeConstructor() */
/* { */
/* 	// 1. create global blur shader */

/* 	AddNodeConstructor(String("blur texture"), BlurOperation, CreateBlurTexture, SetupBlurNode); */
/* } */

void SetupBlurNode(Node *self)
{
	BitmapComputeState state = {};
	state.compute = Shader("assets/shaders/blur.comp");
	/* state.bufferHandle = GFXBufferAdd(GFX_BUFFER_STORAGE); */
	state.pboHandle = GFXBufferAdd(GFX_BUFFER_PBO);
	state.textureHandle = GFXTextureAdd();
	self->extraHandle = GetBitmapComputeStates()->Insert(state);
}

ObjectHandle CreateBlurTexture()
{
	FixedArray<NodeParameter> params = {
	};

	FixedArray<NodeInput> inputs = {
		NodeInput(DATA_BITMAP),
	};

	return AddNode(DATA_BITMAP, params, inputs);
}
