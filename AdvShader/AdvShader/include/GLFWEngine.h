#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stb_image/stb_image.h>

#include "ShaderProgram.h"
#include "Camera.h"

class GLFWEngine {
private:
	const unsigned int m_ScrWidth = 800;
	const unsigned int m_ScrHeight = 600;
	GLFWwindow* m_Window;
	static Camera m_Camera;
	ShaderProgram m_Shader;
	float m_PrevTime;
public:
	GLFWEngine();
	bool InitWindow(unsigned int p_Width = 800, unsigned int p_Height = 600);
	//void RunEngine();
	static void ResizeWindowCallback(GLFWwindow* window, int width, int height);
	static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};