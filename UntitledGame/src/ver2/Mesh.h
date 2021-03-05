#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <string>
#include <vector>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};

struct ModelTexture {
	GLuint id;
	std::string type;
};


class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<ModelTexture> textures;

	Mesh(std::vector<Vertex> vertices_, std::vector<unsigned int> indices_, std::vector<ModelTexture> textures_);

	void Draw(GLuint shaderID);

private:
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	void setupMesh();
};

