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
#include "Instrumentor.h"


class LoadAssets
{
private:
	TextureContainer diffuseMaps[9];
	TextureContainer specularMaps[3];
	TextureContainer normalMaps[2];
public:
	//ObjectContainer models[NUMBER_OF_OBJECTS];
	std::vector<ObjectContainer> models_;

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