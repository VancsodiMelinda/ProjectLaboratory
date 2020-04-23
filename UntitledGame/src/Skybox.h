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
	Uniforms uniforms;

	Data& box;
	std::string textureFolder = "";
	std::vector<std::string> textureFileNames = { "" };
	GLuint shaderID = 0;
	GLuint textureID = 0;
	glm::mat4 MVP = glm::mat4(1.0f);

	void setUpShaders();
	void configVertexAttributes();
	void getUniformLocations();
	void initializeTexture();
	void loadTextures();

	void updateMVP(Camera camera);
	void uploadUniforms();
};

