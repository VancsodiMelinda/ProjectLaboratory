#include "LoadAssets.h"

LoadAssets::LoadAssets()
{
	InstrumentationTimer timer("Load assets");

	std::cout << "Loading textures..." << std::endl;
	loadDiffuseMaps();		// load all diffuse maps
	loadSpecularMaps();		// load all spcular maps
	loadNormalMaps();		// load all normal maps

	std::cout << std::endl << "Loading models..." << std::endl;
	loadObjects();			// load all models and assign texture maps
	std::cout << std::endl;
}

void LoadAssets::loadDiffuseMaps()
{
	InstrumentationTimer timer("Load diffuse maps");

	CreateTexture tex1(TextureType::diffuseMap);  // load default diffuse map
	diffuseMaps[0] = tex1.textureContainer;

	CreateTexture tex2("resources/diffuse maps/color grid.png", TextureType::diffuseMap);
	diffuseMaps[1] = tex2.textureContainer;

	CreateTexture tex3("resources/diffuse maps/uv grid.png", TextureType::diffuseMap);
	diffuseMaps[2] = tex3.textureContainer;

	CreateTexture tex4("resources/diffuse maps/white.png", TextureType::diffuseMap);
	diffuseMaps[3] = tex4.textureContainer;

	CreateTexture tex5("resources/diffuse maps/cracked asphalt.png", TextureType::diffuseMap);
	diffuseMaps[4] = tex5.textureContainer;

	CreateTexture tex6("resources/diffuse maps/rubik's cube colorful.png", TextureType::diffuseMap);
	diffuseMaps[5] = tex6.textureContainer;

	CreateTexture tex7("resources/diffuse maps/earth.png", TextureType::diffuseMap);
	diffuseMaps[6] = tex7.textureContainer;

	CreateTexture tex8("resources/diffuse maps/colorful gradient.jpg", TextureType::diffuseMap);
	diffuseMaps[7] = tex8.textureContainer;

	CreateTexture tex9("resources/diffuse maps/dark wood.jpg", TextureType::diffuseMap);
	diffuseMaps[8] = tex9.textureContainer;
}

void LoadAssets::loadSpecularMaps()
{
	InstrumentationTimer timer("Load specular maps");

	CreateTexture tex1(TextureType::specularMap);  // load default spacular map
	specularMaps[0] = tex1.textureContainer;

	CreateTexture tex2("resources/specular maps/black.png", TextureType::specularMap);
	specularMaps[1] = tex2.textureContainer;

	CreateTexture tex3("resources/specular maps/rubik's cube specular map.png", TextureType::specularMap);
	specularMaps[2] = tex3.textureContainer;
}

void LoadAssets::loadNormalMaps()
{
	InstrumentationTimer timer("Load normal maps");

	CreateTexture tex1(TextureType::normalMap);
	normalMaps[0] = tex1.textureContainer;

	CreateTexture tex2("resources/normal maps/cracked asphalt.png", TextureType::normalMap);
	normalMaps[1] = tex2.textureContainer;
}

void LoadAssets::loadObjects()
{
	InstrumentationTimer timer("Load models");
	
	CreateModel suzanne("resources/models/suzanne.obj", glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, "x");
	suzanne.objectContainer.material.diffuseMap = diffuseMaps[7].ID;
	suzanne.objectContainer.material.specularMap = specularMaps[0].ID;
	//suzanne.objectContainer.material.normalMap = normalMaps[0].ID;
	suzanne.objectContainer.material.shininess = 0.6f * 128.0f;
	//models[0] = suzanne.objectContainer;
	models_.push_back(suzanne.objectContainer);

	CreateModel sphere("resources/models/sphere.obj", glm::vec3(3.0f, 2.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, "y");
	sphere.objectContainer.material.diffuseMap = diffuseMaps[6].ID;
	sphere.objectContainer.material.specularMap = specularMaps[1].ID;
	//sphere.objectContainer.material.normalMap = normalMaps[0].ID;
	sphere.objectContainer.material.shininess = 0.6f * 128.0f;
	//models[1] = sphere.objectContainer;
	models_.push_back(sphere.objectContainer);

	/*
	CreateModel cube("resources/models/cube.obj", glm::vec3(-4.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, "y");
	cube.objectContainer.material.diffuseMap = diffuseMaps[5].ID;
	cube.objectContainer.material.specularMap = specularMaps[2].ID;
	//cube.objectContainer.material.normalMap = normalMaps[0].ID;
	cube.objectContainer.material.shininess = 0.6f * 128.0f;
	models[0] = cube.objectContainer;
	
	CreateModel sphere("resources/models/sphere.obj", glm::vec3(4.0f, 2.0f, 0.0f), glm::vec3(1.3f, 1.3f, 1.3f), 0.0f, "y");
	sphere.objectContainer.material.diffuseMap = diffuseMaps[6].ID;
	sphere.objectContainer.material.specularMap = specularMaps[1].ID;
	//sphere.objectContainer.material.normalMap = normalMaps[0].ID;
	sphere.objectContainer.material.shininess = 0.6f * 128.0f;
	models[1] = sphere.objectContainer;
	
	CreateModel suzanne("resources/models/suzanne.obj", glm::vec3(-4.0f, 2.3f, 0.0f), glm::vec3(0.6f, 0.6f, 0.6f), -30.0f, "x");
	suzanne.objectContainer.material.diffuseMap = diffuseMaps[2].ID;
	suzanne.objectContainer.material.specularMap = specularMaps[0].ID;
	//suzanne.objectContainer.material.normalMap = normalMaps[0].ID;
	suzanne.objectContainer.material.shininess = 0.6f * 128.0f;
	models[2] = suzanne.objectContainer;

	CreateModel raptor("resources/models/velociraptor.obj", glm::vec3(-1.0f, 1.5f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f, "y");
	raptor.objectContainer.material.diffuseMap = diffuseMaps[8].ID;
	raptor.objectContainer.material.specularMap = specularMaps[1].ID;
	//room.objectContainer.material.normalMap = normalMaps[0].ID;
	raptor.objectContainer.material.shininess = 0.6f * 128.0f;
	models[3] = raptor.objectContainer;

	
	CreateModel plane("resources/models/plane.obj", glm::vec3(0.0f, -1.5f, 0.0f), glm::vec3(8.0f, 8.0f, 8.0f), 0.0f, "y");
	plane.objectContainer.material.diffuseMap = diffuseMaps[4].ID;
	plane.objectContainer.material.specularMap = specularMaps[1].ID;
	plane.objectContainer.material.normalMap = normalMaps[1].ID;
	plane.objectContainer.material.shininess = 0.6f * 128.0f;
	models[4] = plane.objectContainer;
	*/

	/*
	CreateModel plane("resources/models/plane.obj", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 10.0f, 10.0f), 0.0f, "y");
	plane.objectContainer.material.diffuseMap = diffuseMaps[4].ID;
	plane.objectContainer.material.specularMap = specularMaps[1].ID;
	plane.objectContainer.material.normalMap = normalMaps[1].ID;
	plane.objectContainer.material.shininess = 0.6f * 128.0f;
	models[3] = plane.objectContainer;

	CreateModel plane2("resources/models/plane.obj", glm::vec3(0.0f, 5.0f, -10.0f), glm::vec3(10.0f, 10.0f, 10.0f), 90.0f, "x");
	plane2.objectContainer.material.diffuseMap = diffuseMaps[4].ID;
	plane2.objectContainer.material.specularMap = specularMaps[1].ID;
	plane2.objectContainer.material.normalMap = normalMaps[1].ID;
	plane2.objectContainer.material.shininess = 0.6f * 128.0f;
	models[4] = plane2.objectContainer;

	CreateModel plane3("resources/models/plane.obj", glm::vec3(10.0f, 5.0f, 0.0f), glm::vec3(10.0f, 10.0f, 10.0f), 90.0f, "z");
	plane3.objectContainer.material.diffuseMap = diffuseMaps[4].ID;
	plane3.objectContainer.material.specularMap = specularMaps[1].ID;
	plane3.objectContainer.material.normalMap = normalMaps[1].ID;
	plane3.objectContainer.material.shininess = 0.6f * 128.0f;
	models[5] = plane3.objectContainer;
	*/

	/*
	for (int i = 0; i < NUMBER_OF_OBJECTS; i++)
	{
		CreateModel cube("resources/models/cube.obj", glm::vec3(i * 3.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, "y");
		cube.objectContainer.material.diffuseMap = diffuseMaps[1].ID;
		cube.objectContainer.material.specularMap = specularMaps[1].ID;
		//cube.objectContainer.material.normalMap = normalMaps[0].ID;
		cube.objectContainer.material.shininess = 0.6f * 128.0f;
		models[i] = cube.objectContainer;
	}
	*/
}
