#pragma once

void NoiseBitmapOp(Node *self)
{
	// OUTPUT
	Bitmap *output = GetBitmaps()->Get(self->GetDataLast());
	if(!output) {
		ErrorLog("BlurNode: cannot find output bitmap");
		return;
	}


	i32 width = self->params[0].Int();
	i32 height = self->params[1].Int();
	i32 octaves = self->params[2].Int();
	double persistance = self->params[3].Double();
	double freq = self->params[4].Double();

	output->Create(width, height);

	for(i32 x = 0; x < width; x++) {
		for(i32 y = 0; y < height; y++) {
			i32 index = GetPixelIndex(x, y, width);
			Pixel *pixel = &output->pixels[index];
			pixel->r = (i32)(256 * Noise2D(x, y, octaves, persistance, freq));
			pixel->g = (i32)(256 * Noise2D(x, y, octaves, persistance, freq));
			pixel->b = (i32)(256 * Noise2D(x, y, octaves, persistance, freq));
			pixel->a = 1.0f;
		}
	}
}

ObjectHandle CreateNoiseBitmapNode()
{
	FixedArray<NodeParameter> params = {
		NodeParameter("width", 512),
		NodeParameter("heigth", 512),
		NodeParameter("octaves", 1),
		NodeParameter("persistance", 1.0),
		NodeParameter("freq", 1.0),
	};

	FixedArray<NodeInput> inputs = {
		NodeInput(DATA_BITMAP),
	};

	return AddNode(DATA_BITMAP, params, inputs);
}
