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
		String path = tinyfd_saveFileDialog(NULL, NULL, 0, NULL, NULL);
		// String path("testsave.octo");
		ProjectSave(path);
	}
	if(ImGui::Button("load")) {
		String path = tinyfd_openFileDialog(NULL, NULL, 0, NULL, NULL, 0);
		// String path("testsave.octo");
		ProjectLoad(path);
	}

	ImGui::Spacing();

	if(ImGui::Button("reset time")) {
		ResetTime();
	}

	ImGui::Text("deltaTime: %f", deltaTime);

	ImGui::Text("CAMERA");
	Camera *cam = &_viewerRenderState->freeCam;
	ImGui::InputFloat3("pos", glm::value_ptr(cam->pos));
	ImGui::InputFloat3("front", glm::value_ptr(cam->front));
	ImGui::Text("speed: %f", cam->moveSpeed);
	ImGui::Text("orbitdist: %f", cam->orbitDist);
	ImGui::Text("magnitude: %f", glm::length(cam->front));

	ImGui::Spacing();
	ImGui::InputFloat2("mouse", glm::value_ptr(mouse));

	ImGui::Spacing();
	ImGui::Text("midi (channel 0, cc 1):%d", _midiState->channels[0].cc[1]);

    ImGui::End();

}
