#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Data.h"
#include "PointLight.h"
#include "Camera.h"
#include "Globals.h"

class Scene2
{
private:
	struct Uniforms {
		int MVPloc;
		int modelMatrixLoc;
		int lightSpaceMatrixLoc;

		int cameraPosLoc;

		int diffuseMapLoc;
		int specularMapLoc;
		int shininessLoc;

		int shadowMapLoc;
	};
	Uniforms uniforms;

	// member vars to draw object
	Data& object;						// data (vertices, uvs, normals, indices), vao, vbo, ibo
	glm::mat4 modelMatrix;				// translation, rotation, scaling
	glm::mat4 MVP = glm::mat4(0.0f);	// calculated
	GLuint shaderID;					// standard shader

	// member vars for lighting
	PointLight& pointLight;

	// member vars for shadow
	//glm::mat4 lightSpaceMatrix;

	// textures/maps
	GLuint diffuseMap;
	GLuint specularMap;
	GLuint shadowMap;

	// camera
	Camera& camera;

public:
	Scene2(Data& object_, glm::mat4 modelMatrix_, GLuint shaderID_, Camera& camera_,
		GLuint diffuseMap_, GLuint specularMap_, GLuint shadowMap_, PointLight pointLight_);

	void initialize();
private:
	void configVertexAttributes();
	void getUniformLocations();

public:
	void render(Camera &camera_);
private:
	void updateMVP();
	void uploadUniforms();
};

