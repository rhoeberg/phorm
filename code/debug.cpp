void UpdateDebug()
{
	ImGui::Begin("LABELS");

	HashMap<NodeHandle> *labels = &_nodeState->labels;
	HashIter<NodeHandle> iter = HashIter<NodeHandle>(labels);

	HashNode<NodeHandle> *next = iter.Next();
	while(next) {
		// we have the next label do something
		ImGui::Text("key:%s\tvalue:%d", next->key.buffer, next->value.id);
		next = iter.Next();
	}
	ImGui::End();

	ImGui::Begin("save load");
	if(ImGui::Button("save")) {
		SaveNodes();
	}
	if(ImGui::Button("load")) {
		LoadNodes();
	}
    ImGui::End();
}
