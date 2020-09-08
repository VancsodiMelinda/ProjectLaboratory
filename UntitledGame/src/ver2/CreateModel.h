#pragma once

#include <glad\glad.h>
#include <glm\ext\matrix_float4x4.hpp>
#include <glm\ext\matrix_transform.hpp>

#include <vector>
#include <string>

#include "ModelData.h"
#include "ObjReader.h"

class CreateModel
{
public:
	ObjectContainer objectContainer;
	CreateModel();
	CreateModel(std::string objectFileName);
	CreateModel(std::string objectFileName, glm::vec3 translate, glm::vec3 scale, float rotate, std::string rotateAxis);

private:
	void loadObjectData(std::string objectFileName);
	void createVAOandVBOs();
	void fillVBOs();
	glm::mat4 createModelMatrix(glm::vec3 translate, glm::vec3 scale, float rotateAngle, std::string rotateAxis);
};
