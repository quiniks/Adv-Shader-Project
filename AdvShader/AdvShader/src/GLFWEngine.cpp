#include "..\include\GLFWEngine.h"

GLFWEngine::GLFWEngine()
{
	
}

bool GLFWEngine::InitWindow(unsigned int p_Width = 800, unsigned int p_Height = 600)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	m_Window = glfwCreateWindow(p_Width, p_Height, "Advanced Shader Project", NULL, NULL);
	if (m_Window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(m_Window);
	glfwSetFramebufferSizeCallback(m_Window, ResizeWindowCallback);
	glfwSetCursorPosCallback(m_Window, MouseCallback);
	glfwSetKeyCallback(m_Window, KeyCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	glEnable(GL_DEPTH_TEST);

	return true;
}
/*
void GLFWEngine::RunEngine()
{
	while (!glfwWindowShouldClose(m_Window))
	{
		////Rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		vertexShaderProgram.Use();

		glm::mat4 viewMatrix = glm::mat4(1.0f);
		glm::mat4 projectionMatrix;
		//modelMatrix = glm::rotate(modelMatrix, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//modelMatrix = glm::rotate(modelMatrix, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, -3.0f));
		projectionMatrix = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		vertexShaderProgram.SetMat4("view", viewMatrix);
		vertexShaderProgram.SetMat4("projection", projectionMatrix);

		//glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 modelMatrix = glm::mat4(1.0f);
			modelMatrix = glm::translate(modelMatrix, cubePositions[i]);
			float angle = 20.0f * i;
			modelMatrix = glm::rotate(modelMatrix, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
			vertexShaderProgram.SetMat4("model", modelMatrix);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);
		////Events & swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}
*/
void GLFWEngine::ResizeWindowCallback(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void GLFWEngine::MouseCallback(GLFWwindow * window, double xpos, double ypos)
{
	m_Camera.ProcessMouse(xpos, ypos);
}

void GLFWEngine::KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE and action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	m_Camera.ProcessKeyboard(key, action);
}
