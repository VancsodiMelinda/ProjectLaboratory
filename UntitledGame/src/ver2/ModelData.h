#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>

struct ObjectData {
	std::vector<float> vertices = { 0.0f };
	std::vector<float> uvs = { 0.0f };
	std::vector<float> normals = { 0.0f };
	std::vector<int> indices = { 0 };
};

struct ObjectMaterial {
	GLuint diffuseMap = 0;
	GLuint specularMap = 0;
	GLuint normalMap = 0;
	float shininess = 0.0f;
};

struct ObjectContainer
{
	std::string name = "";
	ObjectData data;
	GLuint vao = 0;		// acts as ID of object
	GLuint vbo = 0;
	GLuint ibo = 0;
	glm::mat4 modelMatrix = glm::mat4();	// identity matrix
	ObjectMaterial material;
};
