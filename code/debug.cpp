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
    ImGui::End();

}
