#pragma once

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include <string>

#include "GlobalVariables.h"
#include "SceneLoader.h"
#include "CreateProgram.h"
#include "DebugOpenGL.h"
#include "Kamera.h"

class DeferredRendering
{
private:
	Kamera& kamera;

	struct GeometryPassData {
		GLuint gBuffer;
		GLuint gPosition;
		GLuint gNormal;
		GLuint gAlbedoSpecular;
		GLuint programID;
		std::vector<ObjectContainer> models;
		glm::vec3 cameraPos;
	};
	GeometryPassData geomPassData;

	struct LightingPassData {
		int numberOfLights;
		std::vector<glm::vec3> lightPositions;
		std::vector<glm::vec3> lightColors;
		GLuint programID;
		ObjectContainer quad;
	};
	LightingPassData lightPassData;

	struct LightModelData {
		ObjectContainer model;
		GLuint programID;
	};
	LightModelData lightModelData;

public:
	DeferredRendering(Kamera& kamera_);
	void createFboAndAttachments();
	void initGeometryPass();
	void renderGeometryPass();

	void initLightingPass();
	void renderLightingPass();
	
	void initLightModel();
	void renderLightModel();

private:
	void createQuad();
};

