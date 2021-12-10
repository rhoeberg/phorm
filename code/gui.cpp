// TODO (rhoe) make some smart drag and drop stuff
global char droppedFile[128];
global bool readyToConsumeDropped;

void initializeGUI(GLFWwindow *win)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    const char* glsl_version = "#version 150";
    ImGui_ImplGlfw_InitForOpenGL(win, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

// void NodeInspector()
// {
// 	int nodeHandle = GetSelectedNodeHandle();
// 	Node* node = GetNode(nodeHandle);

// 	if(node != NULL) {
// 		bool show = true;
// 		ImGui::Begin("Inspector", &show);
// 		ImGui::Text("%s\n", node->name);

// 		int outputSize = GetNodeOutputSize(nodeHandle);
// 		for(int i = 0; i < outputSize; i++) {
// 			switch(node->outputs[i].type) {
// 				case DATA_TYPE_VEC3: {
// 					ImGui::InputFloat3("vec3", glm::value_ptr(node->outputs[i].data.v3));
// 					break;
// 				}
// 				case DATA_TYPE_SCALAR: {
// 					ImGui::InputFloat("scalar", &node->outputs[i].data.scalar);
// 					break;
// 				}
// 				case DATA_TYPE_TEXTURE: {
// 					static char strBuffer[128] = "";
// 					if(readyToConsumeDropped) {
// 						sprintf(strBuffer, "%s", droppedFile);
// 						node->outputs[i].data.texture = createTexture(strBuffer);
// 						readyToConsumeDropped = false;
// 					}
// 					ImGui::InputText("filePath", strBuffer, ARRAY_SIZE(strBuffer));
// 					if(ImGui::Button("load texture")) {
// 						node->outputs[i].data.texture = createTexture(strBuffer);
// 					}
// 					ImGui::Text("texture id:%d\n", node->outputs[i].data.texture);
// 					break;
// 				}
// 			}
// 		}

// 		ImGui::End();
// 	}
// }

// void NodeUtils()
// {
//     bool show = true;

//     ImGui::Begin("debug", &show);
// 	ImGui::Text("mouse:%f %f", mouse.x, mouse.y);
//     ImGui::End();

//     ImGui::Begin("Shapes", &show);
// 	if(ImGui::Button("cube")) {
// 		AddCubeNode();
// 	}
// 	if(ImGui::Button("debug")) {
// 		AddDebugNode();
// 	}
// 	if(ImGui::Button("vec3")) {
// 		AddVec3Node();
// 	}
// 	if(ImGui::Button("time")) {
// 		AddTimeNode();
// 	}
// 	if(ImGui::Button("sin")) {
// 		AddSinNode();
// 	}
// 	if(ImGui::Button("texture")) {
// 		AddTextureNode();
// 	}
//     ImGui::End();
// }

void gui()
{
    bool show = true;

    ImGui::Begin("debug", &show);
	ImGui::Text("mouse:%f %f", mouse.x, mouse.y);
	if(ImGui::Button("save")) {
		SaveNodes();
	}
	if(ImGui::Button("load")) {
		LoadNodes();
	}
    ImGui::End();
}
