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
	ObjectData readObjFile(std::string objFileName);

};