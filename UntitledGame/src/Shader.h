#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "ObjLoader.h"

class Shader
{
private:
	union uniformType
	{
		glm::mat4 matrix4fv;
		glm::vec3 vector3f;
	};

public:
	// public attribute
	GLuint programObject;
	//std::list<std::string> vertexAttribList;
	std::list<std::string> uniformNameList;
	std::list<GLuint> uniformLocList;

	//GLint numberOfUniforms;

	GLuint vao;
	objectData data;

	// constructors
	Shader();  // default constructor
	Shader(std::string vertexShaderFileName_, std::string fragmentShaderFileName_);  // parameterized constructor

	Shader(std::string vertexShaderFileName_, std::string geometryShaderFileName_, std::string fragmentShaderFileName_);  // new
	void init3shaders();

	// public methods
	//void runShaderCode();
	void initialize();
	void cleanUpProgram();
	void useShader();
	void configVertexAttributes(GLuint vao, objectData data);
	void getUniformLocations();
	void uploadUniform(char uniformName[], glm::mat4 uniformValue);
	void uploadUniform(char uniformName[], glm::vec3 uniformValue);

private:
	// private attributes
	std::string vertexShaderFileName	= "";
	std::string fragmentShaderFileName	= "";
	std::string geometryShaderFileName	= "";

	// methods
	std::string shaderSourceCodeReader(std::string shaderFileName);
	GLuint setUpShader(GLenum shaderType, std::string shaderFileName, GLuint programObject);
	void shaderErrorHandling(GLuint shaderObject);
	void linkingErrorHandling(GLuint programObject);
	void cleanUpShader(GLuint programObject, GLuint shaderObject);
};

