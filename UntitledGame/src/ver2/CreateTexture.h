#pragma once

#include <glad/glad.h>
#include <string>
#include <stdio.h>
#include <iostream>

#include "../stb_image.h"
#include "TextureData.h"

class CreateTexture
{
public:
	TextureContainer textureContainer;
	CreateTexture(TextureType textureType);
	CreateTexture(std::string textureFileName, TextureType textureType);

private:
	void setUpTexture();
	void loadTexture(std::string textureFileName);
};

