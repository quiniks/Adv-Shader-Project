#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
private:
	glm::vec3 m_Position;
	float m_Yaw;
	float m_Pitch;
	float m_PitchLimit = 90.f;
	double m_Sensitivity = 0.1;
	double m_PrevX = 0.0;
	double m_PrevY = 0.0;
	bool m_FirstMouse = true;
	float m_MaxSpeed = 2.5f;

	glm::vec3 m_FrontDir = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 m_RightDir;
	glm::vec3 m_UpDir = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 m_WorldUp = glm::vec3(0.f, 1.f, 0.f);

	bool m_Forward;
	bool m_Backward;
	bool m_Left;
	bool m_Right;
	bool m_Up;
	bool m_Down;

	void UpdateVectors() {
		glm::vec3 front;
		front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		front.y = sin(glm::radians(m_Pitch));
		front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_FrontDir = glm::normalize(front);
		m_RightDir = glm::normalize(glm::cross(m_FrontDir, m_WorldUp));
		m_UpDir = glm::normalize(glm::cross(m_RightDir, m_FrontDir));
	}
public:
	Camera(glm::vec3 p_Position = glm::vec3(0.0f, 0.0f, 0.0f)) {
		m_Position = p_Position;
		UpdateVectors();
	}

	glm::mat4 GetViewMatrix() {
		return glm::lookAt(m_Position, m_Position + m_FrontDir, m_UpDir);
	}

	glm::vec3 GetPosition() {
		return m_Position;
	}

	void Update(float p_DeltaTime) {
		float l_Velocity = m_MaxSpeed * p_DeltaTime;
		if (m_Forward) {
			m_Position += m_FrontDir * l_Velocity;
		}
		if (m_Backward) {
			m_Position -= m_FrontDir * l_Velocity;
		}
		if (m_Right) {
			m_Position += m_RightDir * l_Velocity;
		}
		if (m_Left) {
			m_Position -= m_RightDir * l_Velocity;
		}
		if (m_Up) {
			m_Position += m_UpDir * l_Velocity;
		}
		if (m_Down) {
			m_Position -= m_UpDir * l_Velocity;
		}
	}

	void ProcessKeyboard(int p_Key, int p_Action) {
		if (p_Action == GLFW_PRESS) {
			switch (p_Key) {
			case GLFW_KEY_W :
				m_Forward = true;
				break;
			case GLFW_KEY_A:
				m_Left = true;
				break;
			case GLFW_KEY_S:
				m_Backward = true;
				break;
			case GLFW_KEY_D:
				m_Right = true;
				break;
			case GLFW_KEY_SPACE:
				m_Up = true;
				break;
			case GLFW_KEY_LEFT_SHIFT:
				m_Down = true;
				break;
			default:
				break;
			}
		}
		else if (p_Action == GLFW_RELEASE) {
			switch (p_Key) {
			case GLFW_KEY_W:
				m_Forward = false;
				break;
			case GLFW_KEY_A:
				m_Left = false;
				break;
			case GLFW_KEY_S:
				m_Backward = false;
				break;
			case GLFW_KEY_D:
				m_Right = false;
				break;
			case GLFW_KEY_SPACE:
				m_Up = false;
				break;
			case GLFW_KEY_LEFT_SHIFT:
				m_Down = false;
				break;
			default:
				break;
			}
		}
	}

	void ProcessMouse(double p_X, double p_Y) {
		if (m_FirstMouse) {
			m_PrevX = p_X;
			m_PrevY = p_Y;
			m_FirstMouse = false;
		}

		double l_OffsetX = (p_X - m_PrevX) * m_Sensitivity;
		double l_OffsetY = (m_PrevY - p_Y) * m_Sensitivity;
		m_Yaw += l_OffsetX;
		m_Pitch += l_OffsetY;
		if (m_Pitch >= m_PitchLimit)
			m_Pitch = 89.0f;
		if (m_Pitch <= -m_PitchLimit)
			m_Pitch = -89.0f;

		UpdateVectors();

		m_PrevX = p_X;
		m_PrevY = p_Y;
	}

	
};