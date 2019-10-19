#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stb_image/stb_image.h> 

#include "ShaderProgram.h"
#include "Camera.h"
#include "Quad.h"
#include "Cube.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
unsigned int loadTexture(const char *path);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
Camera camera;
float deltaTime;
float prevTime;
enum ShaderType {BLINNPHONG, NORMAL, PARRALAX, END};
std::string ShaderNames[]{"Blinn-Phong", "Normal", "Parralax"};
ShaderType shaderType = ShaderType::BLINNPHONG;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Advanced Shader Project", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	//glViewport(0, 0, 800, 600);

	std::cout << "Advanced Shader project!" << std::endl;
	std::cout << "Cycle shaders:   TAB" << std::endl;
	std::cout << "Camera controls: W, A, S, D, SPACE, SHIFT" << std::endl;
	std::cout << "Shader: " << ShaderNames[0];

	glEnable(GL_DEPTH_TEST);

	ShaderProgram normalShader;
	normalShader.CompileShader("shaders/normal.vert", "shaders/normal.frag");
	ShaderProgram parralaxShader;
	parralaxShader.CompileShader("shaders/parralax.vert", "shaders/parralax.frag");
	ShaderProgram phongShader;
	phongShader.CompileShader("shaders/blinnPhong.vert", "shaders/blinnPhong.frag");

	Quad quad;
	Quad floor;
	Cube cube;
	Cube light;

	unsigned int diffuseMap1 = loadTexture("assets/textures/wood.png");
	unsigned int normalMap1 = loadTexture("assets/textures/toy_box_normal.png");

	unsigned int diffuseMap2 = loadTexture("assets/textures/bricks2.jpg");
	unsigned int normalMap2 = loadTexture("assets/textures/bricks2_normal.jpg");
	unsigned int depthMap2 = loadTexture("assets/textures/bricks2_disp.jpg");

	
	normalShader.Use();
	normalShader.SetInt("diffuseMap", 0);
	normalShader.SetInt("normalMap", 1);

	parralaxShader.Use();
	parralaxShader.SetInt("diffuseMap", 0);
	parralaxShader.SetInt("normalMap", 1);
	parralaxShader.SetInt("depthMap", 2);


	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window))
	{
		float l_Time = glfwGetTime();
		deltaTime = l_Time - prevTime;
		prevTime = l_Time;

		processInput(window);
		camera.Update(deltaTime);

		glClearColor(0.75f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 viewMatrix = camera.GetViewMatrix();
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		//modelMatrix = glm::translate(modelMatrix, glm::vec3(2, 0, 0));
		modelMatrix = glm::rotate(modelMatrix, glm::radians((float)glfwGetTime() * -10.0f), glm::normalize(glm::vec3(0.0, 1.0, 1.0)));

		if (shaderType == ShaderType::BLINNPHONG) {
			modelMatrix = glm::mat4(1.0f);

			phongShader.Use();

			phongShader.SetVec3("light.position", lightPos);
			phongShader.SetVec3("light.ambient", 0.4f, 0.4f, 0.4f);
			phongShader.SetVec3("light.diffuse", 0.7f, 0.7f, 0.7f);
			phongShader.SetVec3("light.specular", 1.0f, 1.0f, 1.0f);
			phongShader.SetVec3("viewPos", camera.GetPosition());
			phongShader.SetMat4("viewMatrix", viewMatrix);
			phongShader.SetMat4("projectionMatrix", projectionMatrix);

			phongShader.SetVec3("material.ambient", 1.0f, 0.5f, 0.31f);
			phongShader.SetVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
			phongShader.SetVec3("material.specular", 0.5f, 0.5f, 0.5f);
			phongShader.SetFloat("material.shininess", 32.0f);
			phongShader.SetMat4("modelMatrix", modelMatrix);
			cube.Draw();

			modelMatrix = glm::translate(modelMatrix, lightPos);
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3, 0.3, 0.3));
			phongShader.SetMat4("modelMatrix", modelMatrix);
			phongShader.SetVec3("material.ambient", 10.0f, 10.0f, 10.0f);
			light.Draw();

			modelMatrix = glm::mat4(1.0f);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0,-1, 0));
			modelMatrix = glm::rotate(modelMatrix, glm::radians((float)-90), glm::normalize(glm::vec3(1.0, 0.0, 0.0)));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(5, 5, 5));
			phongShader.SetMat4("modelMatrix", modelMatrix);
			phongShader.SetVec3("material.ambient", 0.2f, 0.2f, 0.2f);
			phongShader.SetVec3("material.diffuse", 0.5f, 0.9f, 0.9f);
			phongShader.SetVec3("material.specular", 0.2f, 0.2f, 0.2f);
			phongShader.SetFloat("material.shininess", 5.0f);
			floor.Draw();
		}
		else if (shaderType == ShaderType::NORMAL) {
			normalShader.Use();
			normalShader.SetVec3("viewPos", camera.GetPosition());
			normalShader.SetVec3("lightPos", lightPos);
			normalShader.SetMat4("modelMatrix", modelMatrix);
			normalShader.SetMat4("viewMatrix", viewMatrix);
			normalShader.SetMat4("projectionMatrix", projectionMatrix);


			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, diffuseMap1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, normalMap1);

			quad.Draw();
		}
		else if (shaderType == ShaderType::PARRALAX) {
			parralaxShader.Use();
			parralaxShader.SetVec3("viewPos", camera.GetPosition());
			parralaxShader.SetVec3("lightPos", lightPos);
			parralaxShader.SetMat4("modelMatrix", modelMatrix);
			parralaxShader.SetMat4("viewMatrix", viewMatrix);
			parralaxShader.SetMat4("projectionMatrix", projectionMatrix);
			parralaxShader.SetFloat("depthScale", 0.1f);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, diffuseMap2);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, normalMap2);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, depthMap2);

			quad.Draw();
		}
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	camera.ProcessKeyboard(key, action);

	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_TAB) {
			int currentShader = shaderType;
			currentShader++;
			if (currentShader == (int)ShaderType::END)
				currentShader = 0;
			std::cout << "\rShader: " << ShaderNames[currentShader] << "		";
			shaderType = static_cast<ShaderType>(currentShader);
		}
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	camera.ProcessMouse(xpos, ypos);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}