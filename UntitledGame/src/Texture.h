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
	//int numberOfTextures;

	Texture();  // default constructor
	Texture(std::string textureFileName_);  // parameterized constructor
	Texture(int numberOfTextures_);		// new
	void setUpTexture();

	//void init(GLuint (&textureIDs)[10]);			// new

private:
	//int imageWidth;
	//int imageHeight;
	//int nrChannels;

	//void setUpTexture();
	void loadTexture();
	//void createTextures();	// new
};

