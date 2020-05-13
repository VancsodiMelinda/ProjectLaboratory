#pragma once

#include <string>
#include <list>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

struct vector2f {
	float x;
	float y;
};

struct vector3f {
	float x;
	float y;
	float z;
};

struct vector3i {
	int x;
	int y;
	int z;
};

struct objectData {
	std::vector<float> vertices		= { 0.0f };
	std::vector<float> uvs			= { 0.0f };
	std::vector<float> normals		= { 0.0f };
	std::vector<int> indices		= { 0 };
};

class ObjLoader
{
public:
	objectData loadObjFileV2(std::string objFileName);
	objectData advancedObjLoader(std::string objFileName);
};

