#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

#include "ObjLoader.h"
#include "Texture.h"
#include "Camera.h"

class Test2
{
private:

	struct uniforms {
		int modelMatrixLoc;
		int MVPloc;
		int lightColorLoc;
		int lightPosLoc;
		int cameraPosLoc;
	};
	uniforms uniLocs;

	GLuint shaderID;
	GLuint vao;
	GLuint vbo;
	GLuint ibo;
	objectData data;
	glm::mat4 modelMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 MVP;

	Camera camera;
	GLuint textureID;
	int WINDOW_WIDTH;
	int WINDOW_HEIGHT;
	glm::vec3 lightColor;
	glm::vec3 lightPos;

	void configVertexAttributes();
	void getUniformLocations();
	void uploadUniforms();
	void createMVP();

public:
	Test2(GLuint shaderID_, GLuint vao_, GLuint vbo_, GLuint ibo_, objectData data_,
		glm::mat4 modelMatrix_, glm::mat4 viewMatrix_, glm::mat4 projectionMatrix_);

	void initialize();
	void render();
};

