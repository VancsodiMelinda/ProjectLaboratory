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

struct uniforms {
	int modelMatrixLoc;
	int MVPloc;
	int lightColorLoc;
	int lightPosLoc;
	int cameraPosLoc;
};

class Object
{
private:
	GLuint vao;
	GLuint vbo, ibo;
	objectData data;

	std::string objectFileName;	// done
	GLuint shaderID;			// done
	glm::mat4 modelMatrix;		// done
	Camera camera;				// done
	GLuint textureID;			// done
	glm::vec3 lightColor;		// done
	glm::vec3 lightPos;			// done

	uniforms uniLocs;
	glm::mat4 MVP;

	int WINDOW_WIDTH;
	int WINDOW_HEIGHT;

public:
	//Object();  // default constructor
	Object(std::string objectFileName_, GLuint shaderID_, glm::vec3 translate_, glm::vec3 scale_, float rotateAngle_, std::string rotateAxis_,
		Camera camera_, GLuint textureID_, int windowWidth, int windowHeight, glm::vec3 lightColor_, glm::vec3 lightPos_);  // parameterized constructor

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
	void render(Camera camera);
};

