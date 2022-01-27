#pragma once

void VideoOperation(Node *self)
{
	// SELF
	Texture *output = GetTextures()->Get(self->GetDataLast());
	double time = self->params[0].Double();

	VideoNodeState *state = _nodeState->videoNodes.Get(self->extraHandle);
	if(!state) {
		return;
	}

	/* VideoNodeState *state = &_nodeState->videoNodes[self->extraHandle]; */

	if(!self->initialized) {
		const char *filename = "assets/output.mpg";
		/* plm_t *plm = plm_create_with_filename(filename); */
		state->plm = plm_create_with_filename(filename);
		/* uint8_t *rgb_buffer = (uint8_t *)malloc(w * h * 3); */
		if (!state->plm) {
			printf("Couldn't open file %s\n", filename);
			exit(1);
			// TODO (rhoe) we need to handle this without quitting
		}
		plm_set_audio_enabled(state->plm, FALSE);

		self->initialized = true;
	}
	int w = plm_get_width(state->plm);
	int h = plm_get_height(state->plm);
	output->Create(w, h);

	plm_frame_t *frame = NULL;

	if(!plm_seek(state->plm, time, true)) {
		DebugLog("couldn't seek frame\n");
	}
	else {
		frame = plm_decode_video(state->plm);
		plm_frame_to_rgba(frame, (uint8_t*)(output->pixels), w * 4);
	}
}

void SetupVideoNode(Node *self)
{
	self->extraHandle = _nodeState->videoNodes.Insert(VideoNodeState());
}

ObjectHandle CreateVideoNode()
{
	FixedArray<NodeParameter> params = {
		NodeParameter("time", 0.0),
	};

	FixedArray<NodeInput> inputs = {
	};

	return AddNode(DATA_TEXTURE, params, inputs);
}
