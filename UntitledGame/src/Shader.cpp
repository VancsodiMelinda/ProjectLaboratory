#include "Shader.h"

// default constructor
Shader::Shader()
{
	//programObject = glCreateProgram();  // empty program object
	vertexShaderFileName = "src/VertexShader.txt";
	fragmentShaderFileName = "src/FragmentShader.txt";
}

Shader::Shader(std::string vertexShaderFileName_, std::string fragmentShaderFileName_)
{
	//programObject = glCreateProgram();
	vertexShaderFileName = vertexShaderFileName_;
	fragmentShaderFileName = fragmentShaderFileName_;
}

Shader::Shader(std::string vertexShaderFileName_, std::string geometryShaderFileName_, std::string fragmentShaderFileName_)
{
	vertexShaderFileName = vertexShaderFileName_;
	geometryShaderFileName = geometryShaderFileName_;
	fragmentShaderFileName = fragmentShaderFileName_;

	init3shaders();
}

void Shader::init3shaders()
{
	std::cout << "VS: " << vertexShaderFileName << "; GS: " << geometryShaderFileName << "; FS: " << fragmentShaderFileName << std::endl;
	programObject = glCreateProgram();

	// create, compile and attach shaders
	GLuint vertexShaderObject = setUpShader(GL_VERTEX_SHADER, vertexShaderFileName, programObject);
	GLuint geometryShaderObject = setUpShader(GL_GEOMETRY_SHADER, geometryShaderFileName, programObject);
	GLuint fragmentShaderObject = setUpShader(GL_FRAGMENT_SHADER, fragmentShaderFileName, programObject);

	glLinkProgram(programObject);
	linkingErrorHandling(programObject);

	// detach and delete shaders
	cleanUpShader(programObject, vertexShaderObject);
	cleanUpShader(programObject, geometryShaderObject);
	cleanUpShader(programObject, fragmentShaderObject);
}

//void Shader::runShaderCode()
void Shader::initialize()
{
	std::cout << "VS: " << vertexShaderFileName << "; FS: " << fragmentShaderFileName << std::endl;

	programObject = glCreateProgram();

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


void Shader::configVertexAttributes(GLuint vao, objectData data)  // get list of vertex attributes
{
	glBindVertexArray(vao);  // bind vao

	// iterate through list of vertex attributes
	//std::list<std::string>::iterator it;
	//for (it = vertexAttribList.begin(); it != vertexAttribList.end; ++it)
	//{
		//std::string vertexAttrib_s = *it;
		//const char* vertexAttrib_c = vertexAttrib_s.c_str();

		//GLuint attribIndex = glGetAttribLocation(programObject, vertexAttrib_c);
		//glEnableVertexAttribArray(attribIndex);
		//glVertexAttribPointer(attribIndex, 3, GL_FLOAT, GL_FALSE, stride3f, (GLvoid*)vOffset);
	//}
	
	// get attribute indexes from the relevant shader
	GLuint positionAttribIndex = glGetAttribLocation(programObject, "in_vertexPosition");	// layout (location = 0) in vec3 in_vertexPosition;
	GLuint textureAttribIndex = glGetAttribLocation(programObject, "in_textureCoords");		// layout (location = 1) in vec2 in_textureCoords;
	GLuint normalAttribIndex = glGetAttribLocation(programObject, "in_normalVec");			// layout (location = 2) in vec3 in_normalVec;

	// enable vertex attribute array
	glEnableVertexAttribArray(positionAttribIndex);
	glEnableVertexAttribArray(textureAttribIndex);
	glEnableVertexAttribArray(normalAttribIndex);

	// calculate offsets
	GLintptr vOffset = 0 * sizeof(GL_FLOAT);
	GLintptr tOffset = data.vertices.size() * sizeof(GL_FLOAT);
	GLintptr nOffset = (data.vertices.size() + data.uvs.size()) * sizeof(GL_FLOAT);
	int stride3f = 3 * sizeof(GL_FLOAT);
	int stride2f = 2 * sizeof(GL_FLOAT);

	// setup the pointers
	glVertexAttribPointer(positionAttribIndex, 3, GL_FLOAT, GL_FALSE, stride3f, (GLvoid*)vOffset);
	glVertexAttribPointer(textureAttribIndex, 2, GL_FLOAT, GL_FALSE, stride2f, (GLvoid*)tOffset);
	glVertexAttribPointer(normalAttribIndex, 3, GL_FLOAT, GL_FALSE, stride3f, (GLvoid*)nOffset);

	glBindVertexArray(0);  // unbind VAO
}


/*
void Shader::getUniformLocations()  // get list of uniform variables
{
	
	// iterate through list of uniform names
	std::list<std::string>::iterator it;
	for (it = uniformNameList.begin(); it != uniformNameList.end; ++it)
	{
		std::string uniformName_s = *it;
		const char* uniformName_c = uniformName_s.c_str();

		// get uniform location
		GLuint uniformLoc = glGetUniformLocation(programObject, uniformName_c);
		uniformLocList.push_back(uniformLoc);
	}
	
	//glGetProgramiv(programObject, GL_ACTIVE_UNIFORMS, &numberOfUniforms);
}
*/
void Shader::uploadUniform(char uniformName[], glm::mat4 uniformValue)
{
	int uniformLocation = glGetUniformLocation(programObject, uniformName);
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(uniformValue));

	/*
	GLenum uniformType;
	std::list<GLuint>::iterator it;
	int loopCounter = 0;

	for (it = uniformLocList.begin(); it != uniformLocList.end(); ++it)
	{
		GLuint uniformLocation = *it;
		glGetActiveUniform(programObject, uniformLocation, NULL, NULL, NULL, &uniformType, NULL);

		switch (uniformType)
		{
		case GL_FLOAT_MAT4:
			//glm::mat4 uniformValue = uniformValueVector[loopCounter];
			glm::mat4 uniformValue = std::get<loopCounter>(uniformValueTuple);
			//glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(uniformValue));
		}

		loopCounter = loopCounter + 1;
	}
	*/
}

void Shader::uploadUniform(char uniformName[], glm::vec3 uniformValue)
{
	int uniformLocation = glGetUniformLocation(programObject, uniformName);
	glUniform3fv(uniformLocation, 1, glm::value_ptr(uniformValue));
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

