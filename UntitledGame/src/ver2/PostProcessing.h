#pragma once

#include <glad/glad.h>
#include <stdio.h>

#include "GlobalVariables.h"
#include "DebugOpenGL.h"
#include "ModelData.h"
#include "ProgramData.h"

class PostProcessing
{
private:
	struct PostProcContainer
	{
		GLuint texture = 0;
		GLuint rbo = 0;
		GLuint fbo = 0;
		ObjectContainer object;
	};

public:
	PostProcessing(ProgramContainer program);
	PostProcContainer postProcContainer;

private:
	GLuint programID = 0;
	void checkProgram(ProgramContainer program);

	struct UniformLocs
	{
		GLint screenTextureLoc = -1;
	};
	UniformLocs uniformLocs;

	GLuint createTexture();
	GLuint createRbo();
	GLuint createFbo();
	ObjectContainer createQuad();

public:
	void render();
};

