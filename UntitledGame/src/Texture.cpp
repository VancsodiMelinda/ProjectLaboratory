#include "Texture.h"

// default constructor
Texture::Texture()
{
	textureFileName = "resources/DefaultTexture.png";
}

// parameterized constructor
Texture::Texture(std::string textureFileName_)
{
	textureFileName = textureFileName_;
}

void Texture::setUpTexture()
{
	GLuint tex;
	glGenTextures(1, &tex);
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);  // bind texture

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  // wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  // wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  // filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  // filtering
}

void Texture::loadTexture()
{
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(textureFileName.c_str(), &imageWidth, &imageHeight, &nrChannels, 0);
	if (data)
	{
		std::cout << "Texture image has been loaded succesfully: " << textureFileName << std::endl;
		std::cout << "imageWidth: " << imageWidth << std::endl << "imageHeight: " << imageHeight << std::endl << "nrChannels: " << nrChannels << std::endl;
		GLenum format = GL_RGB;
		if ((nrChannels / 8) == 3)
		{
			format = GL_RGB;
		}
		else if ((nrChannels / 8) == 4)
		{
			format = GL_RGBA;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Texture image loading has failed!" << std::endl;
	}

	stbi_image_free(data);  // free the image memory
}
