#include "CreateTexture.h"

CreateTexture::CreateTexture(TextureType textureType)
{
	if (textureType == TextureType::diffuseMap)
	{
		textureContainer.name = "resources/diffuse maps/default diffuse map.png";
		textureContainer.type = TextureType::diffuseMap;
		setUpTexture();
		loadTexture(textureContainer.name);
	}
	else if (textureType == TextureType::specularMap)
	{
		textureContainer.name = "resources/specular maps/default specular map.png";
		textureContainer.type = TextureType::specularMap;
		setUpTexture();
		loadTexture(textureContainer.name);
	}
	else if (textureType == TextureType::normalMap)
	{
		textureContainer.name = "resources/normal maps/default normal map.jpg";
		textureContainer.type = TextureType::normalMap;
		setUpTexture();
		loadTexture(textureContainer.name);
	}
	
}

CreateTexture::CreateTexture(std::string textureFileName, TextureType textureType)
{
	if (textureType == TextureType::diffuseMap)
	{
		textureContainer.name = textureFileName;
		textureContainer.type = TextureType::diffuseMap;
		setUpTexture();
		loadTexture(textureFileName);
	}
	else if (textureType == TextureType::specularMap)
	{
		textureContainer.name = textureFileName;
		textureContainer.type = TextureType::specularMap;
		setUpTexture();
		loadTexture(textureFileName);
	}
	else if (textureType == TextureType::normalMap)
	{
		textureContainer.name = textureFileName;
		textureContainer.type = TextureType::normalMap;
		setUpTexture();
		loadTexture(textureFileName);
	}
}

void CreateTexture::setUpTexture()
{
	glGenTextures(1, &textureContainer.ID);				// create texture
	glBindTexture(GL_TEXTURE_2D, textureContainer.ID);  // bind texture

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  // wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  // wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  // filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  // filtering
}

void CreateTexture::loadTexture(std::string textureFileName)
{
	int imageWidth, imageHeight, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(textureFileName.c_str(), &imageWidth, &imageHeight, &nrChannels, 0);

	if (data)
	{
		std::cout << "OK: " << textureFileName << std::endl;
		//std::cout << "imageWidth: " << imageWidth << std::endl << "imageHeight: " << imageHeight << std::endl << "nrChannels: " << nrChannels << std::endl;
		GLenum format = GL_RGB;
		if (nrChannels == 3)
		{
			format = GL_RGB;
			//std::cout << "format: GL_RGB" << std::endl;
		}
		else if (nrChannels == 4)
		{
			format = GL_RGBA;
			//std::cout << "format: GL_RGBA" << std::endl;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR: " << textureFileName << std::endl;
	}

	stbi_image_free(data);  // free the image memory
	glBindTexture(GL_TEXTURE_2D, 0);  // unbind texture?
}
