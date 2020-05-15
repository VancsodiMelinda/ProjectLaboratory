#include "PointShadow.h"

PointShadow::PointShadow(Data& object_, PointLight& light_) :
	object(object_),
	light(light_)
{
	createTexture();	// textureID
	createFBO();		// fbo
}

void PointShadow::initialize()
{
	createTexture();	// textureID
	createFBO();		// fbo
}

void PointShadow::createTexture()
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
			POINT_SHADOW_WIDTH, POINT_SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}
}

void PointShadow::createFBO()
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureID, 0);
	glDrawBuffer(GL_NONE);	// no color buffers are written
	glReadBuffer(GL_NONE);	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


PointShadow::PointShadow(GLuint shaderID_, Data& object_, glm::mat4 modelMatrix_, PointLight& light_) :
	shaderID(shaderID_),
	object(object_),
	light(light_),
	modelMatrix(modelMatrix_)
{
	configVertexAttributes();
	getUniformLocations();		// uniLocs
	//lightSpaceTransform();		// VPs
}

void PointShadow::configVertexAttributes()
{
	glBindVertexArray(object.vao);
	glBindBuffer(GL_ARRAY_BUFFER, object.vbo);

	GLuint positionAttribIndex = glGetAttribLocation(shaderID, "vertexPos");
	glEnableVertexAttribArray(positionAttribIndex);

	GLintptr vOffset = 0 * sizeof(GL_FLOAT);
	int stride3f = 3 * sizeof(GL_FLOAT);
	glVertexAttribPointer(positionAttribIndex, 3, GL_FLOAT, GL_FALSE, stride3f, (GLvoid*)vOffset);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void PointShadow::getUniformLocations()
{
	uniLocs.modelMatrixLoc	= glGetUniformLocation(shaderID, "modelMatrix");
	uniLocs.VPsLoc			= glGetUniformLocation(shaderID, "VPs");
	uniLocs.lightPosLoc		= glGetUniformLocation(shaderID, "lightPos");
	uniLocs.farPlaneLoc		= glGetUniformLocation(shaderID, "farPlane");
}

void PointShadow::lightSpaceTransform()
{
	std::vector<glm::mat4> newVPs;

	// create projection matrix
	float aspect = (float)POINT_SHADOW_WIDTH / (float)POINT_SHADOW_WIDTH;
	float near = 1.0f;
	farPlane = 100.0f;
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(90.0f), aspect, near, farPlane);
	
	
	// create view matrices
	glm::mat4 viewMatrix1 = glm::lookAt(light.params.position, light.params.position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));	// right
	glm::mat4 viewMatrix2 = glm::lookAt(light.params.position, light.params.position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));	// left
	glm::mat4 viewMatrix3 = glm::lookAt(light.params.position, light.params.position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0));		// top
	glm::mat4 viewMatrix4 = glm::lookAt(light.params.position, light.params.position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0));	// bottom
	glm::mat4 viewMatrix5 = glm::lookAt(light.params.position, light.params.position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0));	// near
	glm::mat4 viewMatrix6 = glm::lookAt(light.params.position, light.params.position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0));	// far

	// create MVPs
	//std::vector<glm::mat4> VPs;
	newVPs.push_back(projectionMatrix * viewMatrix1);
	newVPs.push_back(projectionMatrix * viewMatrix2);
	newVPs.push_back(projectionMatrix * viewMatrix3);
	newVPs.push_back(projectionMatrix * viewMatrix4);
	newVPs.push_back(projectionMatrix * viewMatrix5);
	newVPs.push_back(projectionMatrix * viewMatrix6);

	VPs = newVPs;
}

void PointShadow::render()
{
	glUseProgram(shaderID);

	lightSpaceTransform();		// VPs
	uploadUniforms();
	
	glBindVertexArray(object.vao);
	glDrawElements(GL_TRIANGLES, object.data.indices.size(), GL_UNSIGNED_INT, 0);
}

void PointShadow::uploadUniforms()
{
	glUniformMatrix4fv(uniLocs.modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(uniLocs.VPsLoc, 6, GL_FALSE, glm::value_ptr(VPs[0]));
	glUniform3fv(uniLocs.lightPosLoc, 1, glm::value_ptr(light.params.position));
	glUniform1f(uniLocs.farPlaneLoc, farPlane);
}


