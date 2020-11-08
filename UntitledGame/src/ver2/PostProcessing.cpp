#include "PostProcessing.h"

PostProcessing::PostProcessing(ProgramContainer program)
{
	std::cout << "Loading post-processing data..." << std::endl;
	checkProgram(program);
	programID = program.ID;
	postProcContainer.texture = createTexture();	// for color attachment
	postProcContainer.selectionTexture = createSelectionTexture();  // for object selection
	postProcContainer.rbo = createRbo();			// for depth and stencil attachment
	postProcContainer.fbo = createFbo();
	postProcContainer.object = createQuad();		// create data, vao, vbo, ibo, config vertex attribs, get uniform locs

	std::cout << std::endl;
}

void PostProcessing::checkProgram(ProgramContainer program)
{
	if (program.type == ProgramType::postProcessing)
	{
		std::cout << "OK: Using the correct program for post-processing." << std::endl;
	}
	else
	{
		std::cout << "ERROR: Using the incorrect program for post-processing." << std::endl;
	}
}

GLuint PostProcessing::createTexture()
{
	GLuint texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}

GLuint PostProcessing::createSelectionTexture()
{
	GLuint texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}

GLuint PostProcessing::createRbo()
{
	GLuint rbo;  // renderbuffer object

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WINDOW_WIDTH, WINDOW_HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	return rbo;
}

GLuint PostProcessing::createFbo()
{
	GLuint fbo;

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, postProcContainer.texture, 0);  // color attachment #1
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, postProcContainer.selectionTexture, 0);  // color attachment #2
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, postProcContainer.rbo);  // depth and stencil attachment

	GLenum buffersToDraw[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, buffersToDraw);

	debugFramebuffer(GL_FRAMEBUFFER);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return fbo;
}

ObjectContainer PostProcessing::createQuad()
{
	// generate quad data
	ObjectContainer object;

	ObjectData data;

	data.vertices = { -1.0f,  1.0f, 0.0f,
					-1.0f, -1.0f, 0.0f,
					1.0f, -1.0f, 0.0f,
					1.0f,  1.0f, 0.0f };

	data.uvs = { 0.0f, 1.0f,
				0.0f, 0.0f,
				1.0f, 0.0f,
				1.0f, 1.0f };

	data.indices = { 0, 1, 2,
					0, 2, 3 };

	object.data = data;

	// create VAO, VBO, IBO
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	object.vao = vao;

	GLuint vbo;
	glGenBuffers(1, &vbo);
	object.vbo = vbo;

	GLuint ibo;
	glGenBuffers(1, &ibo);
	object.ibo = ibo;

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, (data.vertices.size() + data.uvs.size()) * sizeof(GL_FLOAT), 0, GL_STATIC_DRAW);  // reserve space
	glBufferSubData(GL_ARRAY_BUFFER, 0, data.vertices.size() * sizeof(GL_FLOAT), &data.vertices[0]);  // VERTEX COORDINATES
	glBufferSubData(GL_ARRAY_BUFFER, data.vertices.size() * sizeof(GL_FLOAT), data.uvs.size() * sizeof(GL_FLOAT), &data.uvs[0]);  // TEXTURE COORDINATES
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indices.size() * sizeof(GLuint), &data.indices[0], GL_STATIC_DRAW);  // INDICES

	glBindVertexArray(0);

	// config vertex attributes
	glBindVertexArray(vao);  // bind VAO
	glBindBuffer(GL_ARRAY_BUFFER, vbo);  // bind VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);  // bind IBO

	GLuint positionAttribIndex = glGetAttribLocation(programID, "vertices");	// layout (location = 0) in vec3 in_vertexPosition;
	GLuint textureAttribIndex = glGetAttribLocation(programID, "uvs");		// layout (location = 1) in vec2 in_textureCoords;

	glEnableVertexAttribArray(positionAttribIndex);
	glEnableVertexAttribArray(textureAttribIndex);

	GLintptr vOffset = 0 * sizeof(GL_FLOAT);
	GLintptr tOffset = data.vertices.size() * sizeof(GL_FLOAT);
	int stride3f = 3 * sizeof(GL_FLOAT);
	int stride2f = 2 * sizeof(GL_FLOAT);

	glVertexAttribPointer(positionAttribIndex, 3, GL_FLOAT, GL_FALSE, stride3f, (GLvoid*)vOffset);
	glVertexAttribPointer(textureAttribIndex, 2, GL_FLOAT, GL_FALSE, stride2f, (GLvoid*)tOffset);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// get uniform locations
	uniformLocs.screenTextureLoc = glGetUniformLocation(programID, "screenTexture");
	uniformLocs.selectionTextureLoc = glGetUniformLocation(programID, "selectionTexture");

	return object;
}

void PostProcessing::render()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	glUseProgram(programID);

	glUniform1i(uniformLocs.screenTextureLoc, 0);
	glUniform1i(uniformLocs.selectionTextureLoc, 1);

	glBindVertexArray(postProcContainer.object.vao);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, postProcContainer.texture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, postProcContainer.selectionTexture);

	glDrawElements(GL_TRIANGLES, postProcContainer.object.data.indices.size(), GL_UNSIGNED_INT, 0);
}
