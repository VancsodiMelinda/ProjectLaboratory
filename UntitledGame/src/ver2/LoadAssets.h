#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <map>

#include "CreateModel.h"
#include "CreateTexture.h"


class LoadAssets
{
private:
	TextureContainer diffuseMaps[3];
	TextureContainer specularMaps[2];
public:
	ObjectContainer models[1];

	LoadAssets();	// ctor
private:
	void loadDiffuseMaps();
	void loadSpecularMaps();
	void loadObjects();

public:
	void config(ObjectContainer& object, GLuint programID);
	void render(ObjectContainer& object, GLuint programID);

private:
	struct DefaultUniformLocs {
		int modelMatrixLoc;
		int diffuseMapLoc;
		int specularMapLoc;
		int shininessLoc;
	};
	DefaultUniformLocs defaultUniformLocs;
};

/*
std::vector<std::string> objectsToLoad = {
	"resources/NewSuzanne.obj",
	"resources/RubiksCube.obj",
	"resources/Velociraptor.obj",
	"resources/objects/sphere.obj"
};
*/