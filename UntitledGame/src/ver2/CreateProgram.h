#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "ProgramData.h"

class CreateProgram
{
public:
	ProgramContainer programContainer;
	CreateProgram(std::string vertexShader, std::string fragmentShader);
	CreateProgram(std::string vertexShaderFileName, std::string fragmentShaderFileName, std::string geometryShaderFileName);

private:
	std::string shaderSourceCodeReader(std::string shaderFileName);
	GLuint setUpShader(GLenum shaderType, std::string shaderFileName, GLuint programObject);
	void shaderErrorHandling(GLuint shaderObject);
	void linkingErrorHandling(GLuint programObject);
	void cleanUpShader(GLuint programObject, GLuint shaderObject);
};

