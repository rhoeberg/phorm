#pragma once

void VideoOperation(Node *self)
{
	// SELF
	Bitmap *output = GetBitmaps()->Get(self->GetDataLast());
	double time = self->params[0].Double();

	VideoNodeState *state = _nodeState->videoNodes.Get(self->extraHandle);
	if(!state) {
		return;
	}

	String *path = GetStrings()->Get(self->params[0].dataHandle);
	if(!path) {
		WarningLog("VideoNode: problem loading video: %s", path->buffer);
		return;
	}

	if(!self->initialized) {
		state->plm = plm_create_with_filename(path->buffer);
		if (!state->plm) {
			printf("Couldn't open file %s\n", path->buffer);
			WarningLog("VideoNode: problem loading video: %s", path->buffer);
			return;
		}
		plm_set_audio_enabled(state->plm, FALSE);

		self->initialized = true;
	}
	int w = plm_get_width(state->plm);
	int h = plm_get_height(state->plm);
	output->Create(w, h);

	plm_frame_t *frame = NULL;

	if(!plm_seek(state->plm, time, false)) {
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
		NodeParameter("path", ""),
		NodeParameter("time", 0.0),
	};

	FixedArray<NodeInput> inputs = {
	};

	return AddNode(DATA_BITMAP, params, inputs);
}
