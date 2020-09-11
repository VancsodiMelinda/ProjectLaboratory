#include "LoadPrograms.h"

LoadPrograms::LoadPrograms()
{
	std::cout << "Loading shaders..." << std::endl;
	programs[0] = loadDefaultProgram();
}

ProgramContainer LoadPrograms::loadDefaultProgram()
{
	std::string vertexShaderFileName = "src/shaders/Default.vert";
	std::string fragmentShaderFileName = "src/shaders/Default.frag";
	CreateProgram program(vertexShaderFileName, fragmentShaderFileName);
	program.programContainer.type = ProgramType::defaultProgram;

	return program.programContainer;
}

ProgramContainer LoadPrograms::loadObjectProgram()
{
	std::string vertexShaderFileName = "src/shaders/Object.vert";
	std::string fragmentShaderFileName = "src/shaders/Object.frag";
	CreateProgram program(vertexShaderFileName, fragmentShaderFileName);
	program.programContainer.type = ProgramType::object;

	return program.programContainer;
}
