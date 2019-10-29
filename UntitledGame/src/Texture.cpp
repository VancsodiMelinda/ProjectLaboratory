#include "Texture.h"

// default constructor

Texture::Texture()
{
	textureFileName = "resources/DefaultTexture.png";
	glGenTextures(1, &textureID);
}


// parameterized constructor
Texture::Texture(std::string textureFileName_)
{
	textureFileName = textureFileName_;
	//textureID = textureID_;
}

void Texture::setUpTexture()
{
	//GLuint textureID;
	glGenTextures(1, &textureID);
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);  // bind texture
	std::cout << "in class textureID: " << textureID << std::endl;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  // wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  // wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  // filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  // filtering

	loadTexture();
}

void Texture::loadTexture()
{
	int imageWidth, imageHeight, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(textureFileName.c_str(), &imageWidth, &imageHeight, &nrChannels, 0);

	if (data)
	{
		std::cout << "Texture image has been loaded succesfully: " << textureFileName << std::endl;
		std::cout << "imageWidth: " << imageWidth << std::endl << "imageHeight: " << imageHeight << std::endl << "nrChannels: " << nrChannels << std::endl;
		GLenum format = GL_RGB;
		if (nrChannels == 3)
		{
			format = GL_RGB;
			std::cout << "format: GL_RGB" << std::endl;
		}
		else if (nrChannels == 4)
		{
			format = GL_RGBA;
			std::cout << "format: GL_RGBA" << std::endl;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Texture image loading has failed!" << std::endl;
	}

	stbi_image_free(data);  // free the image memory
	glBindTexture(GL_TEXTURE_2D, 0);  // unbind texture?
}
