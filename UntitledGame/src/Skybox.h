#pragma once

#include <glad/glad.h>
#include "stb_image.h"
#include <string>
#include <iostream>
#include <vector>
#include "Data.h"
#include "Shader.h"
#include "Camera.h"
#include "Globals.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Skybox
{
public:
	Skybox(Data& box_);
	void render(Camera camera);

private:
	struct Uniforms {
		GLint MVPloc	= 0;
		GLint skyboxLoc	= 0;
	};
	Uniforms uniforms;										// ok

	Data& box;												// ok
	std::string textureFolder = "";							// ok
	std::vector<std::string> textureFileNames = { "" };		// ok
	GLuint shaderID = 0;									// ok
	GLuint textureID = 0;
	glm::mat4 MVP = glm::mat4(1.0f);

	void setUpShaders();				// ok
	void configVertexAttributes();		// ok
	void getUniformLocations();			// ok
	void initializeTexture();
	void loadTextures();

	void updateMVP(Camera camera);
	void uploadUniforms();
};

