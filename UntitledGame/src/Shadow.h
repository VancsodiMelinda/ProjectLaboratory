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
	/*
	struct uniforms {
		int MVPloc;
	};

	
private:
	GLuint vbo, ibo;

	std::string objectFileName;
	GLuint shaderID;
	glm::vec3 lightPos;

	uniforms uniLocs;
	

	int WINDOW_WIDTH;
	int WINDOW_HEIGHT;

public:
	glm::mat4 modelMatrix;		// make it public for shadow mapping
	GLuint vao;
	objectData data;
	glm::mat4 lightSpaceMatrix;
	glm::mat4 MVP;

	Shadow(std::string objectFileName_, GLuint shaderID_, glm::vec3 translate_, glm::vec3 scale_, float rotateAngle_, std::string rotateAxis_,
			int windowWidth, int windowHeight, glm::vec3 lightPos_);  // parameterized constructor
	Shadow(GLuint shaderID_, GLuint vao_, GLuint vbo_, GLuint ibo_, glm::mat4 modelMatrix_, int windowWidth, int windowHeight, glm::vec3 lightPos_);

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
	*/
	

	
private:
	struct uniforms {
		int MVPloc;
	};
	uniforms uniLocs;

	GLuint shaderID;
	GLuint vao;
	GLuint vbo;
	GLuint ibo;
	objectData data;
	glm::mat4 modelMatrix;
	glm::vec3 lightPos;

public:
	int SHADOW_WIDTH;
	int SHADOW_HEIGHT;
	glm::mat4 MVP;
	GLuint shadowMap;
	GLuint fbo;
	
	Shadow(GLuint shaderID_, GLuint vao_, GLuint vbo, GLuint ibo, objectData data_, glm::mat4 modelMatrix_,
		glm::vec3 lightPos_, int shadowWidth, int shadowHeight);
	Shadow();
	void initialize();
	void render();

private:
	void createMVP();
	void createTexture();
	void createFBO();
	void configVertexAttributes();
	void getUniformLocations();

	void uploadUniforms();
};

