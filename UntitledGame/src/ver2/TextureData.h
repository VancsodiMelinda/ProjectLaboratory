#pragma once

#include <glad\glad.h>

#include <string>

enum struct TextureType
{
	defaultMap,
	diffuseMap,		// object color
	specularMap,	// object specularity
	projectiveMap,	// projective texture mapping
	normalMap		// per fragment normals
};

struct TextureContainer
{
	std::string name = "";
	TextureType type = TextureType::defaultMap;
	GLuint ID = 0;
};

