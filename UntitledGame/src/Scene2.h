#pragma once

#include <glm/glm.hpp>

#include "Data.h"
#include "PointLight.h"
#include "Camera.h"

class Scene2
{
	struct Uniforms {
		int MVPloc;
		int modelMatrixLoc;
		int lightSpaceMatrixLoc;

		int cameraPosLoc;

		int textureLoc;
		int shadowMapLoc;
		int specularMapLoc;
	};
	Uniforms uniforms;

	// member vars to draw object
	Data& object;				// data (vertices, uvs, normals, indices), vao, vbo, ibo
	glm::mat4 modelMatrix;		// translation, rotation, scaling
	glm::mat4 MVP;				// calculated
	GLuint shaderID;			// standard shader

	// member vars for lighting
	PointLight& pointlight;

	// member vars for shadow
	glm::mat4 lightSpaceMatrix;

	// textures/maps
	GLuint texture;
	GLuint shadowMap;
	GLuint diffuseMap;

	// camera
	Camera& camera;






};

