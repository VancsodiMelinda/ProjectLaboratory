#include "CreateSkybox.h"

CreateSkybox::CreateSkybox()
{
	std::string skyboxFolder = "resources/skyboxes/default/";
	std::vector<std::string> textureFileNames = { "right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg", "back.jpg" };

	skyboxContainer.ID = createTexture();
	loadTextures(skyboxFolder, textureFileNames);
}

CreateSkybox::CreateSkybox(std::string skyboxFolderName, std::string format)
{
	std::string skyboxFolder = "resources/skyboxes/" + skyboxFolderName + "/";
	std::vector<std::string> textureFileNames = { "right." + format, "left." + format, "top." + format, "bottom." + format, "front." + format, "back." + format };

	skyboxContainer.ID = createTexture();
	loadTextures(skyboxFolder, textureFileNames);
}

GLuint CreateSkybox::createTexture()
{
	GLuint skybox;

	glGenTextures(1, &skybox);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return skybox;
}

void CreateSkybox::loadTextures(std::string skyboxFolder, std::vector<std::string> textureFileNames)
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxContainer.ID);

	int imageWidth, imageHeight, nrChannels;
	stbi_set_flip_vertically_on_load(false);

	for (int i = 0; i < 6; i++)
	{
		std::string currentTextureName = skyboxFolder + textureFileNames[i];
		unsigned char* data = stbi_load(currentTextureName.c_str(), &imageWidth, &imageHeight, &nrChannels, 0);
		if (data)
		{
			std::cout << "OK: Skybox image has been loaded succesfully: " << currentTextureName << std::endl;
			
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
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, imageWidth, imageHeight, 0, format, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			std::cout << "ERROR: Skybox image loading has failed!" << std::endl;
		}
		stbi_image_free(data);  // free the image memory
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
