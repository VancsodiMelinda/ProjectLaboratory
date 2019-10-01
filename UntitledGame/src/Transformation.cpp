#include "Transformation.h"

float Transformation::degreeToRadian(float degree)
{
	return (degree * 2.0f * 3.14f) / 360.0f;
}

void Transformation::makeRotationMatrix(float rotationMatrix[16], std::string axis, float angle)
{
	float angleRadian = (angle * (float)2.0 * (float)3.14) / (float)360.0;

	if (axis == "x")
	{
		// 1st column
		rotationMatrix[0] = 1.0f;
		rotationMatrix[1] = 0.0f;
		rotationMatrix[2] = 0.0f;
		rotationMatrix[3] = 0.0f;

		// 2nd column
		rotationMatrix[4] = 0.0f;
		rotationMatrix[5] = (float)cos(angleRadian);
		rotationMatrix[6] = (float)sin(angleRadian);
		rotationMatrix[7] = 0.0f;

		// 3rd column
		rotationMatrix[8] = 0.0f;
		rotationMatrix[9] = -1.0f * (float)sin(angleRadian);
		rotationMatrix[10] = (float)cos(angleRadian);
		rotationMatrix[11] = 0.0f;

		// 4th column
		rotationMatrix[12] = 0.0f;
		rotationMatrix[13] = 0.0f;
		rotationMatrix[14] = 0.0f;
		rotationMatrix[15] = 1.0f;
	}
	else if (axis == "y")
	{
		// 1st column
		rotationMatrix[0] = (float)cos(angleRadian);
		rotationMatrix[1] = 0.0f;
		rotationMatrix[2] = -1.0f * (float)sin(angleRadian);
		rotationMatrix[3] = 0.0f;

		// 2nd column
		rotationMatrix[4] = 0.0f;
		rotationMatrix[5] = 1.0f;
		rotationMatrix[6] = 0.0f;
		rotationMatrix[7] = 0.0f;

		// 3rd column
		rotationMatrix[8] = (float)sin(angleRadian);
		rotationMatrix[9] = 0.0f;
		rotationMatrix[10] = (float)cos(angleRadian);
		rotationMatrix[11] = 0.0f;

		// 4th column
		rotationMatrix[12] = 0.0f;
		rotationMatrix[13] = 0.0f;
		rotationMatrix[14] = 0.0f;
		rotationMatrix[15] = 1.0f;
	}
	else if (axis == "z")
	{
		// 1st column
		rotationMatrix[0] = (float)cos(angleRadian);
		rotationMatrix[1] = (float)sin(angleRadian);
		rotationMatrix[2] = 0.0f;
		rotationMatrix[3] = 0.0f;

		// 2nd column
		rotationMatrix[4] = -1.0f * (float)sin(angleRadian);
		rotationMatrix[5] = (float)cos(angleRadian);
		rotationMatrix[6] = 0.0f;
		rotationMatrix[7] = 0.0f;

		// 3rd column
		rotationMatrix[8] = 0.0f;
		rotationMatrix[9] = 0.0f;
		rotationMatrix[10] = 1.0f;
		rotationMatrix[11] = 0.0f;

		// 4th column
		rotationMatrix[12] = 0.0f;
		rotationMatrix[13] = 0.0f;
		rotationMatrix[14] = 0.0f;
		rotationMatrix[15] = 1.0f;
	}
}

void Transformation::makeTranslationMatrix(float translationMatrix[16], float x, float y, float z)  // good
{
	translationMatrix[0] = 1.0f;
	translationMatrix[1] = 0.0f;
	translationMatrix[2] = 0.0f;
	translationMatrix[3] = 0.0f;

	translationMatrix[4] = 0.0f;
	translationMatrix[5] = 1.0f;
	translationMatrix[6] = 0.0f;
	translationMatrix[7] = 0.0f;

	translationMatrix[8] = 0.0f;
	translationMatrix[9] = 0.0f;
	translationMatrix[10] = 1.0f;
	translationMatrix[11] = 0.0f;

	translationMatrix[12] = x;
	translationMatrix[13] = y;
	translationMatrix[14] = z;
	translationMatrix[15] = 1.0f;
}

void Transformation::makeScalingMatrix(float scalingMatrix[16], float x, float y, float z)
{
	scalingMatrix[0] = x;
	scalingMatrix[1] = 0.0f;
	scalingMatrix[2] = 0.0f;
	scalingMatrix[3] = 0.0f;

	scalingMatrix[4] = 0.0f;
	scalingMatrix[5] = y;
	scalingMatrix[6] = 0.0f;
	scalingMatrix[7] = 0.0f;

	scalingMatrix[8] = 0.0f;
	scalingMatrix[9] = 0.0f;
	scalingMatrix[10] = z;
	scalingMatrix[11] = 0.0f;

	scalingMatrix[12] = 0.0f;
	scalingMatrix[13] = 0.0f;
	scalingMatrix[14] = 0.0f;
	scalingMatrix[15] = 1.0f;
}

void Transformation::makePerspectiveProjectionMatrix(float projectionMatrix[16], int windowWidth, int windowHeight)
{
	float aspectRatio = (float)windowWidth / (float)windowHeight;
	float fieldOfView = 70.0f;
	float nearPlane = 0.1f;
	float farPlane = 1000.0f;
	float zm = farPlane - nearPlane;
	float zp = farPlane + nearPlane;

	projectionMatrix[0] = (1.0f / (float)tan(degreeToRadian(fieldOfView / 2.0f))) / aspectRatio;
	projectionMatrix[1] = 0.0f;
	projectionMatrix[2] = 0.0f;
	projectionMatrix[3] = 0.0f;

	projectionMatrix[4] = 0.0f;
	projectionMatrix[5] = 1.0f / (float)tan(degreeToRadian(fieldOfView / 2.0f));
	projectionMatrix[6] = 0.0f;
	projectionMatrix[7] = 0.0f;

	projectionMatrix[8] = 0.0f;
	projectionMatrix[9] = 0.0f;
	projectionMatrix[10] = (-1.0f * zp) / zm;
	projectionMatrix[11] = 0.0f;

	projectionMatrix[12] = 0.0f;
	projectionMatrix[13] = 0.0f;
	projectionMatrix[14] = (-1.0f * (2.0f * farPlane * nearPlane)) / zm;
	projectionMatrix[15] = 1.0f;
}