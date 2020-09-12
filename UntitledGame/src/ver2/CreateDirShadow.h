#pragma once

#include <glad/glad.h>
#include <stdio.h>

#include "GlobalVariables.h"

struct DirShadowContainer
{
	GLuint shadowMap = 0;
	GLuint fbo = 0;
};

class CreateDirShadow
{
public:
	DirShadowContainer dirShadowContainer;
	CreateDirShadow();

private:
	GLuint createTexture();
	GLuint createFbo();
};

