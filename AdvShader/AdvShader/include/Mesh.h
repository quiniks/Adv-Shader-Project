#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "ShaderProgram.h"

struct Vertex {
	glm::vec3 m_Position;
	glm::vec3 m_Normal;
	glm::vec2 m_TexCoords;
	glm::vec3 m_Tangent;
	glm::vec3 m_Bitangent;
};

struct Texture {
	unsigned int m_ID;
	std::string m_Type;
	std::string m_Path;
};

class Mesh {
public:

	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	std::vector<Texture> m_Textures;

	Mesh(std::vector<Vertex> p_Vertices, std::vector<unsigned int> p_Indices, std::vector<Texture> p_Textures);
	void Draw(ShaderProgram p_Shader);
	void SetUpMesh();
private:
	unsigned int  m_VAO, m_VBO, m_EBO;
};