#pragma once
#include <string>

class Transformation
{
public:
	float degreeToRadian(float angle);
	void makeRotationMatrix(float rotationMatrix[16], std::string axis, float angle);
	void makeTranslationMatrix(float translationMatrix[16], float x, float y, float z);
	void makeScalingMatrix(float scalingMatrix[16], float x, float y, float z);
	void makePerspectiveProjectionMatrix(float projectionMatrix[16], int windowWidth, int windowHeight);
};

