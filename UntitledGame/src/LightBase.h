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

class LightBase
{
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


	void getObjectUniformLocations(GLuint objectShader);	// for Scene, needs standard shader
	void uploadObjectUniforms();							// for Scene

	void initialize();	// gets light object data by reference, needs light shader

	void configVertexAttributes();		// for light viz.
	void getLightUniformLocations();	// for light viz., uses light shader


	void render(Camera& camera_);

	void updateMVP();					// for light viz.
	void uploadLightUniforms();			// fot light viz.
	glm::mat4 createModelMatrix(glm::vec3 translate, glm::vec3 scale, float rotateAngle, std::string rotateAxis);

	glm::mat4 calculateLightSpaceMatrix(glm::mat4 actModelMatrix);
	void changeParams();

};

