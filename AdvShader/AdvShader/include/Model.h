#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb_image/stb_image.h> 
#include <iostream>
#include "Mesh.h"


class Model {
public:
	/*  Functions   */
	Model(std::string const &path);
	void Draw(ShaderProgram shader);
	void SetPosition(glm::vec3 pos);
private:
	/*  Model Data  */
	std::vector<Texture> loadedTextures;
	std::vector<Mesh> meshes;
	std::string directory;
	/*  Functions   */
	void loadModel(std::string const &path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
	unsigned int TextureFromFile(const char *path, const std::string &directory);
};