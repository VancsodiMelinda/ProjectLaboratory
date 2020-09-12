#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <map>

#include "CreateModel.h"
#include "CreateTexture.h"
#include "GlobalVariables.h"
#include "Kamera.h"


class LoadAssets
{
private:
	TextureContainer diffuseMaps[3];
	TextureContainer specularMaps[2];
public:
	ObjectContainer models[4];

	LoadAssets();	// ctor
private:
	void loadDiffuseMaps();
	void loadSpecularMaps();
	void loadObjects();

	void configAsset(ObjectContainer& object, GLuint programID);
	void renderAsset(ObjectContainer& object, GLuint programID, Kamera& kamera);
	glm::mat4 updateMVP(glm::mat4 M, glm::mat4 V, glm::mat4 P);

	struct DefaultUniformLocs {
		int MVPloc;
		int modelMatrixLoc;
		int diffuseMapLoc;
		int specularMapLoc;
		int shininessLoc;
	};
	DefaultUniformLocs defaultUniformLocs;

public:
	void config(GLuint programID);
	void render(GLuint programID, Kamera& kamera);
};

/*
std::vector<std::string> objectsToLoad = {
	"resources/NewSuzanne.obj",
	"resources/RubiksCube.obj",
	"resources/Velociraptor.obj",
	"resources/objects/sphere.obj"
};
*/