#include "LoadAssets.h"

LoadAssets::LoadAssets()
{
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

	CreateTexture tex6("resources/diffuse maps/rubik's cube diffuse map.png", TextureType::diffuseMap);
	diffuseMaps[5] = tex6.textureContainer;
}

void LoadAssets::loadSpecularMaps()
{
	CreateTexture tex1(TextureType::specularMap);  // load default spacular map
	specularMaps[0] = tex1.textureContainer;

	CreateTexture tex2("resources/specular maps/black.png", TextureType::specularMap);
	specularMaps[1] = tex2.textureContainer;

	CreateTexture tex3("resources/specular maps/rubik's cube specular map.png", TextureType::specularMap);
	specularMaps[2] = tex3.textureContainer;
}

void LoadAssets::loadNormalMaps()
{
	CreateTexture tex1(TextureType::normalMap);
	normalMaps[0] = tex1.textureContainer;

	CreateTexture tex2("resources/normal maps/cracked asphalt.png", TextureType::normalMap);
	normalMaps[1] = tex2.textureContainer;
}

void LoadAssets::loadObjects()
{
	
	CreateModel cube("resources/models/cube.obj", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, "y");
	cube.objectContainer.material.diffuseMap = diffuseMaps[5].ID;
	cube.objectContainer.material.specularMap = specularMaps[2].ID;
	//cube.objectContainer.material.normalMap = normalMaps[0].ID;
	cube.objectContainer.material.shininess = 0.6f * 128.0f;
	models[0] = cube.objectContainer;
	
	CreateModel sphere("resources/models/sphere.obj", glm::vec3(-3.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, "y");
	sphere.objectContainer.material.diffuseMap = diffuseMaps[1].ID;
	sphere.objectContainer.material.specularMap = specularMaps[0].ID;
	//sphere.objectContainer.material.normalMap = normalMaps[0].ID;
	sphere.objectContainer.material.shininess = 0.6f * 128.0f;
	models[1] = sphere.objectContainer;
	
	CreateModel suzanne("resources/models/suzanne.obj", glm::vec3(3.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, "y");
	suzanne.objectContainer.material.diffuseMap = diffuseMaps[2].ID;
	suzanne.objectContainer.material.specularMap = specularMaps[0].ID;
	//suzanne.objectContainer.material.normalMap = normalMaps[0].ID;
	suzanne.objectContainer.material.shininess = 0.6f * 128.0f;
	models[2] = suzanne.objectContainer;
	
	CreateModel plane("resources/models/plane.obj", glm::vec3(0.0f, -1.5f, 0.0f), glm::vec3(8.0f, 8.0f, 8.0f), 0.0f, "y");
	plane.objectContainer.material.diffuseMap = diffuseMaps[4].ID;
	plane.objectContainer.material.specularMap = specularMaps[1].ID;
	plane.objectContainer.material.normalMap = normalMaps[1].ID;
	plane.objectContainer.material.shininess = 0.6f * 128.0f;
	models[3] = plane.objectContainer;
}
