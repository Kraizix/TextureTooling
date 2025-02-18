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
	for (auto& item : stack)
	{
		Text(item.first.c_str());
	}
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
		operations.emplace_back(std::make_pair("Perlin", textureData));
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
		operations.emplace_back(std::make_pair("White Noise", textureData));

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
		const int texWidth = 512, texHeight = 512;
		std::vector<unsigned char> textureData = Operators::Mean(operations[index1].second, operations[index2].second);
		Renderer::Instance()->SetTexture(textureData, texWidth, texHeight);
		operations.emplace_back(std::make_pair("Mean", textureData));
	}
	
	SameLine(); PushItemWidth(150.f); InputInt("Image 1##Combine", &index1, 0);
	SameLine();						  InputInt("Image 2##Combine", &index2, 0);
	Separator();

	if (Button("Save"))
	{
		stack.insert(std::make_pair(std::string(Name), operations[operations.size() - 1].second));
		operations.clear();
		const int texWidth = 512, texHeight = 512;
		Renderer::Instance()->SetTexture(std::vector<unsigned char>(), texWidth, texHeight);
	}
	SameLine(); PushItemWidth(150.f); InputText("Name##Save", Name, IM_ARRAYSIZE(Name));
	Separator();
	if (Button("Load"))
	{
		const int texWidth = 512, texHeight = 512;
		operations.emplace_back(std::make_pair(Name2, stack[Name2]));
		Renderer::Instance()->SetTexture(stack[Name2], texWidth, texHeight);
	}
	SameLine(); PushItemWidth(150.f); InputText("Name##Load", Name2, IM_ARRAYSIZE(Name2));
	End();

	// Used operators
	Begin("Used operators");
	for (auto& pair : operations)
	{
		Text(pair.first.c_str());
	}
	
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
