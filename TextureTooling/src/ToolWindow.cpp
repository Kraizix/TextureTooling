#include "ToolWindow.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "Operators.h"
#include "Renderer.h"

using namespace ImGui;

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
	CreateContext();
	ImGuiIO& io = GetIO(); (void)io;
	StyleColorsClassic();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void ToolWindow::Update()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	NewFrame();

	Begin("Stack");
	End();

	Begin("Main");
	End();

	// Operators
	Begin("Operators");
	if (Button("Perlin"))
	{
		const int texWidth = 512, texHeight = 512;
		std::vector<unsigned char> textureData = Operators::Perlin(texWidth, texHeight, PerlinSeed, PerlinFrequency, PerlinOctaves);
		Renderer::Instance()->SetTexture(textureData, texWidth, texHeight);
	}
	SameLine(); PushItemWidth(60.f); InputInt("Seed##Perlin", &PerlinSeed, 0); 
	SameLine(); PushItemWidth(40.f); InputFloat("Frequency##Perlin", &PerlinFrequency);
	SameLine(); PushItemWidth(30.f); InputInt("Octaves##Perlin", &PerlinOctaves, 0);
	Separator();

	if (Button("White noise"))
	{
		const int texWidth = 512, texHeight = 512;
		std::vector<unsigned char> textureData = Operators::WhiteNoise(texWidth, texHeight, WhiteNoiseSeed);
		Renderer::Instance()->SetTexture(textureData, texWidth, texHeight);
	}
	SameLine(); PushItemWidth(60.f); InputInt("Seed##WhiteNoise", &WhiteNoiseSeed, 0);
	Separator();

	if (Button("Colorize"))
	{

	}
	SameLine(); InputInt("R##Colorize", &ColorizeR, 0); 
	SameLine(); InputInt("G##Colorize", &ColorizeG, 0); 
	SameLine(); InputInt("B##Colorize", &ColorizeB, 0);
	Separator();

	if (Button("Combine"))
	{

	}
	SameLine(); PushItemWidth(150.f); InputText("Image 1##Combine", CombineImage1, IM_ARRAYSIZE(CombineImage1));
	SameLine();						  InputText("Image 2##Combine", CombineImage2, IM_ARRAYSIZE(CombineImage2));
	Separator();

	if (Button("Save"))
	{

	}
	End();

	// Used operators
	Begin("Used operators");
	End();

	Render();
	ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());
}

void ToolWindow::Terminate()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	DestroyContext();
}
