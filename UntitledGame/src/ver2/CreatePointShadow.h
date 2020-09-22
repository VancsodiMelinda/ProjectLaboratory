#pragma once

#include <glad/glad.h>
#include <stdio.h>

#include "GlobalVariables.h"
#include "DebugOpenGL.h"

struct PointShadowContainer
{
	GLuint shadowCube = 0;
	GLuint fbo = 0;
};

class CreatePointShadow
{
public:
	PointShadowContainer pointShadowContainer;
	CreatePointShadow();

private:
	GLuint createTexture();
	GLuint createFbo();
};

