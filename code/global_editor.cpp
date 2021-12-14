#include "global_editor.h"

void UpdateGlobalEditor()
{
	ImGui::Begin("Inspector");
	if(_nodeEditorState->draggedNode.isset) {
		Node *node = GetNode(&_nodeEditorState->draggedNode);
		if(node) {

			ImGui::Text("%s", node->name);

			if(ImGui::Button("OP")) {
				node->CallOp();
			}

			for(int i = 0; i < node->params.Count(); i++) {

				NodeParameter *param = &node->params[i];

				// EXPOSE
				ImGui::Checkbox("##exposed", &param->exposed);
				ImGui::SameLine();

				// TODO (rhoe) be carefull about imgui widget names
				//             they are used as id's and have to be unique
				//             maybe we need to use the imgui hidden name id feature
				//             where we add the id of the object somehow
				char buffer[128];
				sprintf(buffer, "%s##%d", &param->name, _nodeEditorState->draggedNode.id);

				switch(node->params[i].type) {
					case DATA_INT: {
						if(ImGui::InputInt(buffer, &param->i)) {
							node->changed = true;
						}
						break;
					}
					case DATA_DOUBLE: {
						if(ImGui::InputDouble(buffer, &node->params[i].d)) {
							node->changed = true;
						}
						// if(ImGui::SliderScalar(buffer, ImGuiDataType_Double, &param->d, &param->d_min, &param->d_max)) {
						// node->changed = true;
						// }
						break;
					}
					case DATA_VEC3: {
						if(ImGui::InputFloat3(buffer, glm::value_ptr(param->v3))) {
							node->changed = true;
						}
						break;
					}
					case DATA_STRING: {
						// static char buf[128];
						// ImGui::InputText(buffer, buf, ARRAY_SIZE(buf));
						String *str = GetString(&param->dataHandle);
						if(ImGui::InputText(buffer, str->buffer, str->bufferSize)) {
							str->ReCalc();
							node->changed = true;
						}
						break;
					}
				}
			}
		}
	}
	ImGui::End();
}
