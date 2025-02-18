#include "ToolWindow.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

ToolWindow* ToolWindow::_Instance = nullptr;

ToolWindow::ToolWindow()
{
}

ToolWindow::~ToolWindow()
{
	DeleteInstance();
}

ToolWindow* ToolWindow::Instance()
{
	if (!_Instance)
	{
		_Instance = new ToolWindow();
	}
	return _Instance;
}

void ToolWindow::DeleteInstance()
{
	if (_Instance)
	{
		delete _Instance;
		_Instance = nullptr;
	}
}

void ToolWindow::Init(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsClassic();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void ToolWindow::Update()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Texture tool");
	ImGui::Text("test");
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ToolWindow::Terminate()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
