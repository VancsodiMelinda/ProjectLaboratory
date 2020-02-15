#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

#include "ObjLoader.h"
#include "Texture.h"
#include "Camera.h"

class Scene
{
private:
	struct uniforms {
		int MVPloc;
		int modelMatrixLoc;
		int lightSpaceMatrixLoc;
		
		int lightColorLoc;
		int lightPosLoc;
		int cameraPosLoc;
		int texLoc;
		int shadowMapLoc;

		int ambientLoc;
		int diffuseLoc;
		int specularLoc;
		int shininessLoc;

		int ambientStrengthLoc;
		int diffuseStrengthLoc;
		int specularStrengthLoc;
	};
	uniforms uniLocs;

	GLuint shaderID;
	GLuint vao;
	GLuint vbo;
	GLuint ibo;
	objectData data;
	glm::mat4 modelMatrix;
	glm::vec3 lightColor;
	glm::vec3 lightPos;
	glm::mat4 lightSpaceMatrix;
	Camera camera;
	int WINDOW_WIDTH;
	int WINDOW_HEIGHT;
	GLuint textureID;
	GLuint shadowMap;

	glm::mat4 MVP;  // calculated

	void configVertexAttributes();
	void getUniformLocations();

	void uploadUniforms();
	void updateMVP();

public:
	Scene(GLuint shaderID_, GLuint vao_, GLuint vbo_, GLuint ibo_, objectData data_, glm::mat4 modelMatrix_,
		glm::vec3 lightColor_, glm::vec3 lightPos_, glm::mat4 lightSpaceMatrix_, Camera camera_, int windowWidth, int windowHeight, GLuint textureID_, GLuint shadowMap_);
	void initialize();
	void render(Camera camera_);

};

