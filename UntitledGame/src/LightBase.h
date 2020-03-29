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
public:  // derived classes can see member vars
	struct LightUniforms {
		int MVPloc		= 0;
		int colorLoc	= 0;
	};
	LightUniforms lightUniforms;  // for light shader

	// variables for visualizing the light source
	Data& object;
	GLuint shaderID				= 0;
	glm::mat4 modelMatrix		= glm::mat4(0.0f);
	glm::mat4 MVP				= glm::mat4(0.0f);
	Camera& camera;

public:		LightBase(Data& object_, GLuint shaderID_, Camera& camera_);
			LightBase(Data& object_, Camera& camera_);

public:		virtual glm::mat4 calculateLightSpaceMatrix(glm::mat4 actModelMatrix);

public:		void initialize();	// gets light object data by reference, needs light shader
private:	void configVertexAttributes();		// for light viz.
			void getLightUniformLocations();	// for light viz., uses light shader

public:		void render(Camera& camera_);
private:	virtual void updateMVP();					// for light viz.
			virtual void uploadLightUniforms();			// fot light viz.
public:		glm::mat4 createModelMatrix(glm::vec3 translate, glm::vec3 scale, float rotateAngle, std::string rotateAxis);

public:		virtual void getObjectUniformLocations(GLuint objectShader);	// for Scene, needs standard shader
			virtual void uploadObjectUniforms();							// for Scene
			virtual void changeParams();
};

