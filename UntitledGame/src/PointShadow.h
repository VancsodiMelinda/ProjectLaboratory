#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>
#include <vector>

#include "Globals.h"
#include "Data.h"
#include "LightBase.h"
#include "PointLight.h"

class PointShadow
{
private:
	struct Uniforms {
		int modelMatrixLoc	= 0;
		int VPsLoc			= 0;
		int lightPosLoc		= 0;
		int farPlaneLoc		= 0;
	};
	Uniforms uniLocs;

public:
	PointShadow(Data& object_, PointLight& light_);
	PointShadow(GLuint shaderID_, Data& object_, glm::mat4 modelMatrix_, PointLight& light_);
	void initialize();

private:
	void createTexture();
	void createFBO();

	void configVertexAttributes();
	void getUniformLocations();
	void lightSpaceTransform();

public:
	void render();
private:
	void uploadUniforms();

public:
	GLuint textureID	= 0;
	GLuint fbo			= 0;

private:
	GLuint shaderID = 0;
	Data& object;
	glm::mat4 modelMatrix = glm::mat4(1.0);
	PointLight& light;

	std::vector<glm::mat4> VPs = {};
	float farPlane = 0.0f;
};

