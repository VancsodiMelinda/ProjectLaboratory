#pragma once

#include <glad\glad.h>

enum struct ProgramType
{
	defaultProgram,
	object,
	light,
	directionalShadow,
	omnidirectionalShadow,
	skybox
};

struct ProgramContainer
{
	ProgramType type = ProgramType::defaultProgram;
	GLuint ID = 0;
};
