#pragma once

void OutputNodeOp(Node *self)
{

	// TODO (rhoe) HACKING POST PROCESSING EFFECTS HERE
	_viewerRenderState->shader.Begin();
	double waveAmount = self->params[0].Double();
	double waveFreq = self->params[1].Double();
	_viewerRenderState->shader.SetUniform("WaveAmount", (float)waveAmount);
	_viewerRenderState->shader.SetUniform("WaveFreq", (float)waveFreq);
	_viewerRenderState->shader.End();
	// ==================================================

	Bitmap *input = GetBitmaps()->Get(self->inputs[0]);
	if(!input) {
		ErrorLog("Output node: invalid input texture\n");
		return;
	}
	/* Texture *output = GetTextures()->Get(self->GetDataLast()); */
	/* if(!input) { */
	/* 	ErrorLog("Output node: invalid output texture\n"); */
	/* 	return; */
	/* } */
	/* output->Create(input->width, input->height); */
	/* memcpy(output->pixels, input->pixels, sizeof(Pixel) * input->width * input->height); */
}

ObjectHandle CreateOutputNode()
{
	FixedArray<NodeParameter> params = {
		// TODO (rhoe) HACKING POST PROCESSING EFFECTS HERE
		NodeParameter("wave amount", 0.0),
		NodeParameter("wave freq", 1.0),
		// ==================================================
	};

	FixedArray<NodeInput> inputs = {
		NodeInput(DATA_BITMAP),
	};

	return AddNode(DATA_NONE, params, inputs);
}
