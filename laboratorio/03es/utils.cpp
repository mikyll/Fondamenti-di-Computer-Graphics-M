#include "utils.h"

glm::vec3 getPosition(glm::mat4 modelMatrix)
{
	// [3] gets the 4th column of the matrix, which is a vector4 and contains the position (x, y, z, 0/1)
	return modelMatrix[3];
}

glm::vec3 getRotation(glm::mat4 modelMatrix, bool degrees)
{
	glm::vec3 rotation = glm::vec3();

	rotation.x = atan2f(modelMatrix[1][2], modelMatrix[2][2]);
	rotation.y = atan2f(-modelMatrix[0][2], sqrtf(modelMatrix[1][2] * modelMatrix[1][2] + modelMatrix[2][2] * modelMatrix[2][2]));
	rotation.z = atan2f(modelMatrix[0][1], modelMatrix[0][0]);

	if (degrees)
	{
		rotation.x = glm::degrees(rotation.x);
		rotation.y = glm::degrees(rotation.y);
		rotation.z = glm::degrees(rotation.z);
	}

	return rotation;
}

float getScalingFactor(glm::mat4 modelMatrix)
{
	float scalingFactor;
	
	scalingFactor = sqrt(modelMatrix[0][0] * modelMatrix[0][0] +
		modelMatrix[0][1] * modelMatrix[0][1] + 
		modelMatrix[0][2] * modelMatrix[0][2]);

	return scalingFactor;
}

std::string getCoordinateSystemName(ReferenceSystem coordinateSystem)
{
	std::string result = "Unknown";

	switch (coordinateSystem)
	{
		case OCS: result = "OCS"; break;
		case WCS: result = "WCS"; break;
		default: break;
	}

	return result;
}

std::string getOperationModeName(OperationMode operationMode)
{
	std::string result = "Unknown";

	switch (operationMode)
	{
		case MODE_TRASLATING: result = "Translate"; break;
		case MODE_ROTATING: result = "Rotate"; break;
		case MODE_SCALING: result = "Scale"; break;
		case MODE_NAVIGATION: result = "Navigate"; break;
		default: break;
	}

	return result;
}

std::string getWorkingAxisName(WorkingAxis workingAxis)
{
	std::string result = "Unknown";

	switch (workingAxis)
	{
		case AXIS_X: result = "X"; break;
		case AXIS_Y: result = "Y"; break;
		case AXIS_Z: result = "Z"; break;
		default: break;
	}

	return result;
}

std::string getShaderName(ShadingType shadingType)
{
	std::string result = "Unknown";

	switch (shadingType)
	{
		case PASS_THROUGH: result = "Pass Through"; break;
		case GOURAUD: result = "Gouraud"; break;
		case PHONG: result = "Phong"; break;
		case BLINN: result = "Blinn"; break;
		case WAVE: result = "Wave"; break;
		case WAVE_COLORS: result = "Wave Colors"; break;
		case WAVE_OCEAN: result = "Wave Ocean"; break;
		case WAVE_LIGHT: result = "Wave Light"; break;
		case TOON: result = "Toon"; break;
		case TOON_V2: result = "Toon v2"; break;
		default: break;
	}

	return result;
}

Object* getObject(std::string name)
{
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects.at(i).name == name)
		{
			return &objects.at(i);
		}
	}

	return NULL;
}