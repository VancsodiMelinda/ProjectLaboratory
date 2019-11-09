#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

#include <string>
#include <stdio.h>
#include <iostream>

#include "ObjLoader.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"

class Object
{
private:
	GLuint vao;
	GLuint vbo, ibo;
	objectData data;
	GLuint programObject;
	Shader shader;
	Texture texture;
	glm::mat4 MVP;
	glm::mat4 modelMatrix;  // for the normals
	Camera camera;

public:
	std::string objectFileName;

	int WINDOW_WIDTH;
	int WINDOW_HEIGHT;

	Object();  // default constructor
	Object(std::string objectFileName_);  // parameterized constructor

private:
	void loadObjectData();
	void createVAOandVBOs();
	void fillVBOs();
	void configVertexAttributes();
	void createMVP();
	void uploadUniforms();
	void updateMVP();

public:
	void initialize();
	void render();
};

