#pragma once
#include <GLFW/glfw3.h>
#include <string>
#include <functional>
#include <memory>
#include <vector>

class ToolWindow
{
public:
	~ToolWindow();

	static ToolWindow* Instance();

	static void DeleteInstance();

private:
	ToolWindow();

	static ToolWindow* _Instance;

public:
	void Init(GLFWwindow* window);

	void Update();

	void Terminate();

private:
	int PerlinSeed = 0;
	float PerlinFrequency = 0.01f;
	int PerlinOctaves = 1;

	int WhiteNoiseSeed = 0;

	int ColorizeR = 0;
	int ColorizeG = 0;
	int ColorizeB = 0;

	char CombineImage1[20] = {};
	char CombineImage2[20] = {};
	int index1 = 0;
	int index2 = 0;
	char Name[20] = {};
	char Name2[20] = {};
	std::unordered_map<std::string, std::vector<unsigned char>> stack;
	std::vector <std::pair<std::string, std::vector<unsigned char>>> operations;
};