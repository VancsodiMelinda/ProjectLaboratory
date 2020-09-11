#include "CreateProgram.h"


CreateProgram::CreateProgram(std::string vertexShaderFileName, std::string fragmentShaderFileName)
{
	std::cout << vertexShaderFileName << ", " << fragmentShaderFileName << std::endl;
	programContainer.ID = glCreateProgram();	// create empty program object

	GLuint vertexShaderObject = setUpShader(GL_VERTEX_SHADER, vertexShaderFileName, programContainer.ID);			// create, compile and attach vertex shader
	GLuint fragmentShaderObject = setUpShader(GL_FRAGMENT_SHADER, fragmentShaderFileName, programContainer.ID);		// create, compile and attach fragment shader

	glLinkProgram(programContainer.ID);			// attached shaders will create an executable
	linkingErrorHandling(programContainer.ID);

	cleanUpShader(programContainer.ID, vertexShaderObject);
	cleanUpShader(programContainer.ID, fragmentShaderObject);
	std::cout << std::endl;
}

CreateProgram::CreateProgram(std::string vertexShaderFileName, std::string fragmentShaderFileName, std::string geometryShaderFileName)
{
	std::cout << vertexShaderFileName << ", " << fragmentShaderFileName << ", " << geometryShaderFileName << std::endl;
	programContainer.ID = glCreateProgram();	// create empty program object

	GLuint vertexShaderObject = setUpShader(GL_VERTEX_SHADER, vertexShaderFileName, programContainer.ID);			// create, compile and attach vertex shader
	GLuint fragmentShaderObject = setUpShader(GL_FRAGMENT_SHADER, fragmentShaderFileName, programContainer.ID);		// create, compile and attach fragment shader
	GLuint geometryShaderObject = setUpShader(GL_GEOMETRY_SHADER, geometryShaderFileName, programContainer.ID);		// create, compile and attach geometry shader

	glLinkProgram(programContainer.ID);			// attached shaders will create an executable
	linkingErrorHandling(programContainer.ID);

	cleanUpShader(programContainer.ID, vertexShaderObject);
	cleanUpShader(programContainer.ID, fragmentShaderObject);
	cleanUpShader(programContainer.ID, geometryShaderObject);
	std::cout << std::endl;
}


std::string CreateProgram::shaderSourceCodeReader(std::string shaderFileName)
{
	std::ifstream ifs(shaderFileName, std::ifstream::in);
	std::string line;
	std::stringstream ss;

	if (ifs.is_open())
	{
		while (std::getline(ifs, line))
		{
			ss << line << std::endl;
		}
	}
	else
	{
		std::cout << "ERROR: Can't open shader source code!" << std::endl;
	}

	return ss.str();
}

GLuint CreateProgram::setUpShader(GLenum shaderType, std::string shaderFileName, GLuint programObject)
{
	GLuint shaderObject = glCreateShader(shaderType);  // empty shader object
	std::string shaderSrc = shaderSourceCodeReader(shaderFileName);  // read shader source code
	const GLchar* shaderSrc_ = (const GLchar*)shaderSrc.c_str();  // convert string to GLchar*
	glShaderSource(shaderObject, 1, &shaderSrc_, 0);  // give the shader src to the shader object, copy shader into internal memory
	glCompileShader(shaderObject);  // compile shader object

	shaderErrorHandling(shaderObject);

	glAttachShader(programObject, shaderObject);  // attach shader object to the program object
	return shaderObject;
}

void CreateProgram::shaderErrorHandling(GLuint shaderObject)
{
	GLint isCompiled = 0;  // shader compilation error checking
	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &isCompiled);

	if (!isCompiled)
	{
		std::cout << "ERROR: Shader compilation has failed!" << std::endl;
		glDeleteShader(shaderObject);
	}
	else if (isCompiled)
	{
		std::cout << "OK: Shader compilation was successful!" << std::endl;
	}
}

void CreateProgram::linkingErrorHandling(GLuint programObject)
{
	GLint isLinked = 0;  // program linking error checking
	glGetProgramiv(programObject, GL_LINK_STATUS, &isLinked);

	if (!isLinked)
	{
		std::cout << "ERROR: Program linking has failed!" << std::endl;
		glDeleteProgram(programObject);
	}
	else if (isLinked)
	{
		std::cout << "OK: Program linking was successful!" << std::endl;
	}
}

void CreateProgram::cleanUpShader(GLuint programObject, GLuint shaderObject)
{
	glDetachShader(programObject, shaderObject);
	glDeleteShader(shaderObject);
}
