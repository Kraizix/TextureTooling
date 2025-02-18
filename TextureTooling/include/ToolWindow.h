#pragma once
#include <GLFW/glfw3.h>

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
};