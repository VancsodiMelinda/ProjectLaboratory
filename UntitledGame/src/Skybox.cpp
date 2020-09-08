#include "Skybox.h"

Skybox::Skybox(Data& box_) : box(box_)
{
	textureFolder = "resources/skybox textures/blue clouds/";
	textureFileNames = { "right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "back.jpg", "front.jpg" };

	setUpShaders();				// shaderID
	configVertexAttributes();
	getUniformLocations();		// uniforms
	initializeTexture();		// textureID
	loadTextures();
}

void Skybox::setUpShaders()
{
	Shader shader("src/shaders/Skybox.vert", "src/shaders/Skybox.frag");
	shader.initialize();
	shaderID = shader.programObject;
	std::cout << "OK: SET UP SHADERS" << std::endl;
}

void Skybox::configVertexAttributes()
{
	glBindVertexArray(box.vao);
	glBindBuffer(GL_ARRAY_BUFFER, box.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, box.ibo);

	GLuint positionAttribIndex = glGetAttribLocation(shaderID, "vertexPosition");

	glEnableVertexAttribArray(positionAttribIndex);

	int stride3f = 3 * sizeof(GL_FLOAT);
	glVertexAttribPointer(positionAttribIndex, 3, GL_FLOAT, GL_FALSE, stride3f, 0);

	glBindVertexArray(0);

	std::cout << "OK: CONFIG VERTEX ATTRIBS" << std::endl;
}

void Skybox::getUniformLocations()
{
	uniforms.MVPloc = glGetUniformLocation(shaderID, "MVP");
	uniforms.skyboxLoc = glGetUniformLocation(shaderID, "skybox");

	std::cout << "OK: GET UNIFORMS LOCS" << std::endl;
}

void Skybox::initializeTexture()
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
	std::cout << "OK: INIT TEXTURE" << std::endl;
}


void Skybox::loadTextures()
{
	int imageWidth, imageHeight, nrChannels;
	stbi_set_flip_vertically_on_load(false);

	//unsigned char* data = stbi_load(textureFileName.c_str(), &imageWidth, &imageHeight, &nrChannels, 0);

	for (int i = 0; i < 6; i++)
	{
		std::string currentTextureName = textureFolder + textureFileNames[i];
		unsigned char* data = stbi_load(currentTextureName.c_str(), &imageWidth, &imageHeight, &nrChannels, 0);
		if (data)
		{
			//std::cout << "Skybox image has been loaded succesfully: " << currentTextureName << std::endl;
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
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, imageWidth, imageHeight, 0, format, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			std::cout << "Skybox image loading has failed!" << std::endl;
		}
		stbi_image_free(data);  // free the image memory
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	std::cout << "OK: LOAD TEXTURES" << std::endl;
}


void Skybox::render(Camera camera)
{
	//glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL);		// passes if the incoming depth value is less than or equal to the stored depth value

	glUseProgram(shaderID);
	updateMVP(camera);
	uploadUniforms();

	glBindVertexArray(box.vao);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	glDrawElements(GL_TRIANGLES, box.data.indices.size(), GL_UNSIGNED_INT, 0);

	//glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);	// back to default: passes if the incoming depth value is less than the stored depth value 
}


void Skybox::updateMVP(Camera camera)
{
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	//modelMatrix = glm::scale(modelMatrix, glm::vec3(5.0f));
	glm::mat4 viewMatrix = glm::mat4(glm::mat3(camera.CreateViewMatrix()));	// remove translation from view matrix;
	//glm::mat4 viewMatrix = camera.CreateViewMatrix();	// remove translation from view matrix;
	glm::mat4 projecionMatrix = glm::perspective(glm::radians(camera.fov), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
	MVP = projecionMatrix * viewMatrix * modelMatrix;
}

void Skybox::uploadUniforms()
{
	glUniformMatrix4fv(uniforms.MVPloc, 1, GL_FALSE, glm::value_ptr(MVP));
	glUniform1i(uniforms.skyboxLoc, 0);
}
