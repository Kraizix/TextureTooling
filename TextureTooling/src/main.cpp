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

	//NOISE//
	const int texWidth = 512, texHeight = 512;
	unsigned char* textureData = new unsigned char[texWidth * texHeight * 3];
	for (int y = 0; y < texHeight; y++)
	{
		for (int x = 0; x < texWidth; x++)
		{
			// Normalize pixel coordinates to [0,1]
			float nx = x / static_cast<float>(texWidth);
			float ny = y / static_cast<float>(texHeight);
			// Scale coordinates to zoom into the noise pattern.
			unsigned char value = std::rand() % 255;
			int index = (y * texWidth + x) * 3;
			// Create a grayscale texture.
			textureData[index] = value;
			textureData[index + 1] = value;
			textureData[index + 2] = value;
		}
	}

	renderer->SetTexture(textureData, texWidth, texHeight);

	delete[] textureData;
	/////////

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
