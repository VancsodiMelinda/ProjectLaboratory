#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

#include "imgui/imgui.h"

#include "ObjLoader.h"
#include "Data.h"
#include "Camera.h"
#include "Globals.h"
#include "LightBase.h"

struct PointLightParams
{
	glm::vec3 position		= glm::vec3(0.0f);
	glm::vec3 color			= glm::vec3(0.0f);

	float ambientStrength	= 0.0f;
	float diffuseStrength	= 0.0f;
	float specularStrength	= 0.0f;

	float constant			= 0.0f;
	float linear			= 0.0f;
	float quadratic			= 0.0f;

	glm::vec3 scale			= glm::vec3(0.0f);
	float angle				= 0.0f;
	std::string axes		= "x";
};

class PointLight
{
private:
	PointLightParams params;	// PARAMS STRUCT

private:
	struct ObjectUniforms {
		int positionLoc;
		int colorLoc;

		int ambientStrengthLoc;
		int diffuseStrengthLoc;
		int specularStrengthLoc;

		int constantLoc;
		int linearLoc;
		int quadraticLoc;
	};
	ObjectUniforms objectUniforms;  // for standard shader

	struct LightUniforms {
		int MVPloc;
		int colorLoc;
	};
	LightUniforms lightUniforms;  // for light shader

	// variables for visualizing the light source
	Data& object;
	GLuint shaderID;
	glm::mat4 modelMatrix;
	glm::mat4 MVP;
	Camera& camera;

	int WINDOW_WIDTH = 1280;
	int WINDOW_HEIGHT = 720;

public:
	//PointLight(PointLightParams params_ ,Data& object_, GLuint shaderID_, glm::mat4 modelMatrix_, Camera& camera_);
	PointLight(PointLightParams params_, Data& object_, GLuint shaderID_, Camera& camera_);

	void getObjectUniformLocations(GLuint objectShader);	// for Scene, needs standard shader
	void uploadObjectUniforms();							// for Scene

public:
	void initialize();	// gets light object data by reference, needs light shader
private:
	void configVertexAttributes();		// for light viz.
	void getLightUniformLocations();	// for light viz., uses light shader

public:
	void render(Camera& camera_);
private:
	void updateMVP();					// for light viz.
	void uploadLightUniforms();			// fot light viz.
	glm::mat4 createModelMatrix(glm::vec3 translate, glm::vec3 scale, float rotateAngle, std::string rotateAxis);

public:
	glm::mat4 calculateLightSpaceMatrix(glm::mat4 actModelMatrix);
	void changeParams();
};

