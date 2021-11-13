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

void gui()
{
    bool show = true;

    ImGui::Begin("options", &show);
	ImGui::Text("Put your options here");
    ImGui::End();
}
