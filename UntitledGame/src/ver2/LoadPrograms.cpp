#include "LoadPrograms.h"

LoadPrograms::LoadPrograms()
{
	std::cout << "Loading shaders..." << std::endl;

	programs[0] = loadDefaultProgram();
	programs[1] = loadLightProgram();
	programs[2] = loadDirShadowProgram();
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

ProgramContainer LoadPrograms::loadLightProgram()
{
	std::string vertexShaderFileName = "src/shaders/Light.vert";
	std::string fragmentShaderFileName = "src/shaders/Light.frag";
	CreateProgram program(vertexShaderFileName, fragmentShaderFileName);
	program.programContainer.type = ProgramType::light;

	return program.programContainer;
}

ProgramContainer LoadPrograms::loadDirShadowProgram()
{
	std::string vertexShaderFileName = "src/shaders/Shadow.vert";
	std::string fragmentShaderFileName = "src/shaders/Shadow.frag";
	CreateProgram program(vertexShaderFileName, fragmentShaderFileName);
	program.programContainer.type = ProgramType::directionalShadow;

	return program.programContainer;
}
