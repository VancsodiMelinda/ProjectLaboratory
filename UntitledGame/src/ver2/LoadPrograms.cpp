#include "LoadPrograms.h"

LoadPrograms::LoadPrograms()
{
	InstrumentationTimer timer("Load programs");
	std::cout << "Loading shaders..." << std::endl;

	programs[0] = loadDefaultProgram();
	programs[1] = loadLightProgram();
	programs[2] = loadDirShadowProgram();
	programs[3] = loadOmniShadowProgram();
	programs[4] = loadSkyboxProgram();
	programs[5] = loadPostProcessingProgram();
	programs[6] = loadOutlineProgram();
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

ProgramContainer LoadPrograms::loadOmniShadowProgram()
{
	std::string vertexShaderFileName = "src/shaders/ShadowBox.vert";
	std::string fragmentShaderFileName = "src/shaders/ShadowBox.frag";
	std::string geometryShaderFileName = "src/shaders/ShadowBox.geom";
	CreateProgram program(vertexShaderFileName, fragmentShaderFileName, geometryShaderFileName);
	program.programContainer.type = ProgramType::omnidirectionalShadow;

	return program.programContainer;
}

ProgramContainer LoadPrograms::loadSkyboxProgram()
{
	std::string vertexShaderFileName = "src/shaders/Skybox.vert";
	std::string fragmentShaderFileName = "src/shaders/Skybox.frag";
	CreateProgram program(vertexShaderFileName, fragmentShaderFileName);
	program.programContainer.type = ProgramType::skybox;

	return program.programContainer;
}

ProgramContainer LoadPrograms::loadPostProcessingProgram()
{
	std::string vertexShaderFileName = "src/shaders/PostProcessing.vert";
	std::string fragmentShaderFileName = "src/shaders/PostProcessing.frag";
	CreateProgram program(vertexShaderFileName, fragmentShaderFileName);
	program.programContainer.type = ProgramType::postProcessing;

	return program.programContainer;
}

ProgramContainer LoadPrograms::loadOutlineProgram()
{
	std::string vertexShaderFileName = "src/shaders/Outline.vert";
	std::string fragmentShaderFileName = "src/shaders/Outline.frag";
	CreateProgram program(vertexShaderFileName, fragmentShaderFileName);
	program.programContainer.type = ProgramType::outline;

	return program.programContainer;
}
