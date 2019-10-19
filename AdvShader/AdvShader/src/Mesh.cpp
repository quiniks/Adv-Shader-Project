#include "..\include\Mesh.h"

Mesh::Mesh(std::vector<Vertex> p_Vertices, std::vector<unsigned int> p_Indices, std::vector<Texture> p_Textures)
{
	m_Vertices = p_Vertices;
	m_Indices = p_Indices;
	m_Textures = p_Textures;
	SetUpMesh();
}

void Mesh::Draw(ShaderProgram p_Shader)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;

	for (unsigned int i = 0; i < m_Textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		std::string l_Number;
		std::string l_Name = m_Textures[i].m_Type;
		if (l_Name == "diffuse")
			l_Number = std::to_string(diffuseNr++);
		else if (l_Name == "specular")
			l_Number = std::to_string(specularNr++);
		else if (l_Name == "normal")
			l_Number = std::to_string(normalNr++);
		else if (l_Name == "height")
			l_Number = std::to_string(heightNr++);

		p_Shader.SetInt(("material" + l_Number + "." + l_Name).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, m_Textures[i].m_ID);
	}
	glActiveTexture(GL_TEXTURE0);

	// draw mesh
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::SetUpMesh()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_TexCoords));
	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Tangent));
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Bitangent));

	glBindVertexArray(0);
}