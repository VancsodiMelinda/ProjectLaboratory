#pragma once

#include <glad/glad.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include "stb_image.h"

class Texture
{
public:
	std::string textureFileName;

	Texture();  // default constructor
	Texture(std::string textureFileName_);  // parameterized constructor

private:
	int imageWidth;
	int imageHeight;
	int nrChannels;

	void setUpTexture();
	void loadTexture();

};

