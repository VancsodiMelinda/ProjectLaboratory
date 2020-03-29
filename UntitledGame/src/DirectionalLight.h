#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

#include "imgui/imgui.h"

#include "Data.h"
#include "Camera.h"
#include "Globals.h"
#include "LightBase.h"

struct DirLightParams
{
	glm::vec3 position		= glm::vec3(0.0f);
	glm::vec3 color			= glm::vec3(0.0f);

	float ambientStrength	= 0.0f;
	float diffuseStrength	= 0.0f;
	float specularStrength	= 0.0f;

	glm::vec3 scale			= glm::vec3(0.0f);
	float angle				= 0.0f;
	std::string axes		= "x";
};

class DirectionalLight : public LightBase
{
public:
	DirLightParams params;

private:
	struct ObjectUniforms {
		int positionLoc				= 0;
		int colorLoc				= 0;

		int ambientStrengthLoc		= 0;
		int diffuseStrengthLoc		= 0;
		int specularStrengthLoc		= 0;
	};
	ObjectUniforms objectUniforms;  // for standard shader

	//struct LightUniforms {
		//int MVPloc;
		//int colorLoc;
	//};
	//LightUniforms lightUniforms;  // for light shader

	// variables for visualizing the light source
	//Data& object;
	//GLuint shaderID;
	//glm::mat4 modelMatrix;
	//glm::mat4 MVP;
	//Camera& camera;

public:
	DirectionalLight(DirLightParams params_, Data& object_, GLuint shaderID_, Camera& camera_);

	glm::mat4 calculateLightSpaceMatrix(glm::mat4 actModelMatrix) override;

	void updateMVP() override;

	void uploadLightUniforms() override;

	void getObjectUniformLocations(GLuint objectShader) override;

	void uploadObjectUniforms() override;

	void changeParams() override;

	/*
	void getObjectUniformLocations(GLuint objectShader);	// for Scene, needs standard shader
	void uploadObjectUniforms();							// for Scene

	void initialize();

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
	*/
};

