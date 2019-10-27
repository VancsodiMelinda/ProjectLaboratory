#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	// public attribute
	GLuint programObject;

	// constructors
	Shader();  // default constructor
	Shader(GLuint programObject_, std::string vertexShaderFileName_, std::string fragmentShaderFileName_);  // parameterized constructor

	// methods
	void runShaderCode();
	void cleanUpProgram();

private:
	// private attributes
	std::string vertexShaderFileName;
	std::string fragmentShaderFileName;

	// methods
	std::string shaderSourceCodeReader(std::string shaderFileName);
	GLuint setUpShader(GLenum shaderType, std::string shaderFileName, GLuint programObject);
	void shaderErrorHandling(GLuint shaderObject);
	void linkingErrorHandling(GLuint programObject);
	void cleanUpShader(GLuint programObject, GLuint shaderObject);
};

