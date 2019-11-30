#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

#include <string>

#include "ObjLoader.h"

class Test
{
private:
	std::string objectFileName;

	// vars to create model matrix
	glm::vec3 translate;
	glm::vec3 scale;
	float rotateAngle;
	std::string rotateAxis;

	void loadObjectData();		// data
	void createModelMatrix();	// modelMatrix
	void createVAOandVBOs();	// vao, vbo, ibo
	void fillVBOs();

public:
	objectData data;		//good
	glm::mat4 modelMatrix;	//good, not needed here
	GLuint vao;				//good
	GLuint vbo;				//good
	GLuint ibo;				//good

	Test(std::string objectFileName_, glm::vec3 translate_, glm::vec3 scale_, float rotateAngle_, std::string rotateAxis_);
	Test(std::string objectFileName_);
	void initialize();

};

