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
#include "Globals.h"
#include "Data.h"
#include "LightBase.h"

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
		int MVPloc = 0;
	};
	uniforms uniLocs;

	GLuint shaderID = 0;
	//GLuint vao;
	//GLuint vbo;
	//GLuint ibo;
	//objectData data;
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	//glm::vec3 lightPos = glm::vec3(1.0f);
	LightBase& light;

	struct ShadowQuad {
		GLuint quadShaderID	= 0;
		int shadowMapLoc	= 0;
		GLuint shadowVAO	= 0;
		int indicesSize		= 0;
		int shadowMPVLoc	= 0;
	};
	ShadowQuad shadowQuad;

	Data& object;

public:
	glm::mat4 MVP = glm::mat4(1.0f);
	GLuint shadowMap = 0;
	GLuint fbo = 0;
	
	// constructor(s)
	Shadow(Data& object_, LightBase& light_);
	//Shadow(GLuint shaderID_, GLuint vao_, GLuint vbo, GLuint ibo, objectData data_, glm::mat4 modelMatrix_,
		//glm::vec3 lightPos_, int shadowWidth, int shadowHeight);
	//Shadow(GLuint shaderID_, GLuint vao_, GLuint vbo, GLuint ibo, objectData data_, glm::mat4 modelMatrix_,
		//glm::vec3 lightPos_);
	//Shadow(GLuint shaderID_, Data& object_, glm::mat4 modelMatrix_, glm::vec3 lightPos_);
	Shadow(GLuint shaderID_, Data& object_, glm::mat4 modelMatrix_, LightBase& light_);

// create texture and fbo for shadow map, run only once
private:	void createTexture();
			void createFBO();

// initialize shadow for each object
public:		void initialize();
private:	void createMVP();
			void configVertexAttributes();
			void getUniformLocations();

// render object into shadow map for each object
public:		void render();
private:	void uploadUniforms();

// render shadow map to a quad on screen
public:		void initRenderShadowMap(GLuint quadShaderID);
			void renderShadowMap();

	//void init();

private:
	void updateMVP();
};

