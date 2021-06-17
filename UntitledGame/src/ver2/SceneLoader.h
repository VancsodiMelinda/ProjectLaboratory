#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <vector>
#include "Mesh.h"
#include "../stb_image.h"
#include "ModelData.h"
#include "CreateTexture.h"
#include "Instrumentor.h"

class SceneLoader
{
public:
	SceneLoader(char* path);
	void draw(GLuint programID);
	std::vector<ObjectContainer> models;

	GLuint defaultDiffuseMapID = 0;
	GLuint defaultSpecularMapID = 0;

private:
	std::string directory;
	std::vector<Mesh> meshes;

	void loadScene(std::string filePath);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<ModelTexture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
	GLuint TextureFromFile(const char *path, std::string dir);
	void prepareVAOandVBOs(ObjectContainer& objectContainer);
};

