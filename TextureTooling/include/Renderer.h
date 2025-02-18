#pragma once

struct GLFWwindow;

class Renderer
{
public:
	~Renderer();

	static Renderer* Instance();

	static void DeleteInstance();

private:
	Renderer();

	static Renderer* _Instance;

public:
	GLFWwindow* InitWindow();

	void UpdateWindow();

	void TerminateWindow();

	void SetTexture(unsigned char* textureData, const int& texWidth, const int& texHeight);

	GLFWwindow* GetWindow() { return Window; }

private:
	unsigned int vb, ib, va, vs, fs, sp;

	GLFWwindow* Window;

	unsigned int Texture;
};
