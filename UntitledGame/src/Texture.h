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
	GLuint textureID;

	Texture();  // default constructor
	Texture(std::string textureFileName_);  // parameterized constructor
	void setUpTexture();

private:
	//int imageWidth;
	//int imageHeight;
	//int nrChannels;

	//void setUpTexture();
	void loadTexture();

};

