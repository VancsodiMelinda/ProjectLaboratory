#pragma once

#include <glad\glad.h>

#include <string>

enum struct TextureType
{
	defaultMap,
	diffuseMap,		// object color
	specularMap,	// object specularity
	projectiveMap,	// projective texture mapping
	//shadowMap,		// directional light shadow
	//shadowBox,		// pointlight shadow
	//skybox			// environment mapping
};

struct TextureContainer
{
	std::string name = "";
	TextureType type = TextureType::defaultMap;
	GLuint ID = 0;
};

