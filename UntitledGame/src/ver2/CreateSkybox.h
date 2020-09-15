#pragma once

#include <iostream>
#include <vector>

#include <glad/glad.h>

#include "../stb_image.h"


struct SkyboxContainer
{
	GLuint ID = 0;
};

class CreateSkybox
{
public:
	SkyboxContainer skyboxContainer;
	CreateSkybox();
	CreateSkybox(std::string skyboxFolderName, std::string format);

private:
	GLuint createTexture();
	void loadTextures(std::string skyboxFolder, std::vector<std::string> textureFileNames);
};

