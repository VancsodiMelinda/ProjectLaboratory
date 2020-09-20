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
#include "ConstantVariables.h"


class LoadAssets
{
private:
	TextureContainer diffuseMaps[4];
	TextureContainer specularMaps[2];
	TextureContainer normalMaps[1];
public:
	ObjectContainer models[NUMBER_OF_OBJECTS];

	LoadAssets();	// ctor
private:
	void loadDiffuseMaps();
	void loadSpecularMaps();
	void loadNormalMaps();
	void loadObjects();
};

/*
std::vector<std::string> objectsToLoad = {
	"resources/NewSuzanne.obj",
	"resources/RubiksCube.obj",
	"resources/Velociraptor.obj",
	"resources/objects/sphere.obj"
};
*/