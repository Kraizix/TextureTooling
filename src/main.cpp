#ifdef NDEBUG
# define WIN32_LEAN_AND_MEAN
# define WIN32_EXTRA_LEAN
# include <Windows.h>
#endif /* NDEBUG */

#define GLEW_STATIC
#include <GL/glew.h>
#pragma comment(lib, "OpenGl32.lib")
#pragma comment(lib, "glew32s.lib")

#include <GLFW/glfw3.h>
#ifndef NDEBUG
# pragma comment(lib, "glfw3-s.lib")
#else /* !NDEBUG */
# pragma comment(lib, "glfw3-s-d.lib")
#endif /* !NDEBUG */

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <stdexcept>

float vertices[] = {
	// positions    // tex coords
	 0.5f,  0.5f,   1.0f, 1.0f,   // top right
	 0.5f, -0.5f,   1.0f, 0.0f,   // bottom right
	-0.5f, -0.5f,   0.0f, 0.0f,   // bottom left
	-0.5f,  0.5f,   0.0f, 1.0f    // top left 
};

unsigned int indices[] = {
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
};

const char* vsSrc = R"(#version 460 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;
out vec2 vTexCoord;
void main() { gl_Position = vec4(aPos, 0.0, 1.0); vTexCoord = aTexCoord; }
)";

const char* fsSrc = R"(#version 460 core
in vec2 vTexCoord;
layout(location = 0) out vec4 oFragColor;
uniform sampler2D uTexture;
void main() { oFragColor = texture(uTexture, vTexCoord); }
)";

int main(int argc, char** argv)
{
	glfwInit();

	// Set context as OpenGL 4.6 Core, forward compat, with debug depending on build config
	glfwWindowHint(GLFW_CLIENT_API,            GLFW_OPENGL_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE,        GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#ifndef NDEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT,  GL_TRUE);
#else /* !NDEBUG */
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT,  GL_FALSE);
#endif /* !NDEBUG */
	glfwWindowHint(GLFW_SRGB_CAPABLE,          GL_TRUE);
	glfwWindowHint(GLFW_DOUBLEBUFFER,          GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE,             GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 480, "Texture Tooling", nullptr, nullptr);
	if (!window)
		throw std::runtime_error("Unable to initialize GLFW");

	glfwMakeContextCurrent(window);

	glewExperimental = true;
	glewInit();

	// Create buffers and a vertex array object
	unsigned int vb, ib, va, vs, fs, sp;
	glCreateBuffers(1, &vb); glNamedBufferStorage(vb, sizeof(vertices), vertices, 0);
	glCreateBuffers(1, &ib); glNamedBufferStorage(ib,  sizeof(indices), indices, 0);
	glCreateVertexArrays(1, &va);
	glVertexArrayVertexBuffer(va, 0, vb, 0, 4 * sizeof(float));
	glVertexArrayElementBuffer(va, ib);

	// Define vertex attributes:
	// Attribute 0: position (vec2)
	glEnableVertexArrayAttrib(va, 0);
	glVertexArrayAttribFormat(va, 0, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(va, 0, 0);

	// Attribute 1: texture coordinates (vec2)
	glEnableVertexArrayAttrib(va, 1);
	glVertexArrayAttribFormat(va, 1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float));
	glVertexArrayAttribBinding(va, 1, 0);

	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vsSrc, nullptr);
	glCompileShader(vs);

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fsSrc, nullptr);
	glCompileShader(fs);

	sp = glCreateProgram();
	glAttachShader(sp, vs);
	glAttachShader(sp, fs);
	glLinkProgram(sp);

	//TEXTURE//

	//IMGUI//
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsClassic();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	/////////

	do
	{
		glfwPollEvents();

		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//IMGUI//
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		/////////

		glUseProgram(sp);
		glBindVertexArray(va);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
		glUseProgram(0);

		//IMGUI//
		ImGui::Begin("Texture tool");
		ImGui::Text("test");
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		/////////

		glfwSwapBuffers(window);
	} while (!glfwWindowShouldClose(window));

	//IMGUI//
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	/////////

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

#ifdef NDEBUG
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	return main(0, nullptr);
}
#endif /* NDEBUG */
