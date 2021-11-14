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

void gui(NodeState *nodeState)
{
    bool show = true;

    ImGui::Begin("debug", &show);
	ImGui::Text("mouse:%f %f", mouse.x, mouse.y);
    ImGui::End();

    ImGui::Begin("Shapes", &show);
	if(ImGui::Button("add cube")) {
		AddCube(nodeState);
	}
    ImGui::End();
}
