#include <GLFW/glfw3.h>
#include <stdexcept>

#include "Renderer.h"
#include "ToolWindow.h"

int main(int argc, char** argv)
{
	Renderer* renderer = Renderer::Instance();
	GLFWwindow* window = renderer->InitWindow();

	ToolWindow* toolWindow = ToolWindow::Instance();
	toolWindow->Init(window);

	while (!glfwWindowShouldClose(window))
	{
		renderer->UpdateWindow();
		
		toolWindow->Update();

		glfwSwapBuffers(window);
	}

	toolWindow->Terminate();

	renderer->TerminateWindow();
	return 0;
}

#ifdef NDEBUG
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	return main(0, nullptr);
}
#endif /* NDEBUG */
