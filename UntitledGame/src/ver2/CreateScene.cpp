#include "CreateScene.h"



CreateScene::CreateScene()
{
	// LOADING
	LoadAssets assets;		// ObjectContainer models[1];
	LoadPrograms programs;	// array of shaders
	// LoadLights lights;	// arrays of different light types
	// LoadSkyboxes skyboxes;	// array of skyboxes

	// CREATION
	// create shadows with iterating through the lights and assets

	// CONFIG
	// config vertex attribs - object shader
	// get uniform locs - object shader

	for (int i = 0; i < sizeof(assets.models) / sizeof(assets.models[0]); i++)
	{
		// config vertex attributes
		// get uniform locations
		// assets.config(essets.models[i]);
	}

	// RENDER IN GAME LOOP
	// render shadows
	// render objects
	// render skybox

}
