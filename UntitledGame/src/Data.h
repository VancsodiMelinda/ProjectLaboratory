#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

#include <string>

#include "ObjLoader.h"

class Data
{
private:
	std::string objectFileName = "";

	void loadObjectData();		// data
	void createVAOandVBOs();	// vao, vbo, ibo
	void fillVBOs();			// vao, vbo, ibo

public:
	objectData data;
	GLuint vao = 0;
	GLuint vbo = 0;
	GLuint ibo = 0;

	Data();
	Data(std::string objectFileName_);  // constructor
	void initialize();

};

