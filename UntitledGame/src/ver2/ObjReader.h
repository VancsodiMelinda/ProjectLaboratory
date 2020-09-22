#pragma once

#include <string>
#include <list>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

#include "ModelData.h"

class ObjReader
{
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

public:
	//ObjectData readObjFile(std::string objFileName);
	void readObjFile(std::string objFileName);
	ObjectData data;

private:
	void calculateTangentsAndBitangents(std::vector<float>& vertices, std::vector<float>& uvs, std::vector<int>& indices);
	void calculateForOneVertex(glm::vec3 edge1, glm::vec3 edge2, glm::vec2 deltaUV1, glm::vec2 deltaUV2);
};