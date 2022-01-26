void UpdateDebug()
{
	// ImGui::Begin("LABELS");
	// HashMap<ObjectHandle> *labels = &_nodeState->labels;
	// HashIter<ObjectHandle> iter = HashIter<ObjectHandle>(labels);

	// HashNode<ObjectHandle> *next = iter.Next();
	// while(next) {
	// 	// we have the next label do something
	// 	ImGui::Text("key:%s\tvalue:%d", next->key.buffer, next->value.id);
	// 	next = iter.Next();
	// }
	// ImGui::End();

	ImGui::Begin("debug");
	if(ImGui::Button("save")) {
		SaveNodes();
	}
	if(ImGui::Button("load")) {
		LoadNodes();
	}

	ImGui::Spacing();

	if(ImGui::Button("reset time")) {
		ResetTime();
	}

	ImGui::Text("deltaTime: %f", deltaTime);

	ImGui::Text("CAMERA");
	ImGui::InputFloat3("pos", glm::value_ptr(_viewerRenderState.cam.pos));
	ImGui::InputFloat3("front", glm::value_ptr(_viewerRenderState.cam.front));
	ImGui::Text("speed: %f", _viewerRenderState.cam.moveSpeed);
	ImGui::Text("orbitdist: %f", _viewerRenderState.cam.orbitDist);

	ImGui::Spacing();
	ImGui::InputFloat2("mouse", glm::value_ptr(mouse));

	ImGui::Spacing();
	ImGui::Text("midi (channel 0, cc 1):%d", _midiState->channels[0].cc[1]);

    ImGui::End();

}
