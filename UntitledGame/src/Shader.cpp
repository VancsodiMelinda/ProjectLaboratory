#include "Shader.h"

// default constructor
Shader::Shader()
{
	programObject = glCreateProgram();  // empty program object
	vertexShaderFileName = "src/VertexShader.txt";
	fragmentShaderFileName = "src/FragmentShader.txt";
}

Shader::Shader(std::string vertexShaderFileName_, std::string fragmentShaderFileName_)
{
	programObject = glCreateProgram();
	vertexShaderFileName = vertexShaderFileName_;
	fragmentShaderFileName = fragmentShaderFileName_;
}

void Shader::runShaderCode()
{
	std::cout << "VS: " << vertexShaderFileName << "; FS: " << fragmentShaderFileName << std::endl;

	GLuint vertexShaderObject = setUpShader(GL_VERTEX_SHADER, vertexShaderFileName, programObject);  // create, compile and attach vertex shader
	GLuint fragmentShaderObject = setUpShader(GL_FRAGMENT_SHADER, fragmentShaderFileName, programObject);  // create, compile and attach fragment shader

	// before linking you have to set up attribute locations, do NOT do this if layout(location = #) is in the shader code
	//glBindAttribLocation(programObject, positionLocation, "in_vertexPosition");
	//glBindAttribLocation(programObject, colorLocation, "in_vertexColor");
	//glBindAttribLocation(programObject, textureLocation, "in_vertexTexture");

	glLinkProgram(programObject);
	linkingErrorHandling(programObject);
	cleanUpShader(programObject, vertexShaderObject);
	cleanUpShader(programObject, fragmentShaderObject);

	//glUseProgram(programObject);
}

void Shader::useShader()
{
	glUseProgram(programObject);
}

std::string Shader::shaderSourceCodeReader(std::string shaderFileName)
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
		std::cout << "Shader file is NOT open: " << shaderFileName << std::endl;
	}

	return ss.str();
}


GLuint Shader::setUpShader(GLenum shaderType, std::string shaderFileName, GLuint programObject)
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


void Shader::shaderErrorHandling(GLuint shaderObject)
{
	GLint isCompiled = 0;  // shader compilation error checking
	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &isCompiled);

	if (!isCompiled)
	{
		std::cout << "Error: Shader compilation has failed!" << std::endl;
		glDeleteShader(shaderObject);
	}
	else if (isCompiled)
	{
		std::cout << "Shader compilation was successful!" << std::endl;
	}
}

void Shader::linkingErrorHandling(GLuint programObject)
{
	GLint isLinked = 0;  // program linking error checking
	glGetProgramiv(programObject, GL_LINK_STATUS, &isLinked);

	if (!isLinked)
	{
		std::cout << "Error: Program linking has failed!" << std::endl;
		glDeleteProgram(programObject);
	}
	else if (isLinked)
	{
		std::cout << "Program linking was successful!" << std::endl;
	}
}

void Shader::cleanUpShader(GLuint programObject, GLuint shaderObject)
{
	glDetachShader(programObject, shaderObject);
	glDeleteShader(shaderObject);
}

void Shader::cleanUpProgram()
{
	glUseProgram(0);
	glDeleteProgram(programObject);
}

