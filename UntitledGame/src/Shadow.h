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

class Shadow
{
	struct uniforms {
		int MVPloc;
	};

private:
	//GLuint vao;
	GLuint vbo, ibo;
	//objectData data;

	std::string objectFileName;	// done
	GLuint shaderID;			// done
	glm::mat4 lightSpaceMatrix;
	//glm::mat4 modelMatrix;		// done
	//Camera camera;				// done
	//GLuint textureID;			// done
	//glm::vec3 lightColor;		// done
	glm::vec3 lightPos;			// done

	uniforms uniLocs;
	glm::mat4 MVP;

	int WINDOW_WIDTH;
	int WINDOW_HEIGHT;

public:
	glm::mat4 modelMatrix;		// make it public for shadow mapping
	GLuint vao;
	objectData data;

	//Object();  // default constructor
	Shadow(std::string objectFileName_, GLuint shaderID_, glm::vec3 translate_, glm::vec3 scale_, float rotateAngle_, std::string rotateAxis_,
			int windowWidth, int windowHeight, glm::vec3 lightPos_);  // parameterized constructor

private:
	void createModelMatrix(glm::vec3 translate, glm::vec3 scale, float rotateAngle, std::string rotateAxis);
	void loadObjectData();
	void createVAOandVBOs();
	void fillVBOs();
	void configVertexAttributes();
	void createMVP();
	void getUniformLocations();

	void updateMVP();		// every frame
	void uploadUniforms();	// every frame
	//void updateUniforms();

public:
	void initialize();
	void render();
};

